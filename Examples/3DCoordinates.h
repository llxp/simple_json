#pragma once
#include <simple_json/Serializable.h>

class Opts : public simple_json::Serializable
{
public:
	bool one;

private:
	void serialize() override
	{
		addMember("1", one);
	}
};

class Coordinates : public simple_json::Serializable
{
public:
	__int64 x;
	__int64 y;
	__int64 z;
	std::string name;
	Opts opts;

private:
	void serialize() override
	{
		addMember("x", x);
		//ADD(x);
		ADD(y);
		ADD(z);
		ADD(name);
		ADD(opts);
	}
};

class CoordinatesList : public simple_json::Serializable
{
public:
	JsonParser::Vector<Coordinates> items;

private:
	void serialize() override
	{
		ADD(items);
	}
};
