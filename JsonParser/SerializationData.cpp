#include "SerializationData.h"

JsonParser::SerializationData::SerializationData()
{
}

JsonParser::SerializationData::~SerializationData()
{
}

void JsonParser::SerializationData::setType(const JsonTypes & type)
{
	this->m_type = type;
}

JsonTypes JsonParser::SerializationData::type() const
{
	return this->m_type;
}

void JsonParser::SerializationData::setFullString(std::shared_ptr<std::string> str)
{
	this->m_fullString = str;
}

std::shared_ptr<std::string> JsonParser::SerializationData::fullString() const
{
	return this->m_fullString;
} 

char JsonParser::SerializationData::getChar(const size_t & pos) const
{
	if (this->m_fullString != nullptr && this->m_fullString->length() > pos) {
		return this->m_fullString->at(pos);
	}
	return 0;
}

size_t JsonParser::SerializationData::strLen() const
{
	if (this->m_fullString != nullptr) {
		return this->m_fullString->length();
	}
	return 0;
}

void JsonParser::SerializationData::clearAll()
{
	this->m_kvPairNumbers.clear();
	this->m_kvPairStrings.clear();
	this->m_kvPairBools.clear();
	this->m_kvPairObjects.clear();
	this->m_kvPairArrays.clear();

	this->m_arrayObjects.clear();
	this->m_arrayArrays.clear();
	this->m_arrayNumbers.clear();
	this->m_arrayBools.clear();
	this->m_arrayStrings.clear();
}

bool JsonParser::SerializationData::matchChar(const size_t &i, char ch) const
{
	return getChar(i) == ch;
}

void JsonParser::SerializationData::assign(const std::shared_ptr<SerializationData> &other)
{
	this->m_type = other->m_type;
	this->m_fullString = other->m_fullString;

	this->m_kvPairNumbers = other->m_kvPairNumbers;
	this->m_kvPairStrings = other->m_kvPairStrings;
	this->m_kvPairBools = other->m_kvPairBools;
	this->m_kvPairObjects = other->m_kvPairObjects;
	this->m_kvPairArrays = other->m_kvPairArrays;

	this->m_arrayObjects = other->m_arrayObjects;
	this->m_arrayArrays = other->m_arrayArrays;
	this->m_arrayNumbers = other->m_arrayNumbers;
	this->m_arrayBools = other->m_arrayBools;
	this->m_arrayStrings = other->m_arrayStrings;
}
