#ifndef EXAMPLES_KEYVAULTSECRETS_H_
#define EXAMPLES_KEYVAULTSECRETS_H_
#include "../simple_json/Serializable.h"

class KeyVaultAttributes : public simple_json::Serializable
{
public:
	KeyVaultAttributes()
	{
		//addMember("enabled", m_enabled);
		//addMember("nbf", m_nbf);
		addMember("exp", m_exp);
		addMember("created", m_created);
		addMember("updated", m_updated);
		addMember("recoveryLevel", m_recoveryLevel);
	}

public:
	bool m_enabled { [this]() {addMember("enabled", m_enabled); return false;}() };
	__int64 m_nbf{ [this]() {addMember("nbf", m_nbf, true); return 0;}() };
	__int64 m_exp;
	__int64 m_created;
	__int64 m_updated;
	std::string m_recoveryLevel;
};

class KeyVaultTags : public simple_json::Serializable
{
public:
	KeyVaultTags()
	{
		addMember("r-rollover", m_rRollover, true);
		addMember("r-info", m_rInfo, true);
		addMember("r-id", m_rId, true);
		addMember("OriginalName", m_originalName, true);
	}

	std::string m_rRollover;
	std::string m_rInfo;
	std::string m_rId;
	std::string m_originalName;
};

class KeyVaultSecret : public JsonParser::SerializationMapping
{
public:
	KeyVaultSecret()
	{
		//addMember("value", m_value);
		addMember("contentType", m_contentType, true);
		addMember("id", m_id);
		addMember("attributes", m_attributes);
		addMember("tags", m_tags, true);
	}
	~KeyVaultSecret() {}

public:
	std::string m_value;
	std::string m_contentType;
	std::string m_id;
	KeyVaultAttributes m_attributes;
	KeyVaultTags m_tags;
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


#endif