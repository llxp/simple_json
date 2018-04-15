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

template<typename StringType>
bool SerializationMapping<StringType>::fromString()
{
	if (Serializable<StringType>::fromString()) {
		for (typename std::map<StringType, long long>::iterator it = this->m_kvPairNumbers.begin(); it != this->m_kvPairNumbers.end(); it++) {
			if (m_kvPairMappingNumbers.count(it->first) > 0) {
				(*m_kvPairMappingNumbers[it->first]) = std::move(it->second);
			}
		}
		for (typename std::map<StringType, bool>::iterator it = this->m_kvPairBools.begin(); it != this->m_kvPairBools.end(); it++) {
			if (m_kvPairMappingBools.count(it->first) > 0) {
				(*m_kvPairMappingBools[it->first]) = std::move(it->second);
			}
		}
		for (typename std::map<StringType, StringType>::iterator it = this->m_kvPairStrings.begin(); it != this->m_kvPairStrings.end(); it++) {
			if (m_kvPairMappingStrings.count(it->first) > 0) {
				(*m_kvPairMappingStrings[it->first]) = std::move(it->second);
			}
		}
		for (typename std::map<StringType, Serializable<StringType> *>::iterator it = this->m_kvPairObjects.begin(); it != this->m_kvPairObjects.end(); it++) {
			if (m_kvPairMappingObjects.count(it->first) > 0) {
				(*m_kvPairMappingObjects[it->first]) = std::move(*it->second);
				m_kvPairMappingObjects[it->first]->fromString();
			}
		}
	}
	return false;
}

template<typename StringType>
bool SerializationMapping<StringType>::fromString(const StringType & str)
{
	this->m_fullString = new StringType(str);
	return this->fromString();
}

template<class StringType>
StringType SerializationMapping<StringType>::toString() const
{
	StringType outputStr;
	for (typename std::vector<std::pair<StringType, JsonTypes>>::const_iterator it = this->m_serializableMembers.begin(); it != this->m_serializableMembers.end(); it++) {
		std::pair<StringType, int> currentElement = *it;
		StringType index = currentElement.first;
		switch (it->second) {
		case JsonTypes::Number:
		{
			long long *ptr = this->m_kvPairMappingNumbers.at(index);
			if (ptr != nullptr) {
				long long value = *ptr;
				outputStr += makeKvPairStr(index, value);
			}
		}
		break;
		case JsonTypes::String:
		{
			StringType *ptr = this->m_kvPairMappingStrings.at(index);
			if (ptr != nullptr) {
				StringType value = *ptr;
				outputStr += makeKvPairStr(index, value);
			}
		}
		break;
		case JsonTypes::Object:
		{
			SerializationMapping *ptr = this->m_kvPairMappingObjects.at(index);
			if (ptr != nullptr) {
				SerializationMapping value = *ptr;
				outputStr += makeKvPairStr(index, value);
			}
		}
		break;
		case JsonTypes::Array:
		{
			SerializationMapping *ptr = this->m_kvPairMappingArrays.at(index);
			if (ptr != nullptr) {
				SerializationMapping value = *ptr;
				outputStr += makeKvPairStrArray(index, value);
			}
		}
		case JsonTypes::ArrayArray:
		{
			SerializationMapping *ptr = this->m_kvPairMappingArrays.at(index);
			if (ptr != nullptr) {
				SerializationMapping value = *ptr;
				outputStr += makeKvPairStrArray(index, value);
			}
		}
		break;
		}

		if (it + 1 != m_serializableMembers.end()) {
			outputStr += ",";
		}
	}
	return outputStr;
}

template<class StringType>
void SerializationMapping<StringType>::addMember(const StringType & name, long long & memberVariable)
{
	m_kvPairMappingNumbers[name] = &memberVariable;
	m_serializableMembers.push_back(std::pair<StringType, JsonTypes>(name, JsonTypes::Number));
}

template<class StringType>
void SerializationMapping<StringType>::addMember(const StringType & name, StringType & memberVariable)
{
	this->m_kvPairMappingStrings[name] = &memberVariable;
	m_serializableMembers.push_back(std::pair<StringType, JsonTypes>(name, JsonTypes::String));
}

template<class StringType>
void SerializationMapping<StringType>::addMember(const StringType & name, SerializationMapping<StringType>& memberVariable)
{
	this->m_kvPairMappingObjects[name] = &memberVariable;
	m_serializableMembers.push_back(std::pair<StringType, JsonTypes>(name, JsonTypes::Object));
}

