// JsonParser.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include "../JsonParser/JsonParser.h"
#include "Serializable.h"
#include "SerializationMapping.h"
#include "../Examples/DataMappingTest.h"
#include "../Examples/KeyVaultSecrets.h"
#include <fstream>
#include <streambuf>
#include <iostream>
#include <thread>
#include <mutex>

#include <boost/chrono.hpp>
#include <boost/timer/timer.hpp>

#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#include <ctime>
#endif

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

__int64 GetTimeMs64()
{
#ifdef _WIN32
	/* Windows */
	FILETIME ft;
	LARGE_INTEGER li;

	/* Get the amount of 100 nano seconds intervals elapsed since January 1, 1601 (UTC) and copy it
	* to a LARGE_INTEGER structure. */
	GetSystemTimeAsFileTime(&ft);
	li.LowPart = ft.dwLowDateTime;
	li.HighPart = ft.dwHighDateTime;

	__int64 ret = li.QuadPart;
	ret -= 116444736000000000LL; /* Convert from file time to UNIX epoch time. */
	ret /= 10000; /* From 100 nano seconds (10^-7) to 1 millisecond (10^-3) intervals */

	return ret;
#else
	/* Linux */
	struct timeval tv;

	gettimeofday(&tv, NULL);

	uint64 ret = tv.tv_usec;
	/* Convert from micro seconds (10^-6) to milliseconds (10^-3) */
	ret /= 1000;

	/* Adds the seconds (10^0) after converting them to milliseconds (10^-3) */
	ret += (tv.tv_sec * 1000);

	return ret;
#endif
}

int __fastcall main()
{
	std::string str = "Test";
	//int x = 5;
	//std::thread t{ func, str, std::ref(x) };
	//do_something();
	//t.join();
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

	std::string jsonString = "{\"name\":\"testName\",\"nextLink\":\"https://google.de/\",\"value\":{\"str\":\"testString123\"},\"array\":[\"abc\"],\"intVal\":1,\"testArray\":[{\"str\":\"testStr\"}]}";
	DataMappingTest testClass;
	//__int64 beginTime1 = GetTimeMs64();
	{
		boost::timer::auto_cpu_timer t2(5, "%w seconds\n");
		testClass.fromString(&jsonString);
		//((JsonParser::SerializableData *)&testClass)->setFullString(&jsonString);
		//for (int i = 0; i < 500; i++) {
			//testClass.fromString();
		//}
	//__int64 endTime1 = GetTimeMs64();
	}

	JsonParser::Serializable *obj = new JsonParser::Serializable();
	//obj->fromString(jsonString);
	//obj->setFullString(&jsonString);
	{
		boost::timer::auto_cpu_timer t2(5, "%w seconds\n");
		//for (int i = 0; i < 9000000; i++) {
			//obj->fromString(&jsonString);
			//Sleep(100);
		//}
	}
	//return 0;
	//std::cout << "fromString() needed " << endTime1 - beginTime1 << " milliseconds" << std::endl;
	std::cout << "jsonString = " << std::endl << jsonString << std::endl;
	std::cout << "m_name = " << testClass.m_name << std::endl;
	std::cout << "m_nextLink = " << testClass.m_nextLink << std::endl;
	std::cout << "m_value.m_str = " << testClass.m_value.m_str << std::endl;
	std::cout << "m_array = " << testClass.m_array.size() << std::endl;
	std::cout << "m_testArray = " << testClass.m_testArray.size() << std::endl;

	std::cout << "toString(): " << std::endl << testClass.toString() << std::endl;

	//__int64 beginTime2 = GetTimeMs64();
	{
		boost::timer::auto_cpu_timer t2(5, "%w seconds\n");
		std::string serialized = testClass.toString();
		for (int i = 0; i < 50000; i++) {
			serialized = testClass.toString();
		}
		if (serialized == jsonString) {
			std::cout << "class serialization test passed." << std::endl;
		}
	}
	std::ifstream t3;
	t3.open("secrets.txt");
	if (!t3.is_open()) {
		std::cout << "the file \"secrets.txt\" could not be opened." << std::endl;
		return 0;
	}
	std::string str2((std::istreambuf_iterator<char>(t3)),
		std::istreambuf_iterator<char>());
	std::string keyVaultSecretString = str2;
	KeyVaultSecrets secrets;
	secrets.fromString(&keyVaultSecretString);
	{
		boost::timer::auto_cpu_timer t2(5, "%w seconds\n");
		for (int i = 0; i < 50000; i++) {
			secrets.fromString(&keyVaultSecretString);
		}
	}
	std::string serializedKeyVaultSecretString = secrets.toString();
	{
		boost::timer::auto_cpu_timer t2(5, "%w seconds\n");
		for (int i = 0; i < 500; i++) {
			serializedKeyVaultSecretString = secrets.toString();
		}
	}
	if (keyVaultSecretString == serializedKeyVaultSecretString || secrets.toString().length() == keyVaultSecretString.length()) {
		std::cout << "test passed." << std::endl;
		std::cout << "len1 : " << serializedKeyVaultSecretString.length() << std::endl;
		std::cout << "len2 : " << keyVaultSecretString.length() << std::endl;
		std::cout << secrets.toString() << std::endl;
	}
	else {
		std::cout << "len1 : " << serializedKeyVaultSecretString.length() << std::endl;
		std::cout << "len2 : " << keyVaultSecretString.length() << std::endl;
		std::cout << secrets.toString() << std::endl;
		std::cout << secrets.m_value[0].m_attributes.m_enabled << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		//std::cout << keyVaultSecretString << std::endl;
	}
	//__int64 endTime2 = GetTimeMs64();
	//std::cout << "toString() needed " << endTime2 - beginTime2 << " milliseconds" << std::endl;
	//std::string testJson = "{\"name\":\"value\",\"array\":[\"name\",\"value\"],\"objArray\":[{\"name\":\"value\"}],\"int\":2,\"bool\":false,\"obj\":{\"name\":\"value\"}}";
	return 0;
}

