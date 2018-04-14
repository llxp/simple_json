#pragma once
#include "Serializable.h"
class SerializationMapping :
	public Serializable
{
public:
	SerializationMapping();
	~SerializationMapping();
	SerializationMapping &operator=(const SerializationMapping &other);

	void addMember(const std::string &name, size_t &memberVariable);
	bool fromString() override;

private:
	std::map<std::string, size_t *> m_kvPairMappingNumbers;
	std::map<std::string, bool *> m_kvPairMappingBools;
	std::map<std::string, std::string *> m_kvPairMappingStrings;
	std::map<std::string, SerializationMapping *> m_kvPairMappingObjects;
	std::map<std::string, SerializationMapping *> m_kvPairMappingArrays;
};

