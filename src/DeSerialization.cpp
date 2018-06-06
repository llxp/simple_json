/*
MIT License

Copyright (c) 2018 Lukas Lüdke

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <simple_json\DeSerialization.h>
#include <stdio.h>
#include <ctype.h>
#include <iostream>
#include <memory>
#include <utility>
#include <string>

#include <simple_json\VectorBase.h>
#include <simple_json\SerializationUtils.h>

/*DLLEXPORT JsonParser::DeSerialization &JsonParser::DeSerialization::operator=(JsonParser::DeSerialization &&other)
{
	serialize();
	std::cout << __FUNCTION__ << std::endl;
	return *this;
}

DLLEXPORT JsonParser::DeSerialization &JsonParser::DeSerialization::operator=(JsonParser::DeSerialization & const other)
{
	serialize();
	std::cout << __FUNCTION__ << std::endl;
	return *this;
}*/

constexpr size_t constLength(const char* str)
{
	return (*str == 0) ? 0 : constLength(str + 1) + 1;
}

bool JsonParser::DeSerialization::fromString()
{
	if (strLen() > 0) {
		bool parsed = fromString(0) == this->strLen() - 1;
		if (parsed) {
			return true;
		} else {
			return false;
		}
	}
	return true;
}

bool JsonParser::DeSerialization::fromString(
	const std::shared_ptr<JsonString> &str)
{
	this->setFullString(str.get());
	return this->fromString();
}

size_t JsonParser::DeSerialization::fromString(const size_t &currentPos)
{
	this->clearMapping();
	this->serialize();
	size_t openingCount = 0;
	size_t closeCount = 0;
	size_t tempLen = strLen();
	for (size_t i = currentPos; i < tempLen; ++i) {
		switch (getChar(i)) {
		case JsonObjectOpen:
			++openingCount;
			if ((i = addKVPair(i)) <= 0) {
				// Expecting position of JsonObjectClose as result
				return 0;
			}
			break;
		case JsonArrayOpen:
			++openingCount;
			if ((i = addArrayValues(i)) <= 0) {
				return 0;
			}
			break;
		}
		switch (getChar(i)) {
		case JsonObjectClose:
			++closeCount;
			break;
		case JsonArrayClose:
			++closeCount;
		}
		if (openingCount > 0 && openingCount == closeCount) {
			return i;
		}
	}
	return 0;
}

size_t JsonParser::DeSerialization::fromStringArray(const size_t &currentPos)
{
	size_t openingCount = 0;
	size_t closeCount = 0;
	size_t tempLen = strLen();
	for (size_t i = currentPos; i < tempLen; ++i) {
		switch (getChar(i)) {
		case ' ':
		case '\n':
		case '\t':
		case '\r':
			continue;
		case JsonArrayOpen:
			++openingCount;
			if ((i = addArrayValues(i)) <= 0
				// Expecting position of JsonArrayClose as result
				) {
				return 0;
			}
			break;
		}
		switch (getChar(i)) {
		case JsonArrayClose:
			++closeCount;
		}
		if (openingCount == closeCount) {
			return i;
		}
	}
	return 0;
}


size_t JsonParser::DeSerialization::addArrayValues(const size_t &currentPos)
{
	bool commaFound = true;
	size_t tempLen = strLen();
	char c = 0;
	for (size_t i = currentPos + 1; i < tempLen; ++i) {
		switch ((c = getChar(i))) {
		case '\n':
		case ' ':
		case '\t':
		case '\r':
			continue;
		case JsonObjectOpen:
		{
			if ((i = addObjectToArray(i)) <= 0
				// Expecting position of JsonObjectClose as result
				|| !commaFound) {
				return 0;
			}
			setType(JsonTypes::ObjectArray);
			commaFound = false;
		}
		break;
		case JsonArrayOpen:
		{
			if ((i = addArrayToArray(i)) <= 0
				|| !commaFound) {
				return 0;
			}
			setType(JsonTypes::ArrayArray);
			commaFound = false;
		}
		break;
		case JsonStringSeparator:
		{
			if ((i = addStringToArray(i)) <= 0
				|| !commaFound) {
				return 0;
			}
			setType(JsonTypes::StringArray);
			commaFound = false;
		}
		break;
		case JsonEntrySeparator:
			commaFound = true;
			break;
		case JsonArrayClose:
			if (!commaFound) {
				return i;
			}
			break;
		default:
			if (isNumber(c)) {
				if ((i = addNumberToArray(i)) <= 0
					|| !commaFound) {
					return 0;
				}
				setType(JsonTypes::NumberArray);
				commaFound = false;
			} else if (isBool(c)) {
				if ((i = addBoolToArray(i)) <= 0
					|| !commaFound) {
					return 0;
				}
				setType(JsonTypes::BoolArray);
				commaFound = false;
			}
		}
	}
	return 0;
}


