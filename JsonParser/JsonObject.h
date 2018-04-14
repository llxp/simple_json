#pragma once
#include "Serializable.h"
#include <vector>

class JsonObject : Serializable
{
public:
	JsonObject();
	~JsonObject();

	virtual void serialize() = 0;

private:
	std::map<std::string, int> m_integers;
	std::map<std::string, long> m_longs;
	std::map<std::string, JsonObject *> m_objects;
	std::map<std::string, std::vector<JsonObject *>> m_arrays;
};