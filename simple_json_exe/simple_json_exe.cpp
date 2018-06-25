/*
MIT License

Copyright (c) 2018 Lukas Lüdke

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "../Examples/DataMappingTest.h"
#include "../Examples/KeyVaultSecrets.h"
#include "../Examples/GeneratedJsonTestClass.h"
#include "../Examples/AzureToken.h"
#include "../Examples/3DCoordinates.h"
#include <fstream>
#include <streambuf>
#include <iostream>
#include <thread>
#include <mutex>
#include <direct.h>
#include <sstream>

#include <chrono>

#pragma comment(lib, "simple_json_lib.lib")

#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#include <ctime>
#endif

AzureToken func()
{
	AzureToken token = AzureToken();
	token.access_token = "123";
	return std::move(token);
}

template<typename T>
T func2()
{
	T obj;
	obj.access_token = "234";
	return obj;
}

int __cdecl main()
{
	char *buffer;
	buffer = _getcwd(NULL, 0);
	std::cout << "CWD: " << buffer << std::endl;
	free(buffer);
	KeyVaultSecrets secrets;
	/*{
		std::cout << "1...." << std::endl;
		void *newElement = secrets.m_value.addNew();
		if (newElement == nullptr) {
			std::cout << "element could not be added..." << std::endl;
		}
		else {
			KeyVaultSecret * newSecret = static_cast<KeyVaultSecret *>(newElement);
			newSecret->m_id = Stringify(abc);
			std::cout << "element added...: " << newSecret->toString() << std::endl;
			std::cout << "size: " << secrets.m_value.size() << std::endl;
			std::cout << secrets.toString() << std::endl;
		}
	}*/
	{
		std::ifstream t3;
		t3.open("secrets.txt");
		if (t3.is_open()) {
			JsonString keyVaultSecretString((std::istreambuf_iterator<char>(t3)),
				std::istreambuf_iterator<char>());
			std::cout << "size: " << keyVaultSecretString.size() << std::endl;
			std::shared_ptr<JsonString> strPtr = std::make_shared<JsonString>(
				keyVaultSecretString.begin(), keyVaultSecretString.end());
			{
				auto time1 = std::chrono::high_resolution_clock::now();
				//for (int i = 0; i < 50000; ++i) {\
					//KeyVaultSecrets secrets;
					//std::istreambuf_iterator<char> iter(t3);
					if (!secrets.fromString(&keyVaultSecretString)) {
						std::cout << "parsing failed..." << std::endl;
						t3.close();
						return 0;
					}
					//t3.close();
					//t3.open("secrets.txt", std::ios_base::binary);
					//t3.seekg(0, std::ios::beg);
				//}
					auto time2 = std::chrono::high_resolution_clock::now();
					std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count() << "ms" << std::endl;
			}
			{
				auto time1 = std::chrono::high_resolution_clock::now();
				//for (int i = 0; i < 50000; ++i) {
					secrets.toString();
				//}
					auto time2 = std::chrono::high_resolution_clock::now();
					std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count() << "ms" << std::endl;
			}
			std::cout << secrets.toString() << std::endl;
			std::cout << "2...." << std::endl;
			std::cout << sizeof(secrets) << std::endl;
			t3.close();
			/*if (keyVaultSecretString == secrets.toString()
				|| secrets.toString().length() == keyVaultSecretString.length()) {
				std::cout << "test passed." << std::endl;
				std::cout << "len1 : " << secrets.toString().length() << std::endl;
				std::cout << "len2 : " << keyVaultSecretString.length() << std::endl;
				std::cout << secrets.toString() << std::endl;
			}
			else {
				std::cout << "len1 : " << secrets.toString().length() << std::endl;
				std::cout << "len2 : " << keyVaultSecretString.length() << std::endl;
				std::cout << "secrets: " << secrets.toString() << std::endl;
				std::cout << secrets.m_value[0].m_attributes.m_enabled << std::endl;
				std::cout << std::endl;
				std::cout << std::endl;
			}*/
		}
		else {
			std::cout << "the file \"secrets.txt\" could not be opened." << std::endl;
		}
	}

	{
		std::ios_base::sync_with_stdio(false);
		std::ifstream t4;
		t4.open("abc.txt", std::ios::binary);
		if (!t4.is_open()) {
			std::cout << "the file \"abc.txt\" could not be opened." << std::endl;
			return 0;
		}
		//{
			JsonString keyVaultSecretString2((std::istreambuf_iterator<char>(t4)),
				std::istreambuf_iterator<char>());
			GeneratedJsonTestClasses test;
			std::shared_ptr<JsonString> strPtr = std::make_shared<JsonString>(
				keyVaultSecretString2.begin(), keyVaultSecretString2.end());
			std::istringstream istr(keyVaultSecretString2);

			auto time1 = std::chrono::high_resolution_clock::now();
			std::istreambuf_iterator<char> iter(istr);
			test.fromString(&keyVaultSecretString2);
			auto time2 = std::chrono::high_resolution_clock::now();
			std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count() << "ms" << std::endl;

			std::cout << test.items.size() << std::endl;
		//}
	}

	AzureToken token = func();
	AzureToken token2 = func2<AzureToken>();
	std::cout << "token1:" << token.access_token << std::endl;
	std::cout << "token2:" << token2.access_token << std::endl;
	//((JsonParser::DeSerialization *)(&token))->clearMapping();
	//((JsonParser::DeSerialization *)(&token))->serialize();
	token.refresh();
	std::cout << "token1: deserialized: " << token.toString() << std::endl;

	std::cout << sizeof(CoordinatesList) << '\n';
	CoordinatesList coordinates;
	std::cout << sizeof(coordinates) << '\n';
	std::ifstream t4;
	t4.open("1.json", std::ios::binary);
	if (!t4.is_open()) {
		std::cout << "the file \"1.json\" could not be opened." << std::endl;
	}
	else {
		JsonString jsonString((std::istreambuf_iterator<char>(t4)),
			std::istreambuf_iterator<char>());
		std::shared_ptr<JsonString> strPtr = std::make_shared<JsonString>(
			jsonString.begin(), jsonString.end());
		JsonParser::Vector<Coordinates> coords;
		{
			auto time1 = std::chrono::high_resolution_clock::now();
			if (coordinates.fromString(&jsonString)) {
				std::cout << "coordinates parsed successfully..." << std::endl;
				std::cout << coordinates.items.size() << std::endl;
			}
			else {
				std::cout << "parsing failed..." << std::endl;
			}
			auto time2 = std::chrono::high_resolution_clock::now();
			std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count() << "ms" << std::endl;
		}
		//std::cout << coordinates.toString() << std::endl;
	}
	return 0;
}