inline bool JsonParser::DeSerialization::isEscape(
	char ch1, char ch2) const
{
	bool currentPosIsEscape = ch1  == '\\';
	if (currentPosIsEscape) {
		bool previousPosIsEscaped =
			ch2 == '\\';
		return !previousPosIsEscaped;
	}
	return currentPosIsEscape;
}


size_t JsonParser::DeSerialization::addKVPair(const size_t &currentPos)
{
	size_t tempLen = strLen();
	JsonString name;
	for (size_t i = currentPos + 1; i < tempLen; ++i) {
		switch (getChar(i)) {
		case ' ':
		case '\n':
		case '\t':
		case '\r':
			continue;
		case JsonStringSeparator:
			if ((i = getString(i, name)) <= 0
				// Expecting position of closing String quotes as result
				) {
				return 0;
			}
			if (name.length() == 0) {
				return 0;
			}
			break;
		case JsonKvSeparator:
			if ((i = addValue(i, std::move(name))) <= 0
				// Expecting position of JsonObjectClose or
				// JsonArrayClose as result
				) {
				return 0;
			}
			break;
		}
		switch (getChar(i)) {
		case JsonArrayClose:
		case JsonObjectClose:
			return i;
		}
	}
	return 0;
}


inline size_t JsonParser::DeSerialization::getString(
	const size_t &currentPos, JsonString &name) const
{
	size_t tempLen = strLen();
	char cOld1 = 0;
	char cOld2 = 0;
	for (size_t i = currentPos + 1; i < tempLen; ++i) {
		char c = getChar(i);
		if (getChar(i) == JsonStringSeparator && !isEscape(cOld1, cOld2)) {
			name = std::move(substr(currentPos + 1, i - (currentPos + 1)));
			return i;
		}
		cOld2 = cOld1;
		cOld1 = c;
	}
	return 0;
}


size_t JsonParser::DeSerialization::addValue(
	const size_t &currentPos, JsonString &&name)
{
	bool commaFound = true;
	bool valueSet = false;
	const size_t tempLen = strLen();
	char c = 0;
	for (size_t i = currentPos + 1; i < tempLen; ++i) {
		switch ((c = getChar(i))) {
		case ' ':
		case '\n':
		case '\t':
		case '\r':
			continue;
		case JsonStringSeparator:
			if ((i = addStringValue(i, std::move(name))) <= 0
				|| !commaFound) {
				return 0;
			}
			valueSet = true;
			commaFound = false;
			break;
		case JsonObjectOpen:
			if ((i = addObjectValue(i, std::move(name))) <= 0
				|| !commaFound) {
				return 0;
			}
			valueSet = true;
			commaFound = false;
			break;
		case JsonArrayOpen:
			if ((i = addArrayValue(i, std::move(name))) <= 0
				|| !commaFound) {
				return 0;
			}
			valueSet = true;
			commaFound = false;
			break;
		case JsonEntrySeparator:
			if (valueSet && !commaFound) {
				return i;
			}
			commaFound = true;
			break;
		case JsonObjectClose:
			if (!commaFound) {
				return i;
			}
			break;
		default:
			if (isNumber(c)) {
				if ((i = addNumberValue(i, std::move(name))) <= 0 || !commaFound) {
					return 0;
				}
				valueSet = true;
				commaFound = false;
			} else if (isBool(c)) {
				if ((i = addBoolValue(i, std::move(name))) <= 0 || !commaFound) {
					return 0;
				}
				valueSet = true;
				commaFound = false;
			} else if (isNull(c)) {
				if (!commaFound) {
					return 0;
				}
				addNullValue(std::move(name));
				i = i + constLength("null") - 1;
				valueSet = true;
				commaFound = false;
			}
		}
		if (i >= tempLen) {
			return 0;
		}
	}
	return 0;
}