template<class StringType>
void SerializationMapping<StringType>::addMember(const StringType & name, bool & memberVariable)
{
	this->m_kvPairMappingBools[name] = &memberVariable;
	m_serializableMembers.push_back(std::pair<StringType, JsonTypes>(name, JsonTypes::Bool));
}

template<class StringType>
void SerializationMapping<StringType>::addMember(const StringType & name, std::vector<bool>& memberVariable)
{
	SerializationMapping *newObj = new SerializationMapping(this);
	newObj->m_mappingBoolArrays = &memberVariable;
	m_kvPairMappingArrays[name] = newObj;
	m_serializableMembers.push_back(std::pair<StringType, JsonTypes>(name, JsonTypes::BoolArray));
}

template<class StringType>
void SerializationMapping<StringType>::addMember(const StringType & name, std::vector<SerializationMapping<StringType>>& memberVariable)
{
	SerializationMapping *newObj = new SerializationMapping(this);
	newObj->m_mappingObjectArrays = &memberVariable;
	m_kvPairMappingArrays[name] = newObj;
	//this->m_mappingObjectArrays[name] = &memberVariable;
	m_serializableMembers.push_back(std::pair<StringType, JsonTypes>(name, JsonTypes::ObjectArray));
}

template<class StringType>
void SerializationMapping<StringType>::addMember(const StringType & name, std::vector<long long>& memberVariable)
{
	SerializationMapping *newObj = new SerializationMapping(this);
	newObj->m_mappingNumberArrays = &memberVariable;
	m_kvPairMappingArrays[name] = newObj;
	//this->m_mappingNumberArrays[name] = &memberVariable;
	m_serializableMembers.push_back(std::pair<StringType, int>(name, JsonTypes::NumberArray));
}

template<class StringType>
void SerializationMapping<StringType>::addMember(const StringType & name, std::vector<StringType>& memberVariable)
{
	SerializationMapping *newObj = new SerializationMapping(this);
	newObj->m_mappingStringArrays = &memberVariable;
	m_kvPairMappingArrays[name] = newObj;
	//this->m_mappingStringArrays[name] = &memberVariable;
	m_serializableMembers.push_back(std::pair<StringType, JsonTypes>(name, JsonTypes::StringArray));
}

template<class StringType>
StringType SerializationMapping<StringType>::makeString(const StringType & str) const
{
	return "\"" + str + "\"";
}

template<class StringType>
StringType SerializationMapping<StringType>::makeKvPairStr(const StringType & name, long long value) const
{
	return makeString(name) + ":" + std::to_string(value);
}

template<class StringType>
StringType SerializationMapping<StringType>::makeKvPairStr(const StringType & name, const StringType & value) const
{
	return makeString(name) + ":" + makeString(value);
}

template<class StringType>
StringType SerializationMapping<StringType>::makeKvPairStr(const StringType & name, const SerializationMapping<StringType>& value) const
{
	return makeString(name) + ":" + value.toString();
}

template<class StringType>
StringType SerializationMapping<StringType>::makeKvPairStr(const StringType & name, const bool & value) const
{
	return makeString(name) + ":" + std::to_string(value);
}

template<class StringType>
StringType SerializationMapping<StringType>::makeKvPairStrArray(const StringType & name, const SerializationMapping<StringType>& value) const
{
	return StringType();
}

template<class StringType>
StringType SerializationMapping<StringType>::makeKvPairStr(const StringType &name, const std::vector<SerializationMapping<StringType>> &value) const
{
	StringType outputStr = "[";
	for (typename std::vector<SerializationMapping<StringType>>::const_iterator it = value.begin(); it != value.end(); it++) {
		outputStr += makeString(*it);
		if (it + 1 != value.end()) {
			outputStr += ",";
		}
	}
	outputStr += "]";
	return makeString(name) + ":" + outputStr;
}

template<class StringType>
StringType SerializationMapping<StringType>::makeKvPairStr(const StringType &name, const std::vector<long long> &value) const
{
	return makeString(name) + ":" + value.toString();
}

template<class StringType>
StringType SerializationMapping<StringType>::makeKvPairStr(const StringType &name, const std::vector<bool> &value) const
{
	return makeString(name) + ":" + value.toString();
}

template<class StringType>
StringType SerializationMapping<StringType>::makeKvPairStr(const StringType &name, const std::vector<StringType> &value) const
{
	return makeString(name) + ":" + value.toString();
}

template<class StringType>
StringType SerializationMapping<StringType>::makeKvPairStrArray(const StringType & name, const std::vector<SerializationMapping<StringType>>& value) const
{
	return StringType();
}
