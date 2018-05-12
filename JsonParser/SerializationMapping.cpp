#ifndef JSONPARSER_SERIALIZATIONMAPPING_CPP_
#define JSONPARSER_SERIALIZATIONMAPPING_CPP_

#include "SerializationMapping.h"
#include "Vector.h"
#include <utility>
//#include <vector>
#include <iostream>

JsonParser::SerializationMapping::SerializationMapping(
	JsonParser::SerializationMapping* parent) : Serializable(parent)
{
}

bool JsonParser::SerializationMapping::fromString()
{
	if (Serializable::fromString()) {
		std::cout << "parsed..." << std::endl;
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
			for (auto it = this->m_arrayObjects.begin(); it != this->m_arrayObjects.end(); it++) {
				JsonParser::Serializable * currentElement = *it;
				if (currentElement != nullptr) {
					this->m_mappingObjectArrays->addNew();
					SerializationMapping *newElement = static_cast<SerializationMapping *>(this->m_mappingObjectArrays->getLastAddedElement());
					if (newElement != nullptr) {
						//newElement->m_kvPairNumbers.clear();
						newElement->assign(currentElement);
						//*newElement = *currentElement;
						newElement->fromString();
					}
				}
				//m_mappingObjectArrays->push_back(*it);
			}
		}
		break;
	case JsonTypes::ArrayArray:
		if (this->m_arrayArrays.size() > 0) {
			for (auto it = this->m_arrayArrays.begin(); it != this->m_arrayArrays.end(); it++) {
				JsonParser::Serializable * currentElement = *it;
				if (currentElement != nullptr) {
					m_mappingArrayArrays->addNew();
					SerializationMapping *newElement = static_cast<SerializationMapping *>(m_mappingObjectArrays->getLastAddedElement());
					if (newElement != nullptr) {
						newElement->assign(currentElement);
						newElement->fromStringArray();
					}
				}
				//m_mappingArrayArrays->push_back(*it);
			}
		}
		// Would need to know the type of the array during run-/compiletime
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
			JsonParser::Number *ptr = this->m_kvPairMappingNumbers.at(index);
			if (ptr != nullptr) {
				JsonParser::Number value = *ptr;
				outputStr += makeKvPairStr(index, value);
			}
		}
		break;
		case JsonTypes::String:
		{
			std::string *ptr = this->m_kvPairMappingStrings.at(index);
			if (ptr != nullptr) {
				std::string value = *ptr;
				outputStr += makeKvPairStr(index, value);
			}
		}
		break;
		case JsonTypes::Object:
		{
			JsonParser::SerializationMapping *ptr = this->m_kvPairMappingObjects.at(index);
			if (ptr != nullptr) {
				JsonParser::SerializationMapping value = *ptr;
				outputStr += makeKvPairStr(index, value);
			}
		}
		break;
		case JsonTypes::Array:
		{
			JsonParser::SerializationMapping *ptr = this->m_kvPairMappingArrays.at(index);
			if (ptr != nullptr) {
				JsonParser::SerializationMapping value = *ptr;
				outputStr += makeKvPairStrArray(index, value);
			}
		}
		case JsonTypes::ArrayArray:
		{
			JsonParser::SerializationMapping *ptr = this->m_kvPairMappingArrays.at(index);
			if (ptr != nullptr) {
				JsonParser::SerializationMapping value = *ptr;
				outputStr += makeKvPairStrArray(index, value);
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
	const std::string & name, JsonParser::Vector<std::string>& memberVariable)
{
	SerializationMapping *newObj = new SerializationMapping(this);
	newObj->m_mappingStringArrays = &memberVariable;
	m_kvPairMappingArrays[name] = newObj;
	m_serializableMembers.push_back(
		std::pair<std::string, JsonTypes>(name, JsonTypes::StringArray)
	);
}

void JsonParser::SerializationMapping::addMember(
	const std::string & name, JsonParser::Vector<bool> & memberVariable)
{
	SerializationMapping *newObj = new SerializationMapping(this);
	newObj->m_mappingBoolArrays = &memberVariable;
	m_kvPairMappingArrays[name] = newObj;
	m_serializableMembers.push_back(
		std::pair<std::string, JsonTypes>(name, JsonTypes::BoolArray)
	);
}

void JsonParser::SerializationMapping::addMember(
	const std::string & name, JsonParser::Vector<JsonParser::Number> &memberVariable)
{
	SerializationMapping *newObj = new SerializationMapping(this);
	newObj->m_mappingNumberArrays = &memberVariable;
	m_kvPairMappingArrays[name] = newObj;
	m_serializableMembers.push_back(
		std::pair<std::string, JsonTypes>(name, JsonTypes::NumberArray)
	);
}

void JsonParser::SerializationMapping::addMember(
	const std::string & name, JsonParser::VectorBase *memberVariable)
{
	JsonParser::SerializationMapping *newObj = new JsonParser::SerializationMapping(this);
	newObj->m_mappingObjectArrays = memberVariable;
	m_kvPairMappingArrays[name] = newObj;
	m_serializableMembers.push_back(
		std::pair<std::string, JsonTypes>(name, JsonTypes::ObjectArray)
	);
}

/*void JsonParser::SerializationMapping::addMember(
	const std::string & name,
	JsonParser::Vector<bool>& memberVariable)
{
	JsonParser::SerializationMapping *newObj = new JsonParser::SerializationMapping(this);
	newObj->m_mappingBoolArrays = &memberVariable;
	m_kvPairMappingArrays[name] = newObj;
	m_serializableMembers.push_back(
		std::pair<std::string, JsonTypes>(name, JsonTypes::BoolArray)
	);
}

void JsonParser::SerializationMapping::addMember(
	const std::string & name,
	JsonParser::Vector<SerializationMapping>& memberVariable)
{
	JsonParser::SerializationMapping *newObj = new JsonParser::SerializationMapping(this);
	newObj->m_mappingObjectArrays = &memberVariable;
	m_kvPairMappingArrays[name] = newObj;
	m_serializableMembers.push_back(
		std::pair<std::string, JsonTypes>(name, JsonTypes::ObjectArray)
	);
}

void JsonParser::SerializationMapping::addMember(
	const std::string & name,
	JsonParser::Vector<JsonParser::Number>& memberVariable)
{
	JsonParser::SerializationMapping *newObj = new JsonParser::SerializationMapping(this);
	newObj->m_mappingNumberArrays = &memberVariable;
	m_kvPairMappingArrays[name] = newObj;
	m_serializableMembers.push_back(
		std::pair<std::string, int>(name, JsonTypes::NumberArray)
	);
}

void JsonParser::SerializationMapping::addMember(
	const std::string & name,
	JsonParser::VectorBase memberVariable)
{
	SerializationMapping *newObj = new SerializationMapping(this);
	newObj->m_mappingStringArrays = &memberVariable;
	m_kvPairMappingArrays[name] = newObj;
	m_serializableMembers.push_back(
		std::pair<std::string, JsonTypes>(name, JsonTypes::StringArray)
	);
}*/

std::string JsonParser::SerializationMapping::makeString(
	const std::string & str) const
{
	return '"' + str + '"';
}

std::string JsonParser::SerializationMapping::makeKvPairStr(
	const std::string & name,
	JsonParser::Number value) const
{
	return makeString(name) + ':' + value.toString();
}

std::string JsonParser::SerializationMapping::makeKvPairStr(
	const std::string & name,
	const std::string & value) const
{
	return makeString(name) + ':' + makeString(value);
}

std::string JsonParser::SerializationMapping::makeKvPairStr(
	const std::string & name,
	const JsonParser::SerializationMapping& value) const
{
	return makeString(name) + ':' + value.toString();
}

std::string JsonParser::SerializationMapping::makeKvPairStr(
	const std::string & name,
	const bool & value) const
{
	return makeString(name) + ':' + std::to_string(value);
}

std::string JsonParser::SerializationMapping::makeKvPairStrArray(
	const std::string & name,
	const JsonParser::SerializationMapping& value) const
{
	return std::string();//makeString(name) + ':' + value.toString();
}

std::string JsonParser::SerializationMapping::makeKvPairStr(
	const std::string &name,
	const std::vector<JsonParser::SerializationMapping> &value) const
{
	std::string outputStr = "[";
	for (auto it = value.begin(); it != value.end(); it++) {
		outputStr += makeString(it->toString());
		if (it + 1 != value.end()) {
			outputStr += ',';
		}
	}
	outputStr += ']';
	return makeString(name) + ':' + outputStr;
}

std::string JsonParser::SerializationMapping::makeKvPairStr(
	const std::string &name,
	const std::vector<JsonParser::Number> &value) const
{
	std::string outputStr = "[";
	for (auto it = value.begin(); it != value.end(); it++) {
		outputStr += makeString(it->toString());
		if (it + 1 != value.end()) {
			outputStr += ',';
		}
	}
	outputStr += ']';
	return makeString(name) + ':' + outputStr;
}

std::string JsonParser::SerializationMapping::makeKvPairStr(
	const std::string &name,
	const std::vector<bool> &value) const
{
	std::string outputStr = "[";
	for (auto it = value.begin(); it != value.end(); it++) {
		outputStr += makeString(std::to_string(*it));
		if (it + 1 != value.end()) {
			outputStr += ',';
		}
	}
	outputStr += ']';
	return makeString(name) + ':' + outputStr;
}

std::string JsonParser::SerializationMapping::makeKvPairStr(
	const std::string &name,
	const std::vector<std::string> &value) const
{
	std::string outputStr = "[";
	for (auto it = value.begin(); it != value.end(); it++) {
		outputStr += makeString(*it);
		if (it + 1 != value.end()) {
			outputStr += ',';
		}
	}
	outputStr += ']';
	return makeString(name) + ':' + outputStr;
}

std::string JsonParser::SerializationMapping::makeKvPairStrArray(
	const std::string & name,
	const std::vector<JsonParser::SerializationMapping>& value) const
{
	std::string outputStr = "[";
	for (auto it = value.begin(); it != value.end(); it++) {
		outputStr += makeString(it->toString());
		if (it + 1 != value.end()) {
			outputStr += ',';
		}
	}
	outputStr += ']';
	return makeString(name) + ':' + outputStr;
}

#endif  // JSONPARSER_SERIALIZATIONMAPPING_CPP_