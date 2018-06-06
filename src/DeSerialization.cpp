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

size_t lineCounter = 0;

void printDebugMessage(const std::string &function, int line)
{
	std::cout << "line: " << lineCounter << std::endl;
	std::cout << function << " : " << line << std::endl;
}
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
		size_t pos = 0;
		if (fromString(pos)) {
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

bool JsonParser::DeSerialization::fromString(size_t &currentPos)
{
	this->clearMapping();
	this->serialize();
	size_t openingCount = 0;
	size_t closeCount = 0;
	size_t tempLen = strLen();
	while(currentPos < tempLen) {
		switch (getChar(currentPos)) {
		case '\n':
			++lineCounter;
			break;
		case JsonObjectOpen:
			++openingCount;
			// Expecting position of JsonObjectClose as result
			if (!addObjectValues(currentPos)) {
				printDebugMessage(__FUNCTION__, __LINE__);
				return false;
			}
			break;
		case JsonArrayOpen:
			++openingCount;
			// Expecting position of JsonArrayClose as result
			if (!addArrayValues(currentPos)) {
				printDebugMessage(__FUNCTION__, __LINE__);
				return false;
			}
			break;
		}
		switch (getChar(currentPos)) {
		case JsonObjectClose:
			++closeCount;
			break;
		case JsonArrayClose:
			++closeCount;
		}
		if (openingCount > 0 && openingCount == closeCount) {
			return true;
		}
		++currentPos;
	}
	printDebugMessage(__FUNCTION__, __LINE__);
	return false;
}

bool JsonParser::DeSerialization::fromStringArray(size_t &currentPos)
{
	size_t openingCount = 0;
	size_t closeCount = 0;
	size_t tempLen = strLen();
	while (currentPos < tempLen) {
		switch (getChar(currentPos)) {
		case '\n':
			++lineCounter;
			break;
		case JsonArrayOpen:
			++openingCount;
			// Expecting position of JsonArrayClose as result
			if (!addArrayValues(currentPos)) {
				printDebugMessage(__FUNCTION__, __LINE__);
				return false;
			}
			break;
		}
		switch (getChar(currentPos)) {
		case JsonArrayClose:
			++closeCount;
		}
		if (openingCount == closeCount) {
			return true;
		}
		++currentPos;
	}
	printDebugMessage(__FUNCTION__, __LINE__);
	return false;
}


bool JsonParser::DeSerialization::addArrayValues(size_t &currentPos)
{
	bool commaFound = true;
	size_t tempLen = strLen();
	char c = 0;
	++currentPos;
	while(currentPos < tempLen) {
		switch ((c = getChar(currentPos))) {
		case '\n':
			++lineCounter;
			break;
		case JsonObjectOpen:
		{
			// Expecting position of JsonObjectClose as result
			if (!addObjectToArray(currentPos)
				|| !commaFound) {
				printDebugMessage(__FUNCTION__, __LINE__);
				return false;
			}
			commaFound = false;
		}
		break;
		case JsonArrayOpen:
		{
			if (!addArrayToArray(currentPos)
				|| !commaFound) {
				printDebugMessage(__FUNCTION__, __LINE__);
				return false;
			}
			commaFound = false;
		}
		break;
		case JsonStringSeparator:
		{
			if (!addStringToArray(currentPos)
				|| !commaFound) {
				printDebugMessage(__FUNCTION__, __LINE__);
				return false;
			}
			commaFound = false;
		}
		break;
		case JsonEntrySeparator:
			commaFound = true;
			break;
		case JsonArrayClose:
			if (!commaFound) {
				return true;
			}
			break;
		case 't':
		case 'T':
		case 'f':
		case 'F':
			if (!addBoolToArray(currentPos)
				|| !commaFound) {
				printDebugMessage(__FUNCTION__, __LINE__);
				return false;
			}
			commaFound = false;
		default:
			if (isNumber(c)) {
				if (!addNumberToArray(currentPos)
					|| !commaFound) {
					printDebugMessage(__FUNCTION__, __LINE__);
					return false;
				}
				commaFound = false;
			}
		}
		++currentPos;
	}
	printDebugMessage(__FUNCTION__, __LINE__);
	return false;
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


bool JsonParser::DeSerialization::addKVPair(size_t &currentPos)
{
	size_t tempLen = strLen();
	JsonString name;
	for (; currentPos < tempLen; ++currentPos) {
		switch (getChar(currentPos)) {
			case '\n':
				++lineCounter;
				continue;
			case JsonStringSeparator:
			{
				// Expecting position of closing String quotes as result
				if (!getString(currentPos, name)) {
					printDebugMessage(__FUNCTION__, __LINE__);
					return false;
				}
				if (name.length() == 0) {
					printDebugMessage(__FUNCTION__, __LINE__);
					return false;
				}
			}
			break;
			case JsonKvSeparator:
				if (!addValue(currentPos, std::move(name))) {
					printDebugMessage(__FUNCTION__, __LINE__);
					return false;
				}
				return true;
		}
	}
	printDebugMessage(__FUNCTION__, __LINE__);
	return false;
}

bool JsonParser::DeSerialization::addObjectValues(size_t &pos)
{
	size_t tempLen = strLen();
	bool commaFound = true;
	for (++pos; pos < tempLen; ++pos) {
		switch (getChar(pos)) {
		case JsonStringSeparator:
		{
			if (!addKVPair(pos) || !commaFound) {
				printDebugMessage(__FUNCTION__, __LINE__);
				return false;
			}
			commaFound = false;
		}
		break;
		case JsonEntrySeparator:
			commaFound = true;
			break;
		case JsonObjectClose:
			return true;
		}
	}
	return false;
}


inline bool JsonParser::DeSerialization::getString(
	size_t &currentPos, JsonString &name) const
{
	size_t tempLen = strLen();
	char cOld1 = 0;
	char cOld2 = 0;
	bool beginFound = false;
	for (size_t i = currentPos; i < tempLen; ++i) {
		char c = getChar(i);
		if (c == JsonStringSeparator && !isEscape(cOld1, cOld2) && !beginFound) {
			beginFound = true;
		} else if (c == JsonStringSeparator && !isEscape(cOld1, cOld2) && beginFound) {
			name = std::move(substr(currentPos + 1, i - (currentPos + 1)));
			currentPos = i;
			return true;
		}
		cOld2 = cOld1;
		cOld1 = c;
	}
	printDebugMessage(__FUNCTION__, __LINE__);
	return false;
}


bool JsonParser::DeSerialization::addValue(
	size_t &currentPos, JsonString &&name)
{
	bool commaFound = true;
	bool valueSet = false;
	const size_t tempLen = strLen();
	char c = 0;
	++currentPos;
	while(currentPos < tempLen) {
		switch ((c = getChar(currentPos))) {
			case '\n':
				++lineCounter;
				break;
			case JsonStringSeparator:
				return addStringValue(currentPos, std::move(name));
			case JsonObjectOpen:
				return addObjectValue(currentPos, std::move(name));
			case JsonArrayOpen:
				return addArrayValue(currentPos, std::move(name));
			case 'n':
			case 'N':
				return addNullValue(currentPos, std::move(name));
			case 't':
			case 'T':
			case 'f':
			case 'F':
				return addBoolValue(currentPos, std::move(name));
			default:
				if (isNumber(c)) {
					return addNumberValue(currentPos, name);
				}
			break;
		}
		++currentPos;
	}
	printDebugMessage(__FUNCTION__, __LINE__);
	return false;
}


inline bool JsonParser::DeSerialization::isNumber(char c)
{
	if ((c >= 48
		&& c <= 57)
		|| c == '.'
		|| c == '-'
		|| c == 'e') {
		return true;
	}
	return false;
}

/*################################################################
		Begin name value pair parsing related functions
################################################################*/

bool JsonParser::DeSerialization::addStringValue(
	size_t &currentPos, JsonString &&name)
{
	JsonString value;
	if (!this->getString(currentPos, value)) {
		return false;
	}
	if (this->m_kvPairMapping.find(name)
		!= this->m_kvPairMapping.end()) {
		JsonString *ptr = static_cast<JsonString *>(this->m_kvPairMapping[std::move(name)]);
		(*ptr) = std::move(value);
	}
	return true;
}


bool JsonParser::DeSerialization::addObjectValue(
	size_t &currentPos, JsonString &&name)
{
	if (this->m_kvPairMapping.find(name)
		!= this->m_kvPairMapping.end()) {
		DeSerialization *obj = static_cast<DeSerialization *>(
			this->m_kvPairMapping[std::move(name)]);
		if (obj != nullptr)
		{
			obj->clearValues();
			obj->setFullString(this->fullString());
			if (!obj->fromString(currentPos)) {
				return false;
			}
			return true;
		}
	} else {
		DeSerialization obj;
		obj.setFullString(this->fullString());
		if (!obj.fromString(currentPos)) {
			printDebugMessage(__FUNCTION__, __LINE__);
			return false;
		}
		return true;
	}
	printDebugMessage(__FUNCTION__, __LINE__);
	return false;
}


bool JsonParser::DeSerialization::addArrayValue(
	size_t &currentPos, JsonString &&name)
{
	if (this->m_kvPairMapping.find(name)
		!= this->m_kvPairMapping.end()) {
		DeSerialization *obj = static_cast<DeSerialization *>(this->m_kvPairMapping[std::move(name)]);
		if (obj != nullptr)
		{
			obj->clearValues();
			obj->setFullString(this->fullString());
			if (!obj->fromStringArray(currentPos)) {
				printDebugMessage(__FUNCTION__, __LINE__);
				return false;
			}
			return true;
		}
	} else {
		DeSerialization obj;
		obj.setFullString(this->fullString());
		if (!obj.fromStringArray(currentPos)) {
			printDebugMessage(__FUNCTION__, __LINE__);
			return false;
		}
		return true;
	}
	printDebugMessage(__FUNCTION__, __LINE__);
	return false;
}


bool JsonParser::DeSerialization::addNumberValue(
	size_t &currentPos, const JsonString &name)
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
			currentPos = i - 1;
			return true;
		}
	}
	printDebugMessage(__FUNCTION__, __LINE__);
	return false;
}


