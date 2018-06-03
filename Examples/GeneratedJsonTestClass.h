#pragma once
#include "../include/simple_json/Serializable.h"

class JsonPair : public simple_json::Serializable
{
public:
	JsonPair()
	{
		serialize();
	}
private:
	JsonString first;
	JsonString last;

private:
	// Inherited via Serializable
	virtual DLLEXPORT void serialize() override;
};

class GeneratedJsonTestClass : public simple_json::Serializable
{
public:
	GeneratedJsonTestClass()
	{
		serialize();
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
	JsonParser::Vector<bool> bools;
	JsonParser::Vector<std::string> strings;

private:
	// Inherited via Serializable
	virtual DLLEXPORT void serialize() override;

};

class GeneratedJsonTestClasses : public simple_json::Serializable
{
public:
	GeneratedJsonTestClasses()
	{
		serialize();
	}
private:
	void serialize() override
	{
		ADD(items);
	}

public:
	JsonParser::Vector<GeneratedJsonTestClass> items;
};