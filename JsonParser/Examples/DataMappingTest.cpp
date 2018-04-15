//#include "stdafx.h"
#include "DataMappingTest.h"


DataMappingTest::DataMappingTest()
{
	addMember("name", m_name);
	addMember("nextLink", m_nextLink);
	addMember("value", m_value);
	addMember("array", m_array);
}


DataMappingTest::~DataMappingTest()
{
}
