#pragma once
#include "SerializationMapping.h"

class KeyVaultSecret : public JsonParser::SerializationMapping
{
public:
	KeyVaultSecret()
	{
		addMember("value", m_value);
		addMember("contentType", m_contentType);
		addMember("id", m_id);
	}
	~KeyVaultSecret() {}

private:
	std::string m_value;
	std::string m_contentType;
	std::string m_id;
};



class KeyVaultSecrets : public JsonParser::SerializationMapping
{
public:
	KeyVaultSecrets();
	~KeyVaultSecrets();

private:
	JsonParser::Vector<KeyVaultSecret> m_value;
	std::string m_nextLink;
};

