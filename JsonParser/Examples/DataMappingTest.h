#pragma once
#include "..\SerializationMapping.h"
#include "..\SerializationMapping.cpp"

class DataMappingMemberTest : public SerializationMapping<std::string>
{
public:
	DataMappingMemberTest()
	{
		ADD(m_str);
	}

public:
	std::string m_str{ "" };
};

class DataMappingTest :
	public SerializationMapping<std::string>
{
public:
	DataMappingTest();
	~DataMappingTest();

public:
	std::string m_name{ "" };
	std::string m_nextLink{ "" };
	DataMappingMemberTest m_value;
	std::vector<std::string> m_array;
};

