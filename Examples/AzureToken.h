#ifndef EXAMPLES_AZURETOKEN_H_
#define EXAMPLES_AZURETOKEN_H_

#include <string>

#include "../include/simple_json/Serializable.h"

class AzureToken : public simple_json::Serializable
{
public:
	AzureToken()
	{
		serialize();
	}
private:
	void serialize() override
	{
		addMember(std::move("tokenType"), this->token_type);
		addMember(std::move("scope"), scope);
		addMember(std::move("expires_in"), expires_in);
		addMember(std::move("access_token"), access_token);
	}

public:
	std::string token_type{ "invalid" };
	std::string scope;
	std::string expires_in;
	std::string refresh_token;
	std::string access_token;
	std::string resource;
};

#endif  // EXAMPLES_AZURETOKEN_H_