bool JsonParser::DeSerialization::addBoolValue(
	size_t &currentPos, JsonString &&name)
{
	switch (getChar(currentPos)) {
		case 't':  // assigning true to the name
		{
			auto ptr = this->m_kvPairMapping.find(name);
			if (ptr != this->m_kvPairMapping.end() && ptr->second != nullptr) {
				bool *boolean = static_cast<bool *>(ptr->second);
				(*boolean) = true;
			}
			currentPos += constLength("True") - 1;
			return true;
		}
		case 'f':  // assigning false to the name
		{
			auto ptr = this->m_kvPairMapping.find(name);
			if (ptr != this->m_kvPairMapping.end() && ptr->second != nullptr) {
				bool *boolean = static_cast<bool *>(ptr->second);
				(*boolean) = false;
			}
			currentPos += constLength("False") - 1;
			return true;
		}
	}
	printDebugMessage(__FUNCTION__, __LINE__);
	return false;
}

bool JsonParser::DeSerialization::addNullValue(size_t &pos, JsonString &&name)
{
	pos += constLength("null") - 1;
	// this->kvPairNullValues()->push_back(std::move(name));
	return true;
}

/*################################################################
		End name value pair parsing related functions
################################################################*/

/*################################################################
			Begin Array parsing related functions
################################################################*/
bool JsonParser::DeSerialization::addArrayToArray(size_t &currentPos)
{
	if (m_mappingArrayArrays != nullptr) {
		void *newElement = this->m_mappingArrayArrays->addNew();
		if (newElement != nullptr) {
			JsonParser::DeSerialization *element =
				static_cast<JsonParser::DeSerialization *>(newElement);
			if (element != nullptr) {
				element->setFullString(this->fullString());
				this->setType(JsonTypes::ArrayArray);
				return element->fromStringArray(currentPos);
			}
		}
	} else {
		DeSerialization element;
		element.setFullString(this->fullString());
		return element.fromStringArray(currentPos);
	}
	printDebugMessage(__FUNCTION__, __LINE__);
	return false;
}


