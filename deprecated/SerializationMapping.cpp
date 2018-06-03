#include "..\..\KeyVaultClientPP\KeyVaultClientPP\include\simple_json\SerializationMapping.h"
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

#include <simple_json/SerializationMapping.h>

#include <utility>
#include <vector>
#include <iostream>
#include <string>
#include <memory>
#include <sstream>

#include <simple_json\Vector.h>
#include <simple_json\SerializationUtils.h>

JsonParser::SerializationMapping::SerializationMapping()
{
	clearMapping();
	serialize();
}

bool JsonParser::SerializationMapping::fromString()
{
	if (DeSerialization::fromString()) {
		return this->assignValues();
	}
	return false;
}

bool JsonParser::SerializationMapping::fromStringArray()
{
	switch (type()) {
	case JsonTypes::NumberArray:
		m_mappingNumberArrays->assign(std::move(this->arrayNumbers()));
		return true;
	case JsonTypes::StringArray:
		m_mappingStringArrays->assign(std::move(this->arrayStrings()));
		return true;
	case JsonTypes::BoolArray:
		m_mappingBoolArrays->assign(std::move(this->arrayBools()));
		return true;
	case JsonTypes::ObjectArray:
		return fromStringToArrayOfObjects();
	case JsonTypes::ArrayArray:
		return fromStringToArrayOfArrays();
	default:
		break;
	}
	return false;
}

bool JsonParser::SerializationMapping::fromStringToArrayOfObjects()
{
	if (this->arrayObjects()->size() > 0) {
		this->m_mappingObjectArrays->clear();
		for (auto it = this->arrayObjects()->begin();
			it != this->arrayObjects()->end();
			++it) {
			if (*it != nullptr) {
				auto newElement = static_cast<SerializationMapping *>(
					this->m_mappingObjectArrays->addNew());
				if (*it != nullptr) {
					newElement->assign(std::move(*it));
					if (!newElement->assignValues()) {
						return false;
					}
				}
			}
		}
	}
	return true;
}

bool JsonParser::SerializationMapping::fromStringToArrayOfArrays()
{
	if (this->arrayArrays()->size() > 0) {
		this->m_mappingArrayArrays->clear();
		for (auto it = this->arrayArrays()->begin();
			it != this->arrayArrays()->end();
			++it) {
			if (*it != nullptr) {
				auto newElement = static_cast<SerializationMapping *>(
					this->m_mappingObjectArrays->addNew());
				if (newElement != nullptr) {
					newElement->assign(std::move(*it));
					if (!newElement->fromStringArray()) {
						return false;
					}
				}
			}
		}
	}
	return true;
}

void JsonParser::SerializationMapping::clearMapping()
{
	this->m_kvPairMappingArrays.clear();
	this->m_kvPairMappingBools.clear();
	this->m_kvPairMappingNumbers.clear();
	this->m_kvPairMappingObjects.clear();
	this->m_kvPairMappingStrings.clear();

	this->m_mappingArrayArrays = nullptr;
	this->m_mappingBoolArrays = nullptr;
	this->m_mappingNumberArrays = nullptr;
	this->m_mappingObjectArrays = nullptr;
	this->m_mappingStringArrays = nullptr;

	std::cout << "cleared mapping" << std::endl;
}

bool JsonParser::SerializationMapping::fromString(
	const std::shared_ptr<JsonString> &str)
{
	/*std::unique_ptr<std::istringstream> streamPtr = std::make_unique<std::istringstream>(*(str.get()));
	std::unique_ptr<std::istreambuf_iterator<char>> streambufPtr = std::make_unique<std::istreambuf_iterator<char>>(*streamPtr);
	this->setFullString(streambufPtr.get());*/
	// std::istringstream *streamPtr = new std::istringstream(*(str.get()));
	this->setFullString(str.get());
	// this->m_isDynamicallyCreatedStream = true;
	return this->fromString();
	// return false;
}

