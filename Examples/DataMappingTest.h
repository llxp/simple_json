#pragma once
#ifndef JSONPARSER_EXAMPLES_DATAMAPPINGTEST_H_
#define JSONPARSER_EXAMPLES_DATAMAPPINGTEST_H_

#include <string>
#include <vector>

#include "../include/simple_json/Serializable.h"
//#include "..\SerializationMapping.cpp"

//{JsonString testJson = "{\"test\":{\"test\":{\"test2\":\"value2\"}},\"test2\":[{\"test\":\"value\"}]}"; }

class DataMappingMemberTest : public simple_json::Serializable
{
	public:
		DataMappingMemberTest()
		{
			addMember(Stringify(str), m_str);
		}

	public:
		JsonString m_str{ EmptyString };
};

/*class DataMappingMemberTestArray : public JsonParser::Vector<JsonString, SerializationMapping<JsonString>>
{

};*/

class DataMappingTest :
	public simple_json::Serializable
{
	public:
		DataMappingTest();
		~DataMappingTest();

	public:
		JsonString m_name{ EmptyString };
		JsonString m_nextLink{ EmptyString };
		DataMappingMemberTest m_value;
		JsonParser::Vector<JsonString> m_array;
		//JsonParser::Vector<JsonString, DataMappingMemberTest *> m_objectArray;
		/*JsonParser::Vector<JsonString, DataMappingMemberTest *> m_testArray;*/
		JsonParser::Vector<DataMappingMemberTest> m_testArray;
		__int64 m_intVal = 123;
};

#endif  // JSONPARSER_EXAMPLES_DATAMAPPINGTEST_H_
