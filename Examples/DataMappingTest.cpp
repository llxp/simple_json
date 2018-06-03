#include "DataMappingTest.h"


DataMappingTest::DataMappingTest()
{
	addMember(std::move(Stringify(name)), m_name);
	addMember(std::move(Stringify(nextLink)), m_nextLink);
	addMember(std::move(Stringify(value)), m_value);
	addMember(std::move(Stringify(array)), m_array);
	addMember(std::move(Stringify(intVal)), m_intVal);
	addMember(std::move(Stringify(testArray)), m_testArray);
}


DataMappingTest::~DataMappingTest()
{
}