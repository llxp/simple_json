#pragma once
#ifndef JSONPARSER_EXAMPLES_DATAMAPPINGTEST_H_
#define JSONPARSER_EXAMPLES_DATAMAPPINGTEST_H_

#include <string>
#include <vector>

#include "../JsonParser/SerializationMapping.h"
//#include "..\SerializationMapping.cpp"

class DataMappingMemberTest : public JsonParser::SerializationMapping
{
	public:
		DataMappingMemberTest()
		{
			addMember("str", m_str);
		}

	public:
		std::string m_str{ "" };
};

/*class DataMappingMemberTestArray : public JsonParser::Vector<std::string, SerializationMapping<std::string>>
{

};*/

class DataMappingTest :
	public JsonParser::SerializationMapping
{
	public:
		DataMappingTest();
		~DataMappingTest();

	public:
		std::string m_name{ "" };
		std::string m_nextLink{ "" };
		DataMappingMemberTest m_value;
		JsonParser::Vector<std::string> m_array;
		//JsonParser::Vector<std::string, DataMappingMemberTest *> m_objectArray;
		/*JsonParser::Vector<std::string, DataMappingMemberTest *> m_testArray;*/
		JsonParser::Vector<DataMappingMemberTest> m_testArray;
		__int64 m_intVal = 123;
};

#endif  // JSONPARSER_EXAMPLES_DATAMAPPINGTEST_H_