JsonString JsonParser::SerializationMapping::toString() const
{
	JsonString outputStr(JsonObjectOpenStr);
	for (auto it = this->m_serializableMembers.begin();
		it != this->m_serializableMembers.end();
		++it) {
		auto currentElement = *it;
		JsonString index = currentElement.first;
		switch (it->second.first) {
		case JsonTypes::Number:
		{
			auto ptr = this->m_kvPairMappingNumbers.at(index);
			if ((!(it->second.second && ptr.toNumber() == 0) || !it->second.second)) {
				outputStr += SerializationUtils::makeKvPairStrNumber(index, ptr);
			} else {
				continue;
			}
		}
		break;
		case JsonTypes::Bool:
		{
			auto ptr = this->m_kvPairMappingBools.at(index);
			if (ptr != nullptr) {
				outputStr += SerializationUtils::makeKvPairStrBool(index, ptr);
			}
		}
		break;
		case JsonTypes::String:
		{
			auto ptr = this->m_kvPairMappingStrings.at(index);
			if (ptr != nullptr &&
				(!(it->second.second && ptr->length() <= 0) || !it->second.second)) {
				outputStr += SerializationUtils::makeKvPairStrString(index, ptr);
			} else {
				continue;
			}
		}
		break;
		case JsonTypes::Object:
		{
			auto ptr = this->m_kvPairMappingObjects.at(index);
			if (ptr != nullptr) {
				auto str = SerializationUtils::makeKvPairStrObject(index, ptr);
				if (str == SerializationUtils::makeString(index)
					+ JsonKvSeparator + EmptyJsonObject) {
					continue;
				}
				outputStr += str;
			}
		}
		break;
		case JsonTypes::ArrayArray:
		case JsonTypes::StringArray:
		case JsonTypes::ObjectArray:
		case JsonTypes::BoolArray:
		case JsonTypes::NumberArray:
		{
			if (this->m_kvPairMappingArrays.at(index) != nullptr) {
				auto ptr2 = static_cast<SerializationMappingData *>(
					this->m_kvPairMappingArrays.at(index).get());
				outputStr += SerializationUtils::makeKvPairStrArray(index, ptr2);
			}
		}
		break;
		default:
			break;
		}

		if (it + 1 != m_serializableMembers.end()) {
			outputStr += JsonEntrySeparator;
		}
	}
	if (outputStr[outputStr.length() - 1] == JsonEntrySeparator) {
		outputStr = outputStr.substr(0, outputStr.length() - 1);
	}
	outputStr += JsonObjectClose;
	return outputStr;
}

JsonString JsonParser::SerializationMapping::toStringArray() const
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

bool JsonParser::SerializationMapping::assignValues()
{
	assignKvPairNumbers();
	assignKvPairBools();
	assignKvPairStrings();
	if (!assignKvPairObjects()) {
		return false;
	}
	if (!assignKvPairArrays()) {
		return false;
	}
	return true;
}

void JsonParser::SerializationMapping::addSerializableMember(
	const JsonString & name, JsonTypes type, bool optional)
{
	m_serializableMembers.push_back(
		std::pair<JsonString, std::pair<JsonTypes, bool>>(
			name, std::pair<JsonTypes, bool>(type, optional))
	);
}

/*
	################################################################
	addMember() registers the address of the class member,
	so that the variable can be later assigned during deserialization
	################################################################
	Begin addMember() region
	################################################################
*/

void JsonParser::SerializationMapping::addMember(
	const JsonString & name, __int64 & memberVariable, bool optional)
{
	m_kvPairMappingNumbers[name] = JsonParser::Number(&memberVariable);
	addSerializableMember(name, JsonTypes::Number, optional);
}

DLLEXPORT void JsonParser::SerializationMapping::addMember(
	const JsonString & name, double & memberVariable, bool optional)
{
	m_kvPairMappingNumbers[name] = JsonParser::Number(&memberVariable);
	addSerializableMember(name, JsonTypes::Number, optional);
}

void JsonParser::SerializationMapping::addMember(
	const JsonString & name, JsonString & memberVariable, bool optional)
{
	this->m_kvPairMappingStrings[name] = &memberVariable;
	addSerializableMember(name, JsonTypes::String, optional);
}

void JsonParser::SerializationMapping::addMember(
	const JsonString & name,
	SerializationMapping & memberVariable,
	bool optional)
{
	this->m_kvPairMappingObjects[name] = &memberVariable;
	addSerializableMember(name, JsonTypes::Object, optional);
}

void JsonParser::SerializationMapping::addMember(
	const JsonString & name, bool & memberVariable, bool optional)
{
	this->m_kvPairMappingBools[name] = &memberVariable;
	addSerializableMember(name, JsonTypes::Bool, optional);
}

