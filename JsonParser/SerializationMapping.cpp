#ifndef JSONPARSER_SERIALIZATIONMAPPING_CPP_
#define JSONPARSER_SERIALIZATIONMAPPING_CPP_

#include "SerializationMapping.h"
#include "Vector.h"
#include <utility>
#include <vector>
#include <iostream>
#include <string>

JsonParser::SerializationMapping::SerializationMapping()
{
}

bool JsonParser::SerializationMapping::fromString()
{
	if (Serializable::fromString()) {
		return this->fromString2();
	}
	return false;
}

bool JsonParser::SerializationMapping::fromStringArray()
{
	switch (type()) {
	case JsonTypes::NumberArray:
		m_mappingNumberArrays->assign(this->m_arrayNumbers);
		return true;
	case JsonTypes::StringArray:
		m_mappingStringArrays->assign(this->m_arrayStrings);
		return true;
	case JsonTypes::BoolArray:
		m_mappingBoolArrays->assign(this->m_arrayBools);
		return true;
	case JsonTypes::ObjectArray:
		if (this->m_arrayObjects.size() > 0) {
			this->m_mappingObjectArrays->clear();
			for (auto it = this->m_arrayObjects.begin();
				it != this->m_arrayObjects.end();
				it++) {
				auto currentElement = *it;
				if (currentElement != nullptr) {
					this->m_mappingObjectArrays->addNew();
					SerializationMapping *newElement = static_cast<SerializationMapping *>(
						this->m_mappingObjectArrays->lastAddedElement());
					if (newElement != nullptr) {
						newElement->assign(currentElement);
						newElement->setFullString(this->fullString());
						if (!newElement->fromString2()) {
							return false;
						}
					}
				}
			}
		}
		return true;
	case JsonTypes::ArrayArray:
		if (this->m_arrayArrays.size() > 0) {
			this->m_mappingArrayArrays->clear();
			for (auto it = this->m_arrayArrays.begin();
				it != this->m_arrayArrays.end();
				it++) {
				auto currentElement = *it;
				if (currentElement != nullptr) {
					this->m_mappingArrayArrays->addNew();
					SerializationMapping *newElement = static_cast<SerializationMapping *>(
						m_mappingObjectArrays->lastAddedElement());
					if (newElement != nullptr) {
						newElement->assign(currentElement);
						newElement->setFullString(this->fullString());
						if (!newElement->fromStringArray()) {
							return false;
						}
					}
				}
			}
		}
		return true;
	default:
		break;
	}
	return false;
}

bool JsonParser::SerializationMapping::fromString(std::string * const str)
{
	this->setFullString(str);
	return this->fromString();
}

