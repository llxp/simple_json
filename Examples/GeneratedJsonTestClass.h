#pragma once
#include "../include/simple_json/Serializable.h"

class JsonPair : public simple_json::Serializable
{
public:
	JsonPair()
	{
		ADD(first);
		ADD(last);
	}
private:
	JsonString first;
	JsonString last;
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
	JsonString _id;
	JsonString index;
	JsonString guid;
	bool isActive = false;
	JsonString balance;
	JsonString picture;
	__int64 age = 0;
	JsonString eyeColor;
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