#include "DataMappingTest.h"


DataMappingTest::DataMappingTest()
{
	addMember("name", m_name);
	addMember("nextLink", m_nextLink);
	addMember("value", m_value);
	addMember("array", m_array);
	addMember("intVal", m_intVal);
	addMember("testArray", m_testArray);//TEST if working or not!
}


DataMappingTest::~DataMappingTest()
{
}
