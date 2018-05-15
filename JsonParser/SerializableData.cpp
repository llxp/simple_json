#include "SerializableData.h"

JsonParser::SerializableData::SerializableData()
{
}

JsonParser::SerializableData::~SerializableData()
{
}

void JsonParser::SerializableData::setType(const JsonTypes & type)
{
	this->m_type = type;
}

JsonTypes JsonParser::SerializableData::type() const
{
	return this->m_type;
}

void JsonParser::SerializableData::setFullString(std::string * str)
{
	this->m_fullString = str;
}

std::string * const JsonParser::SerializableData::fullString() const
{
	return this->m_fullString;
}

char JsonParser::SerializableData::getChar(const size_t & pos) const
{
	if (this->m_fullString != nullptr && this->m_fullString->length() > pos) {
		return this->m_fullString->at(pos);
	}
	return 0;
}

size_t JsonParser::SerializableData::strLen() const
{
	if (this->m_fullString != nullptr) {
		return this->m_fullString->length();
	}
	return 0;
}