inline bool JsonParser::DeSerialization::isNumber(char c)
{
	if ((c >= 48
		&& c <= 57)
		|| c == '.'
		|| c == '-') {
		return true;
	}
	return false;
}


inline bool JsonParser::DeSerialization::isBool(char c)
{
	switch (c) {
	case 't':
	case 'T':
	case 'f':
	case 'F':
		return true;
	}
	return false;
}

inline bool JsonParser::DeSerialization::isNull(char c)
{
	switch (c) {
	case 'n':
	case 'N':
		return true;
	}
	return false;
}

/*################################################################
		Begin name value pair parsing related functions
################################################################*/

size_t JsonParser::DeSerialization::addStringValue(
	const size_t &currentPos, JsonString &&name)
{
	size_t i = currentPos;
	JsonString value;
	if ((i = this->getString(i, value)) <= 0) {
		return 0;
	}
	if (this->m_kvPairMapping.find(name)
		!= this->m_kvPairMapping.end()) {
		JsonString *ptr = static_cast<JsonString *>(this->m_kvPairMapping[std::move(name)]);
		(*ptr) = std::move(value);
	}
	return i;
}


size_t JsonParser::DeSerialization::addObjectValue(
	const size_t &currentPos, JsonString &&name)
{
	size_t i = currentPos;
	if (this->m_kvPairMapping.find(name)
		!= this->m_kvPairMapping.end()) {
		DeSerialization *obj = static_cast<DeSerialization *>(
			this->m_kvPairMapping[std::move(name)]);
		if (obj != nullptr)
		{
			obj->clearValues();
			obj->setFullString(this->fullString());
			if ((i = obj->fromString(i)) <= 0) {
				return 0;
			}
			return i;
		}
	} else {
		DeSerialization obj;
		obj.setFullString(this->fullString());
		if ((i = obj.fromString(i)) <= 0) {
			return 0;
		}
		return i;
	}
	return 0;
}


size_t JsonParser::DeSerialization::addArrayValue(
	const size_t &currentPos, JsonString &&name)
{
	size_t pos = currentPos;
	if (this->m_kvPairMapping.find(name)
		!= this->m_kvPairMapping.end()) {
		DeSerialization *obj = static_cast<DeSerialization *>(this->m_kvPairMapping[std::move(name)]);
		if (obj != nullptr)
		{
			obj->clearValues();
			obj->setFullString(this->fullString());
			if ((pos = obj->fromStringArray(pos)) <= 0) {
				return 0;
			}
			return pos;
		}
	} else {
		DeSerialization obj;
		obj.setFullString(this->fullString());
		if ((pos = obj.fromStringArray(pos)) <= 0) {
			return 0;
		}
		return pos;
	}
	return 0;
}


size_t JsonParser::DeSerialization::addNumberValue(
	const size_t &currentPos, const JsonString &name)
{
	const size_t tempLen = strLen();
	for (size_t i = currentPos; i < tempLen; ++i) {
		if (!isNumber(getChar(i))) {
			auto ptr = this->m_kvPairMapping.find(name);
			if (ptr != this->m_kvPairMapping.end() && ptr->second != nullptr) {
				JsonParser::Number *number = static_cast<JsonParser::Number *>(ptr->second);
				if (number != nullptr) {
					number->setNumberRefValue(std::move(substr(currentPos, i - currentPos)));
				}
			}
			return i - 1;
		}
	}
	return 0;
}


