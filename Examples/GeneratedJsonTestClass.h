#pragma once
#include "../JsonParser/SerializationMapping.h"

class JsonPair : public JsonParser::SerializationMapping
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

class GeneratedJsonTestClass : public JsonParser::SerializationMapping
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