// JsonParser.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
//#include "JsonParser/JsonParser.h"
#include "../include/simple_json/DeSerialization.h"
#include "../include/simple_json/SerializationMapping.h"
#include "../Examples/DataMappingTest.h"
#include "../Examples/KeyVaultSecrets.h"
#include "../Examples/GeneratedJsonTestClass.h"
#include <fstream>
#include <streambuf>
#include <iostream>
#include <thread>
#include <mutex>

#include <boost/chrono.hpp>
#include <boost/timer/timer.hpp>

#pragma comment(lib, "simple_json_dll.lib")

#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#include <ctime>
#endif

int __cdecl main()
{
	KeyVaultSecrets secrets;
	{
		std::cout << "1...." << std::endl;
		void *newElement = secrets.m_value.addNew();
		if (newElement == nullptr) {
			std::cout << "element could not be added..." << std::endl;
		}
		else {
			KeyVaultSecret * newSecret = static_cast<KeyVaultSecret *>(newElement);
			newSecret->m_id = "abc";
			std::cout << "element added...: " << newSecret->toString() << std::endl;
			std::cout << "size: " << secrets.m_value.size() << std::endl;
			std::cout << secrets.toString() << std::endl;
		}
	}
	{
		std::ifstream t3;
		t3.open("secrets.txt");
		if (t3.is_open()) {
			std::string keyVaultSecretString((std::istreambuf_iterator<char>(t3)),
				std::istreambuf_iterator<char>());
			std::cout << "size: " << keyVaultSecretString.size() << std::endl;
			std::shared_ptr<std::string> strPtr = std::make_shared<std::string>(
				keyVaultSecretString.begin(), keyVaultSecretString.end());
			{
				boost::timer::auto_cpu_timer t2(5, "%w seconds\n");
				for (int i = 0; i < 50000; ++i) {
					if (!secrets.fromString(strPtr)) {
						std::cout << "parsing failed..." << std::endl;
						return 0;
					}
				}
			}
			{
				boost::timer::auto_cpu_timer t2(5, "%w seconds\n");
				for (int i = 0; i < 50000; ++i) {
					secrets.toString();
				}
			}
			std::cout << "2...." << std::endl;
			std::cout << sizeof(secrets) << std::endl;
			t3.close();
			//}
			if (keyVaultSecretString == secrets.toString()
				|| secrets.toString().length() == keyVaultSecretString.length()) {
				std::cout << "test passed." << std::endl;
				std::cout << "len1 : " << secrets.toString().length() << std::endl;
				std::cout << "len2 : " << keyVaultSecretString.length() << std::endl;
				std::cout << secrets.toString() << std::endl;
			}
			else {
				std::cout << "len1 : " << secrets.toString().length() << std::endl;
				std::cout << "len2 : " << keyVaultSecretString.length() << std::endl;
				std::cout << secrets.toString() << std::endl;
				std::cout << secrets.m_value[0].m_attributes.m_enabled << std::endl;
				std::cout << std::endl;
				std::cout << std::endl;
			}
		}
		else {
			std::cout << "the file \"secrets.txt\" could not be opened." << std::endl;
		}
	}

	{
		std::ifstream t4;
		t4.open("abc.txt");
		if (!t4.is_open()) {
			std::cout << "the file \"secrets.txt\" could not be opened." << std::endl;
			return 0;
		}
		std::string keyVaultSecretString2((std::istreambuf_iterator<char>(t4)),
			std::istreambuf_iterator<char>());
		GeneratedJsonTestClasses test;
		std::shared_ptr<std::string> strPtr = std::make_shared<std::string>(
			keyVaultSecretString2.begin(), keyVaultSecretString2.end());
		{
			boost::timer::auto_cpu_timer t2(5, "%w seconds\n");
			test.fromString(strPtr);
		}
		std::cout << test.items.size() << std::endl;
	}
	return 0;
}