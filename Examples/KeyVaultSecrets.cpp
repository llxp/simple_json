#include "KeyVaultSecrets.h"



KeyVaultSecrets::KeyVaultSecrets()
{
	addMember(Stringify(value), m_value);
	addMember(Stringify(nextLink), m_nextLink);
}


KeyVaultSecrets::~KeyVaultSecrets()
{
}
