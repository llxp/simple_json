#include "DataMappingTest.h"


DataMappingTest::DataMappingTest()
{
	addMember("name", m_name);
	addMember("nextLink", m_nextLink);
	addMember("value", m_value);
	addMember("array", m_array);
	addMember("intVal", m_intVal);
	//addMember("objectArray", (JsonParser::Vector<std::string, SerializationMapping<std::string> *>)&m_objectArray);
	addMember("testArray", m_testArray);//TEST if working or not!
	/*dynamic_cast<JsonParser::Vector<std::string, SerializationMapping<std::string>> *>(&m_testArray)*/
}


DataMappingTest::~DataMappingTest()
{
}
