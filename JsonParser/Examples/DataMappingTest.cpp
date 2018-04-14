//#include "stdafx.h"
#include "DataMappingTest.h"


DataMappingTest::DataMappingTest()
{
	addMember(L"name", m_name);
	addMember(L"nextLink", m_nextLink);
	addMember(L"value", m_value);
}


DataMappingTest::~DataMappingTest()
{
}
