#include "DataMappingTest.h"


DataMappingTest::DataMappingTest()
{
	addMember(Stringify(name), m_name);
	addMember(Stringify(nextLink), m_nextLink);
	addMember(Stringify(value), m_value);
	addMember(Stringify(array), m_array);
	addMember(Stringify(intVal), m_intVal);
	addMember(Stringify(testArray), m_testArray);
}


DataMappingTest::~DataMappingTest()
{
}