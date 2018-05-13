#ifndef JSONPARSER_SERIALIZATIONMAPPING_CPP_
#define JSONPARSER_SERIALIZATIONMAPPING_CPP_

#include "SerializationMapping.h"
#include "Vector.h"
#include <utility>
#include <vector>
#include <iostream>
#include <string>

JsonParser::SerializationMapping::SerializationMapping(
	JsonParser::SerializationMapping* parent) : Serializable(parent)
{
}

bool JsonParser::SerializationMapping::fromString()
{
	if (Serializable::fromString()) {
		for (auto it = this->m_kvPairNumbers.begin();
			it != this->m_kvPairNumbers.end();
			it++) {
			if (m_kvPairMappingNumbers.count(it->first) > 0) {
				(*m_kvPairMappingNumbers[it->first]) = std::move(it->second);
			}
		}
		for (auto it = this->m_kvPairBools.begin();
			it != this->m_kvPairBools.end();
			it++) {
			if (m_kvPairMappingBools.count(it->first) > 0) {
				(*m_kvPairMappingBools[it->first]) = std::move(it->second);
			}
		}
		for (auto it = this->m_kvPairStrings.begin();
			it != this->m_kvPairStrings.end();
			it++) {
			if (m_kvPairMappingStrings.count(it->first) > 0) {
				m_kvPairMappingStrings[it->first]->operator=(std::move(it->second));
			}
		}
		for (auto it = this->m_kvPairObjects.begin();
			it != this->m_kvPairObjects.end(); it++) {
			if (m_kvPairMappingObjects.count(it->first) > 0) {
				m_kvPairMappingObjects[it->first]->assign(it->second);
				m_kvPairMappingObjects[it->first]->fromString();
			}
		}
		for (auto it = this->m_kvPairArrays.begin();
			it != this->m_kvPairArrays.end();
			it++) {
			if (m_kvPairMappingArrays.count(it->first) > 0) {
				m_kvPairMappingArrays[it->first]->assign(it->second);
				m_kvPairMappingArrays[it->first]->fromStringArray();
			}
		}
		return true;
	}
	return false;
}

bool JsonParser::SerializationMapping::fromStringArray()
{
	switch (this->m_type) {
	case JsonTypes::NumberArray:
		*m_mappingNumberArrays = this->m_arrayNumbers;
		break;
	case JsonTypes::StringArray:
		*m_mappingStringArrays = this->m_arrayStrings;
		break;
	case JsonTypes::BoolArray:
		*m_mappingBoolArrays = this->m_arrayBools;
		break;
	case JsonTypes::ObjectArray:
		if (this->m_arrayObjects.size() > 0) {
			this->m_mappingObjectArrays->clear();
			for (auto it = this->m_arrayObjects.begin();
				it != this->m_arrayObjects.end();
				it++) {
				JsonParser::Serializable * currentElement = *it;
				if (currentElement != nullptr) {
					this->m_mappingObjectArrays->addNew();
					SerializationMapping *newElement = static_cast<SerializationMapping *>(
						this->m_mappingObjectArrays->lastAddedElement());
					newElement->m_parent = this;
					if (newElement != nullptr) {
						newElement->assign(currentElement);
						newElement->fromString();
					}
				}
			}
		}
		break;
	case JsonTypes::ArrayArray:
		if (this->m_arrayArrays.size() > 0) {
			this->m_mappingArrayArrays->clear();
			for (auto it = this->m_arrayArrays.begin();
				it != this->m_arrayArrays.end();
				it++) {
				JsonParser::Serializable * currentElement = *it;
				if (currentElement != nullptr) {
					this->m_mappingArrayArrays->addNew();
					SerializationMapping *newElement = static_cast<SerializationMapping *>(
						m_mappingObjectArrays->lastAddedElement());
					if (newElement != nullptr) {
						newElement->assign(currentElement);
						newElement->fromStringArray();
					}
				}
			}
		}
		break;
	default:
		break;
	}
	return false;
}

bool JsonParser::SerializationMapping::fromString(const std::string & str)
{
	this->m_fullString = new std::string(str);
	return this->fromString();
}

