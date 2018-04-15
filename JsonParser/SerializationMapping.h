#pragma once
#include "Serializable.h"
#include "Serializable.cpp"

#define ADD(MEMBER) addMember(#MEMBER, MEMBER);

template<class StringType>
class SerializationMapping :
	private Serializable<StringType>
{
public:
	//#define StringType std::string
	SerializationMapping<StringType>(SerializationMapping<StringType> *parent = nullptr) : Serializable<StringType>(parent) {}
	~SerializationMapping<StringType>() {}

private:
	std::map<StringType, long long *> m_kvPairMappingNumbers;
	std::map<StringType, bool *> m_kvPairMappingBools;
	std::map<StringType, StringType *> m_kvPairMappingStrings;
	std::map<StringType, SerializationMapping<StringType> *> m_kvPairMappingObjects;
	std::map<StringType, SerializationMapping<StringType> *> m_kvPairMappingArrays;

	std::vector<StringType> *m_mappingStringArrays{ nullptr };
	std::vector<bool> *m_mappingBoolArrays{ nullptr };
	std::vector<long long> *m_mappingNumberArrays{ nullptr };
	std::vector<SerializationMapping<StringType>> *m_mappingObjectArrays{ nullptr };
	std::vector<SerializationMapping<StringType>> *m_mappingArrayArrays{ nullptr };

	//std::map<StringType, std::vector<SerializationMapping<StringType>> *> m_kvPairMappingObjectArrays;

	std::vector<std::pair<StringType, JsonTypes>> m_serializableMembers;

public:

	virtual bool fromString() override;
	bool fromString(const StringType &str);

	virtual StringType toString() const;

protected:
	void addMember(const StringType &name, long long &memberVariable);
	void addMember(const StringType &name, StringType &memberVariable);
	void addMember(const StringType &name, SerializationMapping<StringType> &memberVariable);
	void addMember(const StringType &name, bool &memberVariable);

	void addMember(const StringType &name, std::vector<StringType> &memberVariable);
	void addMember(const StringType &name, std::vector<long long> &memberVariable);
	void addMember(const StringType &name, std::vector<bool> &memberVariable);
	void addMember(const StringType &name, std::vector<SerializationMapping<StringType>> &memberVariable);

private:
	StringType makeString(const StringType &str) const;
	StringType makeKvPairStr(const StringType &name, long long value) const;
	StringType makeKvPairStr(const StringType &name, const StringType &value) const;
	StringType makeKvPairStr(const StringType &name, const SerializationMapping<StringType> &value) const;
	StringType makeKvPairStr(const StringType &name, const bool &value) const;
	StringType makeKvPairStrArray(const StringType &name, const SerializationMapping<StringType> &value) const;

	StringType makeKvPairStr(const StringType &name, const std::vector<SerializationMapping<StringType>> &value) const;
	StringType makeKvPairStr(const StringType &name, const std::vector<long long> &value) const;
	StringType makeKvPairStr(const StringType &name, const std::vector<bool> &value) const;
	StringType makeKvPairStr(const StringType &name, const std::vector<StringType> &value) const;
	StringType makeKvPairStrArray(const StringType &name, const std::vector<SerializationMapping<StringType>> &value) const;

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