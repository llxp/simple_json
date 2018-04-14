#pragma once
#include "Serializable.h"
#include "Serializable.cpp"

template<class StringType>
class SerializationMapping :
	private Serializable<StringType>
{
public:
	//#define StringType std::string
	SerializationMapping<StringType>() {}
	~SerializationMapping<StringType>() {}

private:
	std::map<StringType, size_t *> m_kvPairMappingNumbers;
	std::map<StringType, bool *> m_kvPairMappingBools;
	std::map<StringType, StringType *> m_kvPairMappingStrings;
	std::map<StringType, SerializationMapping<StringType> *> m_kvPairMappingObjects;
	std::map<StringType, SerializationMapping<StringType> *> m_kvPairMappingArrays;

public:

	bool fromString() override
	{
		if (Serializable<StringType>::fromString()) {
			for (std::map<StringType, size_t>::iterator it = m_kvPairNumbers.begin(); it != m_kvPairNumbers.end(); it++) {
				if (m_kvPairMappingNumbers.count(it->first) > 0) {
					(*m_kvPairMappingNumbers[it->first]) = std::move(it->second);
				}
			}
			for (std::map<StringType, bool>::iterator it = m_kvPairBools.begin(); it != m_kvPairBools.end(); it++) {
				if (m_kvPairMappingBools.count(it->first) > 0) {
					(*m_kvPairMappingBools[it->first]) = std::move(it->second);
				}
			}
			for (std::map<StringType, StringType>::iterator it = m_kvPairStrings.begin(); it != m_kvPairStrings.end(); it++) {
				if (m_kvPairMappingStrings.count(it->first) > 0) {
					(*m_kvPairMappingStrings[it->first]) = std::move(it->second);
				}
			}
			for (std::map<StringType, Serializable<StringType> *>::iterator it = m_kvPairObjects.begin(); it != m_kvPairObjects.end(); it++) {
				if (m_kvPairMappingObjects.count(it->first) > 0) {
					(*m_kvPairMappingObjects[it->first]) = std::move(*it->second);
					m_kvPairMappingObjects[it->first]->fromString();
				}
			}
		}
		return false;
	}
	bool fromString(const StringType &str)
	{
		this->m_fullString = new StringType(str);
		return this->fromString();
	}

	virtual StringType toString() const;

protected:
	void addMember(const StringType &name, size_t &memberVariable)
	{
		m_kvPairMappingNumbers[name] = &memberVariable;
	}
	void addMember(const StringType &name, StringType &memberVariable)
	{
		this->m_kvPairMappingStrings[name] = &memberVariable;
	}
	void addMember(const StringType &name, SerializationMapping<StringType> &memberVariable)
	{
		this->m_kvPairMappingObjects[name] = &memberVariable;
	}

private:
	SerializationMapping<StringType> &operator=(const Serializable<StringType> &other)
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
};

template<class StringType>
inline StringType SerializationMapping<StringType>::toString() const
{
	return StringType();
}
