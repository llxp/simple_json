// JsonParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "JsonParser.h"
#include "Serializable.h"
#include "SerializationMapping.h"
#include <fstream>
#include <streambuf>
#include <iostream>

int main()
{
	/*std::string testJson = "{\
		\"name\":\"value\",\
		\"array\" : [\"name\",\"value\"],\
		\"objArray\" : [{\"name\":\"value\"}],\
		\"int\" : 2,\
		\"bool\" : false,\
		\"obj\" : {\"name\":\"value\"}\
}";*/
	std::string testJson = "{\"test\":{\"test\":{\"test2\":\"value2\"}},\"test2\":[{\"test\":\"value\"}]}";
	std::ifstream t;
	std::string str;
	t.open("C:\\Users\\v-luldke\\Documents\\Visual Studio 2017\\Projects\\JsonParser\\x64\\Debug\\ops_secrets_json.json", std::ifstream::ios_base::in);
	if (t.is_open()) {
		std::string content((std::istreambuf_iterator<char>(t)),
			(std::istreambuf_iterator<char>()));
		//std::cout << "str: " << content << std::endl;
		Serializable *s = new Serializable(testJson);
		s->fromString();
	}
	SerializationMapping mappingTest;
	mappingTest.fromString();
	//std::string testJson = "{\"name\":\"value\",\"array\":[\"name\",\"value\"],\"objArray\":[{\"name\":\"value\"}],\"int\":2,\"bool\":false,\"obj\":{\"name\":\"value\"}}";
    return 0;
}



JsonParser::JsonParser()
{
}


JsonParser::~JsonParser()
{
}
