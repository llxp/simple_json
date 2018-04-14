#include "stdafx.h"
#include "SerializationMapping.h"


SerializationMapping::SerializationMapping()
{
}


SerializationMapping::~SerializationMapping()
{
}

SerializationMapping & SerializationMapping::operator=(const SerializationMapping & other)
{
	this->m_kvPairNumbers = other.m_kvPairNumbers;
	this->m_kvPairStrings = other.m_kvPairStrings;
	this->m_kvPairBools = other.m_kvPairBools;
	this->m_kvPairObjects = other.m_kvPairObjects;

	this->m_arrayObjects = other.m_arrayObjects;
	this->m_arrayArrays = other.m_arrayArrays;
	this->m_arrayNumbers = other.m_arrayNumbers;
	this->m_arrayBools = other.m_arrayBools;
	this->m_arrayStrings = other.m_arrayStrings;
	return *this;
}

void SerializationMapping::addMember(const std::string & name, size_t & memberVariable)
{
	m_kvPairMappingNumbers[name] = &memberVariable;
}

bool SerializationMapping::fromString()
{
	if (Serializable::fromString()) {
		for (std::map<std::string, size_t>::iterator it = m_kvPairNumbers.begin(); it != m_kvPairNumbers.end();it++) {
			if (m_kvPairMappingNumbers.count(it->first) > 0) {
				(*m_kvPairMappingNumbers[it->first]) = it->second;
			}
		}
		for (std::map<std::string, bool>::iterator it = m_kvPairBools.begin(); it != m_kvPairBools.end(); it++) {
			if (m_kvPairMappingBools.count(it->first) > 0) {
				(*m_kvPairMappingBools[it->first]) = it->second;
			}
		}
		for (std::map<std::string, std::string>::iterator it = m_kvPairStrings.begin(); it != m_kvPairStrings.end(); it++) {
			if (m_kvPairMappingStrings.count(it->first) > 0) {
				(*m_kvPairMappingStrings[it->first]) = it->second;
			}
		}
	}
	return false;
}
