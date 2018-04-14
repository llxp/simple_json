#include "stdafx.h"
#include "SerializationMapping.h"

/*template<typename StringType>
SerializationMapping<StringType> & SerializationMapping<StringType>::operator=(const Serializable & other)
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

template<typename StringType>
void SerializationMapping<StringType>::addMember(const StringType & name, size_t & memberVariable)
{
	m_kvPairMappingNumbers[name] = &memberVariable;
}

template<typename StringType>
void SerializationMapping<StringType>::addMember(const StringType & name, StringType & memberVariable)
{
	this->m_kvPairMappingStrings[name] = &memberVariable;
}

template<typename StringType>
void SerializationMapping<StringType>::addMember(const StringType & name, SerializationMapping<StringType> & memberVariable)
{
	this->m_kvPairMappingObjects[name] = &memberVariable;
}*/

/*template<typename StringType>
bool SerializationMapping<StringType>::fromString()
{
	
}*/

/*template<typename StringType>
bool SerializationMapping<StringType>::fromString(const StringType & str)
{
	
}
*/