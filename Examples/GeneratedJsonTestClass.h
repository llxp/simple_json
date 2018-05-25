#pragma once
#include "../simple_json/Serializable.h"

class JsonPair : public simple_json::Serializable
{
public:
	JsonPair()
	{
		ADD(first);
		ADD(last);
	}
private:
	std::string first;
	std::string last;
};

class GeneratedJsonTestClass : public simple_json::Serializable
{
public:
	GeneratedJsonTestClass()
	{
		ADD(_id);
		ADD(index);
		ADD(guid);
		ADD(isActive);
		ADD(balance);
		ADD(picture);
		ADD(age);
		ADD(eyeColor);
		ADD(name);
	}

public:
	std::string _id;
	std::string index;
	std::string guid;
	bool isActive = false;
	std::string balance;
	std::string picture;
	__int64 age = 0;
	std::string eyeColor;
	JsonPair name;
};

class GeneratedJsonTestClasses : public JsonParser::SerializationMapping
{
	public:
	GeneratedJsonTestClasses()
	{
		ADD(items);
	}

	public:
	JsonParser::Vector<GeneratedJsonTestClass> items;
};