size_t JsonParser::DeSerialization::addBoolValue(
	const size_t &currentPos, JsonString &&name)
{
	switch (getChar(currentPos)) {
	case 't':  // assigning true to the name
	{
		//m_workers.push_back(std::thread([this, name]() {
			auto ptr = this->m_kvPairMapping.find(name);
			if (ptr != this->m_kvPairMapping.end() && ptr->second != nullptr) {
				bool *boolean = static_cast<bool *>(ptr->second);
				(*boolean) = true;
			}
		//}));
	}
		return currentPos + constLength("True") - 1;
	case 'f':  // assigning false to the name
		//m_workers.push_back(std::thread([this, name]() {
			auto ptr = this->m_kvPairMapping.find(name);
			if (ptr != this->m_kvPairMapping.end() && ptr->second != nullptr) {
				bool *boolean = static_cast<bool *>(ptr->second);
				(*boolean) = false;
			}
		//}));
		return currentPos + constLength("False") - 1;
	}
	return 0;
}

void JsonParser::DeSerialization::addNullValue(JsonString &&name)
{
	// this->kvPairNullValues()->push_back(std::move(name));
}

/*################################################################
		End name value pair parsing related functions
################################################################*/

/*################################################################
			Begin Array parsing related functions
################################################################*/
size_t JsonParser::DeSerialization::addArrayToArray(const size_t &currentPos)
{
	size_t i = currentPos;
	if (m_mappingArrayArrays != nullptr) {
		void *newElement = this->m_mappingArrayArrays->addNew();
		if (newElement != nullptr) {
			JsonParser::DeSerialization *element =
				static_cast<JsonParser::DeSerialization *>(newElement);
			if (element != nullptr) {
				element->setFullString(this->fullString());
				if ((i = element->fromStringArray(i)) <= 0) {
					return 0;
				}
				return i;
			}
		}
	} else {
		DeSerialization element;
		element.setFullString(this->fullString());
		if ((i = element.fromStringArray(i)) <= 0) {
			return 0;
		}
		return i;
	}
	return 0;
}


size_t JsonParser::DeSerialization::addStringToArray(const size_t &currentPos)
{
	size_t i = currentPos;
	JsonString value;
	if ((i = this->getString(i, value)) <= 0) {
		return 0;
	}
	if (this->m_mappingStringArrays != nullptr) {
		this->m_mappingStringArrays->push_back(std::move(value));
	}
	return i;
}

size_t JsonParser::DeSerialization::addObjectToArray(const size_t &currentPos)
{
	size_t i = currentPos;
	if (m_mappingObjectArrays != nullptr) {
		void *newElement = this->m_mappingObjectArrays->addNew();
		if (newElement != nullptr) {
			JsonParser::DeSerialization *element =
				static_cast<JsonParser::DeSerialization *>(newElement);
			if (element != nullptr) {
				element->setFullString(this->fullString());
				if ((i = element->fromString(i)) <= 0) {
					return 0;
				}
				return i;
			}
		}
	} else {
		DeSerialization element;
		element.setFullString(this->fullString());
		if ((i = element.fromString(i)) <= 0) {
			return 0;
		}
		return i;
	}
	return 0;
}

//std::vector<std::thread> JsonParser::DeSerialization::m_workers;
std::mutex JsonParser::DeSerialization::m1;
std::mutex JsonParser::DeSerialization::m2;
std::mutex JsonParser::DeSerialization::m3;

size_t JsonParser::DeSerialization::addNumberToArray(const size_t &currentPos)
{
	const size_t tempLen = strLen();
	// iterate as long there is a number, otherwise,
	// copy the substring and create a number
	for (size_t i = currentPos; i < tempLen; ++i) {
		if (!isNumber(getChar(i))) {
			if (this->m_mappingNumberArrays != nullptr) {
				JsonParser::DeSerialization::m_workers.push_back(std::thread([=](){
					std::scoped_lock<std::mutex> lock(m1);
					JsonParser::Number number(
						std::move(substr(currentPos, i - currentPos)));
					this->m_mappingNumberArrays->push_back(std::move(number));
				}));
			}
			return i - 1;
		}
	}
	return 0;
}

