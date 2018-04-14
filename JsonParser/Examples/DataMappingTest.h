#pragma once
#include "..\SerializationMapping.h"

class DataMappingMemberTest : public SerializationMapping<std::wstring>
{
public:
	DataMappingMemberTest()
	{
		addMember(L"str", m_str);
	}

public:
	std::wstring m_str{ L"" };
};

class DataMappingTest :
	public SerializationMapping<std::wstring>
{
public:
	DataMappingTest();
	~DataMappingTest();

public:
	std::wstring m_name{ L"" };
	std::wstring m_nextLink{ L"" };
	DataMappingMemberTest m_value;
};

