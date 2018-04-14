// JsonParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "JsonParser.h"
#include "Serializable.h"
#include "SerializationMapping.h"
#include "Examples/DataMappingTest.h"
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
	//std::ifstream t;
	//std::string str;
	//t.open("C:\\Users\\v-luldke\\Documents\\Visual Studio 2017\\Projects\\JsonParser\\x64\\Debug\\ops_secrets_json.json", std::ifstream::ios_base::in);
	//if (t.is_open()) {
		//std::string content((std::istreambuf_iterator<char>(t)),
		//	(std::istreambuf_iterator<char>()));
		//std::cout << "str: " << content << std::endl;
		//Serializable<std::string> *s = new Serializable<std::string>;
		//s->fromString(testJson);
	//}
	SerializationMapping<std::string> mappingTest;
	mappingTest.fromString(testJson);

	DataMappingTest testClass;
	testClass.fromString(L"{\"name\":\"testName\",\"nextLink\":\"https://google.de/\",\"value\":{\"str\":\"testString123\"}}");
	std::wcout << testClass.m_name << std::endl;
	std::wcout << testClass.m_nextLink << std::endl;
	std::wcout << testClass.m_value.m_str << std::endl;
	//std::string testJson = "{\"name\":\"value\",\"array\":[\"name\",\"value\"],\"objArray\":[{\"name\":\"value\"}],\"int\":2,\"bool\":false,\"obj\":{\"name\":\"value\"}}";
    return 0;
}



JsonParser::JsonParser()
{
}


JsonParser::~JsonParser()
{
}