void JsonParser::SerializationMapping::addMember(
	const JsonString & name,
	JsonParser::Vector<JsonString>& memberVariable, bool optional)
{
	auto newObj = std::make_shared<SerializationMapping>();
	if (newObj != nullptr) {
		newObj->m_mappingStringArrays = &memberVariable;
		newObj->setType(JsonTypes::StringArray);
		m_kvPairMappingArrays[name] = newObj;
		addSerializableMember(name, JsonTypes::StringArray, optional);
	}
}

void JsonParser::SerializationMapping::addMember(
	const JsonString & name,
	JsonParser::Vector<bool> & memberVariable, bool optional)
{
	auto newObj = std::make_unique<SerializationMapping>();
	if (newObj != nullptr) {
		newObj->m_mappingBoolArrays = &memberVariable;
		newObj->setType(JsonTypes::BoolArray);
		m_kvPairMappingArrays[name] =
			std::unique_ptr<SerializationMappingData>(newObj.release());
		addSerializableMember(name, JsonTypes::BoolArray, optional);
	}
}

void JsonParser::SerializationMapping::addMember(
	const JsonString & name,
	JsonParser::Vector<JsonParser::Number> &memberVariable, bool optional)
{
	auto newObj = std::make_unique<SerializationMapping>();
	if (newObj != nullptr) {
		newObj->m_mappingNumberArrays = &memberVariable;
		newObj->setType(JsonTypes::NumberArray);
		m_kvPairMappingArrays[name] =
			std::unique_ptr<SerializationMappingData>(newObj.release());
		addSerializableMember(name, JsonTypes::NumberArray, optional);
	}
}

void JsonParser::SerializationMapping::addMember(
	const JsonString & name,
	JsonParser::VectorBase &memberVariable, bool optional)
{
	auto newObj = std::make_unique<SerializationMapping>();
	if (newObj != nullptr) {
		newObj->m_mappingObjectArrays = &memberVariable;
		newObj->setType(JsonTypes::ObjectArray);
		m_kvPairMappingArrays[name] =
			std::unique_ptr<SerializationMappingData>(newObj.release());
		addSerializableMember(name, JsonTypes::ObjectArray, optional);
	}
}

/*
	################################################################
		End addMember() region
	################################################################
*/

void JsonParser::SerializationMapping::assignKvPairNumbers()
{
	for (auto it = this->kvPairNumbers()->begin();
		it != this->kvPairNumbers()->end();
		++it) {
		if (m_kvPairMappingNumbers.count(it->first) > 0) {
			m_kvPairMappingNumbers[it->first].setNumberRefValue(std::move(it->second));
		}
	}
}

void JsonParser::SerializationMapping::assignKvPairBools()
{
	for (auto it = this->kvPairBools()->begin();
		it != this->kvPairBools()->end();
		++it) {
		if (m_kvPairMappingBools.count(it->first) > 0) {
			(*m_kvPairMappingBools[it->first]) = std::move(it->second);
		}
	}
}

void JsonParser::SerializationMapping::assignKvPairStrings()
{
	for (auto it = this->kvPairStrings()->begin();
		it != this->kvPairStrings()->end();
		++it) {
		if (m_kvPairMappingStrings.count(it->first) > 0) {
			(*m_kvPairMappingStrings[it->first]) = (std::move(it->second));
		}
	}
}

bool JsonParser::SerializationMapping::assignKvPairObjects()
{
	for (auto it = this->kvPairObjects()->begin();
		it != this->kvPairObjects()->end(); ++it) {
		if (it->second != nullptr) {
			if (m_kvPairMappingObjects.count(it->first) > 0) {
				static_cast<SerializationMapping *>(
					m_kvPairMappingObjects[it->first])->assign(std::move(it->second));
				if (!static_cast<SerializationMapping *>(
					m_kvPairMappingObjects[it->first])->assignValues()) {
					return false;
				}
			}
		}
	}
	return true;
}

bool JsonParser::SerializationMapping::assignKvPairArrays()
{
	for (auto it = this->kvPairArrays()->begin();
		it != this->kvPairArrays()->end();
		++it) {
		if (it->second != nullptr) {
			if (m_kvPairMappingArrays.count(it->first) > 0) {
				static_cast<SerializationMapping *>(
					m_kvPairMappingArrays[it->first].get())->assign(std::move(it->second));
				if (!static_cast<SerializationMapping *>(
					m_kvPairMappingArrays[it->first].get())->fromStringArray()) {
					return false;
				}
			}
		}
	}
	return true;
}