size_t JsonParser::DeSerialization::addBoolToArray(const size_t &currentPos)
{
	switch (getChar(currentPos)) {
	case 't':  // adding true to the array
		if (this->m_mappingBoolArrays != nullptr) {
			JsonParser::DeSerialization::m_workers.push_back(std::thread([=]() {
				std::scoped_lock<std::mutex> lock(m2);
				this->m_mappingBoolArrays->push_back(true);
			}));
		}
		return currentPos + constLength("True") - 1;
	case 'f':  // adding false to the array
		if (this->m_mappingBoolArrays != nullptr) {
			JsonParser::DeSerialization::m_workers.push_back(std::thread([=]() {
				std::scoped_lock<std::mutex> lock(m2);
				this->m_mappingBoolArrays->push_back(false);
			}));
		}
		return currentPos + constLength("False") - 1;
	}
	return 0;
}

/*################################################################
			End Array parsing related functions
################################################################*/

// function used inside addMember() function to register the name for
// serialization, later used in toString() and toStringArray() methods
void JsonParser::DeSerialization::addSerializableMember(
	JsonString &&name, JsonTypes type, bool optional)
{
	m_serializableMembers.push_back(
		std::pair<JsonString, std::pair<JsonTypes, bool>>(
			std::move(name), std::pair<JsonTypes, bool>(type, optional))
	);
}

void JsonParser::DeSerialization::clearValues()
{
	if (this->m_mappingArrayArrays != nullptr
		&& !this->m_mappingArrayArrays->isEmpty()) {
		this->m_mappingArrayArrays->clear();
	}
	if (this->m_mappingBoolArrays != nullptr
		&& !this->m_mappingBoolArrays->isEmpty()) {
		this->m_mappingBoolArrays->clear();
	}
	if (this->m_mappingNumberArrays != nullptr
		&& !this->m_mappingNumberArrays->isEmpty()) {
		this->m_mappingNumberArrays->clear();
	}
	if (this->m_mappingObjectArrays != nullptr
		&& !this->m_mappingObjectArrays->isEmpty()) {
		this->m_mappingObjectArrays->clear();
	}
	if (this->m_mappingStringArrays != nullptr
		&& !this->m_mappingStringArrays->isEmpty()) {
		this->m_mappingStringArrays->clear();
	}
}

/*
################################################################
addMember() registers the address of the class member,
so that the variable can be later assigned during deserialization
################################################################
Begin addMember() region
################################################################
*/

void JsonParser::DeSerialization::addMember(
	JsonString &&name, __int64 & memberVariable, bool optional)
{
	this->m_kvPairMapping[name] = new JsonParser::Number(&memberVariable);
	//m_kvPairMappingNumbers[name] = JsonParser::Number(&memberVariable);
	addSerializableMember(std::move(name), JsonTypes::Number, optional);
}

void JsonParser::DeSerialization::addMember(
	JsonString &&name, double & memberVariable, bool optional)
{
	this->m_kvPairMapping[name] = new JsonParser::Number(&memberVariable);
	//m_kvPairMappingNumbers[name] = JsonParser::Number(&memberVariable);
	addSerializableMember(std::move(name), JsonTypes::Number, optional);
}

void JsonParser::DeSerialization::addMember(
	JsonString &&name, JsonString & memberVariable, bool optional)
{
	this->m_kvPairMapping[name] = &memberVariable;
	//this->m_kvPairMappingStrings[name] = &memberVariable;
	addSerializableMember(std::move(name), JsonTypes::String, optional);
}

void JsonParser::DeSerialization::addMember(
	JsonString &&name,
	JsonParser::DeSerialization & memberVariable,
	bool optional)
{
	this->m_kvPairMapping[name] = &memberVariable;
	//this->m_kvPairMappingObjects[name] = &memberVariable;
	addSerializableMember(std::move(name), JsonTypes::Object, optional);
}