std::string JsonParser::SerializationMapping::toString() const
{
	std::string outputStr("{");
	for (auto it = this->m_serializableMembers.begin();
		it != this->m_serializableMembers.end();
		it++) {
		std::pair<std::string, std::pair<JsonTypes, bool>> currentElement = *it;
		std::string index = currentElement.first;
		switch (it->second.first) {
		case JsonTypes::Number:
		{
			JsonParser::Number *ptr =
				this->m_kvPairMappingNumbers.at(index);
			if (ptr != nullptr && (!(it->second.second && ptr->toNumber() == 0) || !it->second.second)) {
				outputStr += makeKvPairStrNumber(index, ptr);
			} else {
				continue;
			}
		}
		break;
		case JsonTypes::Bool:
		{
			bool *ptr =
				this->m_kvPairMappingBools.at(index);
			if (ptr != nullptr) {
				outputStr += makeKvPairStrBool(index, ptr);
			}
		}
		break;
		case JsonTypes::String:
		{
			std::string *ptr =
				this->m_kvPairMappingStrings.at(index);
			if (ptr != nullptr && (!(it->second.second && ptr->length() <= 0) || !it->second.second)) {
				outputStr += makeKvPairStrString(index, ptr);
			} else {
				continue;
			}
		}
		break;
		case JsonTypes::Object:
		{
			JsonParser::SerializationMapping *ptr =
				this->m_kvPairMappingObjects.at(index);
			if (ptr != nullptr) {
				std::string str = makeKvPairStrObject(index, ptr);
				if (str == makeString(index) + ':' + "{}") {
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
			JsonParser::SerializationMapping *ptr =
				this->m_kvPairMappingArrays.at(index);
			if (ptr != nullptr) {
				outputStr += makeKvPairStrArray(index, ptr);
			}
		}
		break;
		}

		if (it + 1 != m_serializableMembers.end()) {
			outputStr += ",";
		}
	}
	if (outputStr[outputStr.length() - 1] == ',') {
		outputStr = outputStr.substr(0, outputStr.length() - 1);
	}
	outputStr += '}';
	return outputStr;
}

std::string JsonParser::SerializationMapping::toStringArray() const
{
	switch (type()) {
	case JsonTypes::StringArray:
		return makeStrStringArray(this->m_mappingStringArrays);
	case JsonTypes::NumberArray:
		return makeStrNumberArray(this->m_mappingNumberArrays);
	case JsonTypes::ObjectArray:
		return makeStrObjectArray(this->m_mappingObjectArrays);
	case JsonTypes::BoolArray:
		return makeStrBoolArray(this->m_mappingBoolArrays);
	case JsonTypes::ArrayArray:
		return makeStrArrayArray(this->m_mappingArrayArrays);
	}
	return "[]";
}

bool JsonParser::SerializationMapping::fromString2()
{
	for (auto it = this->m_kvPairNumbers.begin();
		it != this->m_kvPairNumbers.end();
		it++) {
		if (m_kvPairMappingNumbers.count(it->first) > 0) {
			(*m_kvPairMappingNumbers[it->first]) = it->second;
		}
	}
	for (auto it = this->m_kvPairBools.begin();
		it != this->m_kvPairBools.end();
		it++) {
		if (m_kvPairMappingBools.count(it->first) > 0) {
			(*m_kvPairMappingBools[it->first]) = it->second;
		}
	}
	for (auto it = this->m_kvPairStrings.begin();
		it != this->m_kvPairStrings.end();
		it++) {
		if (m_kvPairMappingStrings.count(it->first) > 0) {
			m_kvPairMappingStrings[it->first]->operator=(it->second);
		}
	}
	for (auto it = this->m_kvPairObjects.begin();
		it != this->m_kvPairObjects.end(); it++) {
		auto currentElement = it->second;
		if (currentElement != nullptr) {
			if (m_kvPairMappingObjects.count(it->first) > 0) {
				m_kvPairMappingObjects[it->first]->assign(currentElement);
				if (!m_kvPairMappingObjects[it->first]->fromString2()) {
					return false;
				}
			}
		}
	}
	for (auto it = this->m_kvPairArrays.begin();
		it != this->m_kvPairArrays.end();
		it++) {
		auto currentElement = it->second;
		if (currentElement != nullptr) {
			if (m_kvPairMappingArrays.count(it->first) > 0) {
				m_kvPairMappingArrays[it->first]->assign(currentElement);
				if (!m_kvPairMappingArrays[it->first]->fromStringArray()) {
					return false;
				}
			}
		}
	}
	return true;
}

void JsonParser::SerializationMapping::addSerializableMember(const std::string & name, JsonTypes type, bool optional)
{
	m_serializableMembers.push_back(
		std::pair<std::string, std::pair<JsonTypes, bool>>(name, std::pair<JsonTypes, bool>(type, optional))
	);
}

void JsonParser::SerializationMapping::addMember(
	const std::string & name, __int64 & memberVariable, bool optional)
{
	m_kvPairMappingNumbers[name] = new JsonParser::Number(&memberVariable);
	addSerializableMember(name, JsonTypes::Number, optional);
}

void JsonParser::SerializationMapping::addMember(
	const std::string & name, std::string & memberVariable, bool optional)
{
	this->m_kvPairMappingStrings[name] = &memberVariable;
	addSerializableMember(name, JsonTypes::String, optional);
}

void JsonParser::SerializationMapping::addMember(
	const std::string & name, SerializationMapping& memberVariable, bool optional)
{
	this->m_kvPairMappingObjects[name] = &memberVariable;
	addSerializableMember(name, JsonTypes::Object, optional);
}

void JsonParser::SerializationMapping::addMember(
	const std::string & name, bool & memberVariable, bool optional)
{
	this->m_kvPairMappingBools[name] = &memberVariable;
	addSerializableMember(name, JsonTypes::Bool, optional);
}

void JsonParser::SerializationMapping::addMember(
	const std::string & name,
	JsonParser::Vector<std::string>& memberVariable, bool optional)
{
	SerializationMapping *newObj = new SerializationMapping();
	if (newObj != nullptr) {
		newObj->m_mappingStringArrays = &memberVariable;
		m_kvPairMappingArrays[name] = newObj;
		addSerializableMember(name, JsonTypes::StringArray, optional);
	}
}

void JsonParser::SerializationMapping::addMember(
	const std::string & name,
	JsonParser::Vector<bool> & memberVariable, bool optional)
{
	SerializationMapping *newObj = new SerializationMapping();
	if (newObj != nullptr) {
		newObj->m_mappingBoolArrays = &memberVariable;
		m_kvPairMappingArrays[name] = newObj;
		addSerializableMember(name, JsonTypes::BoolArray, optional);
	}
}

void JsonParser::SerializationMapping::addMember(
	const std::string & name,
	JsonParser::Vector<JsonParser::Number> &memberVariable, bool optional)
{
	SerializationMapping *newObj = new SerializationMapping();
	if (newObj != nullptr) {
		newObj->m_mappingNumberArrays = &memberVariable;
		m_kvPairMappingArrays[name] = newObj;
		addSerializableMember(name, JsonTypes::NumberArray, optional);
	}
}

void JsonParser::SerializationMapping::addMember(
	const std::string & name,
	JsonParser::VectorBase *memberVariable, bool optional)
{
	SerializationMapping *newObj = new SerializationMapping();
	if (newObj != nullptr) {
		newObj->m_mappingObjectArrays = memberVariable;
		m_kvPairMappingArrays[name] = newObj;
		addSerializableMember(name, JsonTypes::ObjectArray, optional);
	}
}

std::string JsonParser::SerializationMapping::makeString(
	const std::string & str) const
{
	return '"' + str + '"';
}

std::string JsonParser::SerializationMapping::makeKvPairStrNumber(
	const std::string & name,
	JsonParser::Number *value) const
{
	return makeString(name) + ':' + value->toString();
}

std::string JsonParser::SerializationMapping::makeKvPairStrString(
	const std::string & name,
	std::string *value) const
{
	return makeString(name) + ':' + makeString(*value);
}

std::string JsonParser::SerializationMapping::makeKvPairStrObject(
	const std::string & name,
	JsonParser::SerializationMapping *value) const
{
	return makeString(name) + ':' + value->toString();
}

std::string JsonParser::SerializationMapping::makeKvPairStrBool(
	const std::string & name,
	bool value) const
{
	return makeString(name) + ':' + (value ? "true" : "false");
}

std::string JsonParser::SerializationMapping::makeKvPairStrArray(
	const std::string & name,
	JsonParser::SerializationMapping *value) const
{
	return makeString(name) + ':' + value->toStringArray();
}

std::string JsonParser::SerializationMapping::makeStrObjectArray(
	const JsonParser::VectorBase * value) const
{
	std::vector<void *> elements = value->getElements();
	return makeStr3<SerializationMapping, std::vector<void *>>(
		&elements, [=](SerializationMapping * currentElement)->std::string{
		return currentElement->toString();
	});
}

std::string JsonParser::SerializationMapping::makeStrNumberArray(
	const JsonParser::Vector<JsonParser::Number> * value) const
{
	if (value == nullptr) {
		return std::string("[]");
	}
	return makeStr3<JsonParser::Number, JsonParser::Vector<JsonParser::Number>>(
		value, [=](JsonParser::Number * currentElement)->std::string {
		return currentElement->toString();
	});
}

std::string JsonParser::SerializationMapping::makeStrBoolArray(
	const JsonParser::Vector<bool> * value) const
{
	if (value == nullptr) {
		return std::string("[]");
	}
	std::string outputStr("[");
	for (auto it = value->begin(); it != value->end(); it++) {
		bool *currentElement = *it;
		if (currentElement != nullptr) {
			outputStr += std::to_string(*currentElement);
			if (it + 1 != value->end()) {
				outputStr += ',';
			}
		}
	}
	outputStr += ']';
	return outputStr;
}

std::string JsonParser::SerializationMapping::makeStrStringArray(
	const JsonParser::Vector<std::string> * value) const
{
	if (value == nullptr) {
		return std::string("[]");
	}
	return makeStr3<std::string, JsonParser::Vector<std::string>>
		(value, [=](std::string *currentElement)->std::string {
		return makeString(*currentElement);
	});
}

std::string JsonParser::SerializationMapping::makeStrArrayArray(
	const JsonParser::VectorBase * value) const
{
	std::vector<void *> elements = value->getElements();
	return makeStr3<SerializationMapping, std::vector<void *>>
		(&elements, [=](SerializationMapping * currentElement)->std::string {
		return currentElement->toStringArray();
	});
}

#endif  // JSONPARSER_SERIALIZATIONMAPPING_CPP_