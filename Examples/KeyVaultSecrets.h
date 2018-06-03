#ifndef EXAMPLES_KEYVAULTSECRETS_H_
#define EXAMPLES_KEYVAULTSECRETS_H_
#include "../include/simple_json/Serializable.h"

class KeyVaultAttributes : public simple_json::Serializable
{
public:
	KeyVaultAttributes()
	{
		serialize();
	}
private:
	void serialize() override
	{
		addMember("enabled", m_enabled);
		addMember("nbf", m_nbf, true);
		addMember(Stringify(exp), m_exp);
		addMember(Stringify(created), m_created);
		addMember(Stringify(updated), m_updated);
		addMember(Stringify(recoveryLevel), m_recoveryLevel);
	}

public:
	bool m_enabled{ false };
	__int64 m_nbf;
	__int64 m_exp;
	__int64 m_created;
	__int64 m_updated;
	JsonString m_recoveryLevel;
};

class KeyVaultTags : public simple_json::Serializable
{
private:
	void serialize() override
	{
		addMember(Stringify(r-rollover), m_rRollover, true);
		addMember(Stringify(r-info), m_rInfo, true);
		addMember(Stringify(r-id), m_rId, true);
		addMember(Stringify(OriginalName), m_originalName, true);
	}

	JsonString m_rRollover;
	JsonString m_rInfo;
	JsonString m_rId;
	JsonString m_originalName;
};

class KeyVaultSecret : public simple_json::Serializable
{
public:
	KeyVaultSecret()
	{
		serialize();
	}
private:
	void serialize() override
	{
		//addMember("value", m_value);
		addMember(Stringify(contentType), m_contentType, true);
		addMember(Stringify(id), m_id);
		addMember(Stringify(attributes), m_attributes);
		addMember(Stringify(tags), m_tags, true);
	}

public:
	JsonString m_value;
	JsonString m_contentType;
	JsonString m_id;
	KeyVaultAttributes m_attributes;
	KeyVaultTags m_tags;
};



class KeyVaultSecrets : public simple_json::Serializable
{
public:
	KeyVaultSecrets()
	{
		serialize();
	}
private:
	void serialize() override
	{
		addMember(Stringify(value), m_value);
		addMember(Stringify(nextLink), m_nextLink);
	}

public:
	JsonParser::Vector<KeyVaultSecret> m_value;
	JsonString m_nextLink;
};


#endif