void JsonParser::DeSerialization::addMember(
	JsonString &&name, bool & memberVariable, bool optional)
{
	this->m_kvPairMapping[name] = &memberVariable;
	//this->m_kvPairMappingBools[name] = &memberVariable;
	addSerializableMember(std::move(name), JsonTypes::Bool, optional);
}

void JsonParser::DeSerialization::addMember(
	JsonString &&name,
	JsonParser::Vector<JsonString>& memberVariable, bool optional)
{
	auto newObj = new JsonParser::DeSerialization();
	if (newObj != nullptr) {
		newObj->m_mappingStringArrays = &memberVariable;
		newObj->setType(JsonTypes::StringArray);
		this->m_kvPairMapping[name] = newObj;
		this->m_collectibleObjects.push_back(newObj);
		//m_kvPairMappingArrays[name] = newObj;
		addSerializableMember(std::move(name), JsonTypes::StringArray, optional);
	}
}

void JsonParser::DeSerialization::addMember(
	JsonString &&name,
	JsonParser::Vector<bool> & memberVariable, bool optional)
{
	auto newObj = new JsonParser::DeSerialization();
	if (newObj != nullptr) {
		newObj->m_mappingBoolArrays = &memberVariable;
		newObj->setType(JsonTypes::BoolArray);
		this->m_kvPairMapping[name] = newObj;
		this->m_collectibleObjects.push_back(newObj);
		//m_kvPairMappingArrays[name] =
		//	std::static_pointer_cast<JsonParser::SerializationData>(newObj);
		addSerializableMember(std::move(name), JsonTypes::BoolArray, optional);
	}
}

void JsonParser::DeSerialization::addMember(
	JsonString &&name,
	JsonParser::Vector<JsonParser::Number> &memberVariable, bool optional)
{
	auto newObj = new JsonParser::DeSerialization();
	if (newObj != nullptr) {
		newObj->m_mappingNumberArrays = &memberVariable;
		newObj->setType(JsonTypes::NumberArray);
		this->m_kvPairMapping[name] = newObj;
		this->m_collectibleObjects.push_back(newObj);
		//m_kvPairMappingArrays[name] =
			//std::static_pointer_cast<JsonParser::SerializationData>(newObj);
		addSerializableMember(
			std::move(name), JsonTypes::NumberArray, optional);
	}
}

void JsonParser::DeSerialization::addMember(
	JsonString &&name,
	JsonParser::VectorBase &memberVariable, bool optional)
{
	JsonParser::DeSerialization *newObj = new JsonParser::DeSerialization();
	if (newObj != nullptr) {
		newObj->m_mappingObjectArrays = &memberVariable;
		newObj->setType(JsonTypes::ObjectArray);
		this->m_kvPairMapping[name] = newObj;
		this->m_collectibleObjects.push_back(newObj);
		//m_kvPairMappingArrays[name] =
			//std::static_pointer_cast<JsonParser::SerializationData>(newObj);
		addSerializableMember(
			std::move(name), JsonTypes::ObjectArray, optional);
	}
}

/*################################################################
End addMember() region
################################################################*/

// unregisters all mapped member variables
void JsonParser::DeSerialization::clearMapping()
{
	/*this->m_kvPairMappingArrays.clear();
	this->m_kvPairMappingBools.clear();
	this->m_kvPairMappingNumbers.clear();
	this->m_kvPairMappingObjects.clear();
	this->m_kvPairMappingStrings.clear();*/
	this->m_kvPairMapping.clear();

	this->m_mappingArrayArrays = nullptr;
	this->m_mappingBoolArrays = nullptr;
	this->m_mappingNumberArrays = nullptr;
	this->m_mappingObjectArrays = nullptr;
	this->m_mappingStringArrays = nullptr;

	this->m_serializableMembers.clear();
}