bool JsonParser::DeSerialization::addStringToArray(size_t &currentPos)
{
	JsonString value;
	if (!this->getString(currentPos, value)) {
		printDebugMessage(__FUNCTION__, __LINE__);
		return false;
	}
	if (this->m_mappingStringArrays != nullptr) {
		this->m_mappingStringArrays->push_back(std::move(value));
	}
	this->setType(JsonTypes::StringArray);
	return true;
}

bool JsonParser::DeSerialization::addObjectToArray(size_t &currentPos)
{
	if (m_mappingObjectArrays != nullptr) {
		void *newElement = this->m_mappingObjectArrays->addNew();
		if (newElement != nullptr) {
			JsonParser::DeSerialization *element =
				static_cast<JsonParser::DeSerialization *>(newElement);
			if (element != nullptr) {
				element->setFullString(this->fullString());
				this->setType(JsonTypes::ObjectArray);
				return element->fromString(currentPos);
			}
		}
	} else {
		DeSerialization element;
		element.setFullString(this->fullString());
		return element.fromString(currentPos);
	}
	printDebugMessage(__FUNCTION__, __LINE__);
	return false;
}

bool JsonParser::DeSerialization::addNumberToArray(size_t &currentPos)
{
	const size_t tempLen = strLen();
	// iterate as long there is a number, otherwise,
	// copy the substring and create a number
	for (size_t i = currentPos; i < tempLen; ++i) {
		if (!isNumber(getChar(i))) {
			if (this->m_mappingNumberArrays != nullptr) {
					JsonParser::Number number(
						std::move(substr(currentPos, i - currentPos)));
					this->m_mappingNumberArrays->push_back(std::move(number));
			}
			this->setType(JsonTypes::NumberArray);
			currentPos = i - 1;
			return true;
		}
	}
	printDebugMessage(__FUNCTION__, __LINE__);
	return false;
}

