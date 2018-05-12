// JsonParser.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include "../JsonParser/JsonParser.h"
#include "Serializable.h"
#include "SerializationMapping.h"
#include "Examples/DataMappingTest.h"
#include <fstream>
#include <streambuf>
#include <iostream>
#include <thread>
#include <mutex>

std::mutex m;

void func(std::string str, int& x)
{
	std::lock_guard<std::mutex> lock(m);
	std::cout << __FUNCTION__ << " " << str << std::to_string(x) << std::endl;
}
void do_something()
{
	std::lock_guard<std::mutex> lock(m);
	std::cout << __FUNCTION__ << std::endl;
}

int __fastcall main()
{
	std::string str = "Test";
	int x = 5;
	std::thread t{ func, str, std::ref(x) };
	do_something();
	t.join();
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
	/*SerializationMapping<std::string> mappingTest;
	mappingTest.fromString(testJson);*/

	DataMappingTest testClass;
	testClass.fromString("{\"name\":\"testName\",\"nextLink\":\"https://google.de/\",\"value\":{\"str\":\"testString123\"},\"array\":[\"abc\"], \"testArray\":[{\"str\":\"testStr\"}]}");
	std::cout << "m_name = " << testClass.m_name << std::endl;
	std::cout << "m_nextLink = " << testClass.m_nextLink << std::endl;
	std::cout << "m_value.m_str = " << testClass.m_value.m_str << std::endl;
	std::cout << "m_array = " << testClass.m_array.size() << std::endl;
	std::cout << "m_testArray = " << testClass.m_testArray.size() << std::endl;

	std::cout << "toString(): " << testClass.toString() << std::endl;
	//std::string testJson = "{\"name\":\"value\",\"array\":[\"name\",\"value\"],\"objArray\":[{\"name\":\"value\"}],\"int\":2,\"bool\":false,\"obj\":{\"name\":\"value\"}}";
	return 0;
}