JsonString JsonParser::DeSerialization::toString() const
{
	JsonString outputStr(JsonObjectOpenStr);
	auto endPos = this->m_serializableMembers.end();
	for (auto it = this->m_serializableMembers.begin();
		it != endPos;
		++it) {
		JsonString index = it->first;
		switch (it->second.first) {
		case JsonTypes::Number:
		{
			auto ptr = this->m_kvPairMapping.find(index);
			if (ptr != this->m_kvPairMapping.end() && ptr->second != nullptr) {
				JsonParser::Number *number = static_cast<JsonParser::Number *>(ptr->second);
				if (number != nullptr) {
					if ((!(it->second.second && number->toNumber() == 0)
						|| !it->second.second)) {
						outputStr +=
							SerializationUtils::makeKvPairStrNumber(index, *number);
					} else {
						continue;
					}
				} else {
					continue;
				}
			} else {
				continue;
			}
		}
		break;
		case JsonTypes::Bool:
		{
			auto ptr = this->m_kvPairMapping.find(index);
			if (ptr != this->m_kvPairMapping.end() && ptr->second != nullptr) {
				bool *boolean = static_cast<bool *>(ptr->second);
				if (boolean != nullptr) {
					outputStr += SerializationUtils::makeKvPairStrBool(index, *boolean);
				} else {
					continue;
				}
			} else {
				continue;
			}
		}
		break;
		case JsonTypes::String:
		{
			auto ptr = this->m_kvPairMapping.find(index);
			if (ptr != this->m_kvPairMapping.end() && ptr->second != nullptr) {
				JsonString *string = static_cast<JsonString *>(ptr->second);
				if (string != nullptr) {
					if ((!(it->second.second && string->length() <= 0)
						|| !it->second.second)) {
						outputStr += SerializationUtils::makeKvPairStrString(index, *string);
					} else {
						continue;
					}
				} else {
					continue;
				}
			} else {
				continue;
			}
		}
		break;
		case JsonTypes::Object:
		{
			auto ptr = this->m_kvPairMapping.find(index);
			if (ptr != this->m_kvPairMapping.end() && ptr->second != nullptr) {
				JsonParser::SerializationData *object = static_cast<JsonParser::SerializationData *>(ptr->second);
				if (object != nullptr) {
					auto str = SerializationUtils::makeKvPairStrObject(index, object);
					if (str == SerializationUtils::makeString(index)
						+ JsonKvSeparator + EmptyJsonObject) {
						continue;
					}
					outputStr += str;
				}
			}
		}
		break;
		case JsonTypes::ArrayArray:
		case JsonTypes::StringArray:
		case JsonTypes::ObjectArray:
		case JsonTypes::BoolArray:
		case JsonTypes::NumberArray:
		{
			auto ptr = this->m_kvPairMapping.find(index);
			if (ptr != this->m_kvPairMapping.end() && ptr->second != nullptr) {
				SerializationData *array = static_cast<SerializationData *>(ptr->second);
				if (array != nullptr) {
					outputStr +=
						SerializationUtils::makeKvPairStrArray(index, array);
				} else {
					continue;
				}
			} else {
				continue;
			}
		}
		break;
		default:
			break;
		}

		if (it + 1 != endPos) {
			outputStr += JsonEntrySeparator;
		}
	}
	if (outputStr[outputStr.length() - 1] == JsonEntrySeparator) {
		outputStr = outputStr.substr(0, outputStr.length() - 1);
	}
	outputStr += JsonObjectClose;
	return outputStr;
}

JsonString JsonParser::DeSerialization::toStringArray() const
{
	switch (type()) {
	case JsonTypes::StringArray:
		return SerializationUtils::makeStrStringArray(this->m_mappingStringArrays);
	case JsonTypes::NumberArray:
		return SerializationUtils::makeStrNumberArray(this->m_mappingNumberArrays);
	case JsonTypes::ObjectArray:
		return SerializationUtils::makeStrObjectArray(this->m_mappingObjectArrays);
	case JsonTypes::BoolArray:
		return SerializationUtils::makeStrBoolArray(this->m_mappingBoolArrays);
	case JsonTypes::ArrayArray:
		return SerializationUtils::makeStrArrayArray(this->m_mappingArrayArrays);
	}
	return EmptyJsonArray;
}
