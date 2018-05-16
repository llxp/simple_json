#include "KeyVaultSecrets.h"



KeyVaultSecrets::KeyVaultSecrets()
{
	addMember("value", m_value);
	addMember("nextLink", m_nextLink);
}


KeyVaultSecrets::~KeyVaultSecrets()
{
}