std::string JsonParser::SerializationMapping::toString() const
{
	std::string outputStr = "{";
	for (auto it = this->m_serializableMembers.begin();
		it != this->m_serializableMembers.end();
		it++) {
		std::pair<std::string, int> currentElement = *it;
		std::string index = currentElement.first;
		switch (it->second) {
		case JsonTypes::Number:
		{
			JsonParser::Number *ptr =
				this->m_kvPairMappingNumbers.at(index);
			if (ptr != nullptr) {
				outputStr += makeKvPairStrNumber(index, ptr);
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
			if (ptr != nullptr) {
				outputStr += makeKvPairStrString(index, ptr);
			}
		}
		break;
		case JsonTypes::Object:
		{
			JsonParser::SerializationMapping *ptr =
				this->m_kvPairMappingObjects.at(index);
			if (ptr != nullptr) {
				outputStr += makeKvPairStrObject(index, ptr);
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
	switch (this->m_type) {
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

void JsonParser::SerializationMapping::addMember(
	const std::string & name, __int64 & memberVariable)
{
	m_kvPairMappingNumbers[name] = new JsonParser::Number(&memberVariable);
	m_serializableMembers.push_back(
		std::pair<std::string, JsonTypes>(name, JsonTypes::Number)
	);
}

void JsonParser::SerializationMapping::addMember(
	const std::string & name, std::string & memberVariable)
{
	this->m_kvPairMappingStrings[name] = &memberVariable;
	m_serializableMembers.push_back(
		std::pair<std::string, JsonTypes>(name, JsonTypes::String)
	);
}

void JsonParser::SerializationMapping::addMember(
	const std::string & name, SerializationMapping& memberVariable)
{
	this->m_kvPairMappingObjects[name] = &memberVariable;
	m_serializableMembers.push_back(
		std::pair<std::string, JsonTypes>(name, JsonTypes::Object)
	);
}

void JsonParser::SerializationMapping::addMember(
	const std::string & name, bool & memberVariable)
{
	this->m_kvPairMappingBools[name] = &memberVariable;
	m_serializableMembers.push_back(
		std::pair<std::string, JsonTypes>(name, JsonTypes::Bool)
	);
}

void JsonParser::SerializationMapping::addMember(
	const std::string & name,
	JsonParser::Vector<std::string>& memberVariable)
{
	SerializationMapping *newObj = new SerializationMapping(this);
	if (newObj != nullptr) {
		newObj->m_mappingStringArrays = &memberVariable;
		m_kvPairMappingArrays[name] = newObj;
		m_serializableMembers.push_back(
			std::pair<std::string, JsonTypes>(name, JsonTypes::StringArray)
		);
	}
}

void JsonParser::SerializationMapping::addMember(
	const std::string & name,
	JsonParser::Vector<bool> & memberVariable)
{
	SerializationMapping *newObj = new SerializationMapping(this);
	if (newObj != nullptr) {
		newObj->m_mappingBoolArrays = &memberVariable;
		m_kvPairMappingArrays[name] = newObj;
		m_serializableMembers.push_back(
			std::pair<std::string, JsonTypes>(name, JsonTypes::BoolArray)
		);
	}
}

void JsonParser::SerializationMapping::addMember(
	const std::string & name,
	JsonParser::Vector<JsonParser::Number> &memberVariable)
{
	SerializationMapping *newObj = new SerializationMapping(this);
	if (newObj != nullptr) {
		newObj->m_mappingNumberArrays = &memberVariable;
		m_kvPairMappingArrays[name] = newObj;
		m_serializableMembers.push_back(
			std::pair<std::string, JsonTypes>(name, JsonTypes::NumberArray)
		);
	}
}

void JsonParser::SerializationMapping::addMember(
	const std::string & name,
	JsonParser::VectorBase *memberVariable)
{
	JsonParser::SerializationMapping *newObj =
		new JsonParser::SerializationMapping(this);
	if (newObj != nullptr) {
		newObj->m_mappingObjectArrays = memberVariable;
		m_kvPairMappingArrays[name] = newObj;
		m_serializableMembers.push_back(
			std::pair<std::string, JsonTypes>(name, JsonTypes::ObjectArray)
		);
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
	std::string outputStr = "[";
	for (auto it = elements.begin(); it != elements.end(); it++) {
		JsonParser::SerializationMapping *currentElement =
			static_cast<SerializationMapping *>(*it);
		if (currentElement != nullptr) {
			outputStr += currentElement->toString();
			if (it + 1 != elements.end()) {
				outputStr += ',';
			}
		}
	}
	outputStr += ']';
	return outputStr;
}

std::string JsonParser::SerializationMapping::makeStrNumberArray(
	const JsonParser::Vector<JsonParser::Number> * value) const
{
	if (value == nullptr) {
		return "[]";
	}
	std::string outputStr = "[";
	for (auto it = value->begin(); it != value->end(); it++) {
		JsonParser::Number *number = *it;
		if (number != nullptr) {
			outputStr += number->toString();
			if (it + 1 != value->end()) {
				outputStr += ',';
			}
		}
	}
	outputStr += ']';
	return outputStr;
}

std::string JsonParser::SerializationMapping::makeStrBoolArray(
	const JsonParser::Vector<bool> * value) const
{
	if (value == nullptr) {
		return "[]";
	}
	std::string outputStr = "[";
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
		return "[]";
	}
	std::string outputStr = "[";
	for (auto it = value->begin(); it != value->end(); it++) {
		std::string *currentElement = *it;
		if (currentElement != nullptr) {
			outputStr += makeString(*currentElement);
			if (it + 1 != value->end()) {
				outputStr += ',';
			}
		}
	}
	outputStr += ']';
	return outputStr;
}

std::string JsonParser::SerializationMapping::makeStrArrayArray(
	const JsonParser::VectorBase * value) const
{
	std::vector<void *> elements = value->getElements();
	std::string outputStr = "[";
	for (auto it = elements.begin(); it != elements.end(); it++) {
		JsonParser::SerializationMapping *currentElement =
			static_cast<SerializationMapping *>(*it);
		if (currentElement != nullptr) {
			outputStr += currentElement->toStringArray();
			if (it + 1 != elements.end()) {
				outputStr += ',';
			}
		}
	}
	outputStr += ']';
	return outputStr;
}

#endif  // JSONPARSER_SERIALIZATIONMAPPING_CPP_