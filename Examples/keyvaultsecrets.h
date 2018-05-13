#pragma once
#include "../JsonParser/SerializationMapping.h"

class KeyVaultAttributes : public JsonParser::SerializationMapping
{
public:
	KeyVaultAttributes()
	{
		//addMember("enabled", m_enabled);
		addMember("nbf", m_nbf);
		addMember("exp", m_exp);
		addMember("created", m_created);
		addMember("updated", m_updated);
		addMember("recoveryLevel", m_recoveryLevel);
	}

public:
	bool m_enabled = DataContract<bool>("enabled", false);
	__int64 m_nbf;
	__int64 m_exp;
	__int64 m_created;
	__int64 m_updated;
	std::string m_recoveryLevel;
};

class KeyVaultTags : public JsonParser::SerializationMapping
{
public:
	KeyVaultTags()
	{
		addMember("r-rollover", m_rRollover);
	}

	std::string m_rRollover;
};

class KeyVaultSecret : public JsonParser::SerializationMapping
{
public:
	KeyVaultSecret()
	{
		//addMember("value", m_value);
		addMember("contentType", m_contentType);
		addMember("id", m_id);
		addMember("attributes", m_attributes);
	}
	~KeyVaultSecret() {}

public:
	std::string m_value;
	std::string m_contentType;
	std::string m_id;
	KeyVaultAttributes m_attributes;
};



class KeyVaultSecrets : public JsonParser::SerializationMapping
{
public:
	KeyVaultSecrets();
	~KeyVaultSecrets();

public:
	JsonParser::Vector<KeyVaultSecret> m_value;
	std::string m_nextLink;
};