bool JsonParser::DeSerialization::addBoolToArray(size_t &currentPos)
{
	switch (getChar(currentPos)) {
	case 't':  // adding true to the array
		if (this->m_mappingBoolArrays != nullptr) {
			this->m_mappingBoolArrays->push_back(true);
		}
		this->setType(JsonTypes::BoolArray);
		currentPos += constLength("True") - 1;
		return true;
	case 'f':  // adding false to the array
		if (this->m_mappingBoolArrays != nullptr) {
			this->m_mappingBoolArrays->push_back(false);
		}
		this->setType(JsonTypes::BoolArray);
		currentPos += constLength("False") - 1;
		return true;
	}
	printDebugMessage(__FUNCTION__, __LINE__);
	return false;
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
	this->m_collectibleObjects.push_back(this->m_kvPairMapping[name]);
	addSerializableMember(std::move(name), JsonTypes::Number, optional);
}

void JsonParser::DeSerialization::addMember(
	JsonString &&name, double & memberVariable, bool optional)
{
	this->m_kvPairMapping[name] = new JsonParser::Number(&memberVariable);
	this->m_collectibleObjects.push_back(this->m_kvPairMapping[name]);
	addSerializableMember(std::move(name), JsonTypes::Number, optional);
}

void JsonParser::DeSerialization::addMember(
	JsonString &&name, JsonString & memberVariable, bool optional)
{
	this->m_kvPairMapping[name] = &memberVariable;
	addSerializableMember(std::move(name), JsonTypes::String, optional);
}

void JsonParser::DeSerialization::addMember(
	JsonString &&name,
	JsonParser::DeSerialization & memberVariable,
	bool optional)
{
	this->m_kvPairMapping[name] = &memberVariable;
	addSerializableMember(std::move(name), JsonTypes::Object, optional);
}

void JsonParser::DeSerialization::addMember(
	JsonString &&name, bool & memberVariable, bool optional)
{
	this->m_kvPairMapping[name] = &memberVariable;
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
