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

void JsonParser::SerializationData::setFullString(std::vector<char> *str)
{
	this->m_fullString = str;
}

std::vector<char> *JsonParser::SerializationData::fullString() const
{
	return this->m_fullString;
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

void JsonParser::SerializationData::assign(const std::unique_ptr<SerializationData> &other)
{
	this->m_type = other->m_type;
	this->m_fullString = other->m_fullString;

	this->m_kvPairNumbers = std::move(other->m_kvPairNumbers);
	this->m_kvPairStrings = std::move(other->m_kvPairStrings);
	this->m_kvPairBools = std::move(other->m_kvPairBools);
	this->m_kvPairObjects = std::move(other->m_kvPairObjects);
	this->m_kvPairArrays = std::move(other->m_kvPairArrays);

	this->m_arrayObjects = std::move(other->m_arrayObjects);
	this->m_arrayArrays = std::move(other->m_arrayArrays);
	this->m_arrayNumbers = std::move(other->m_arrayNumbers);
	this->m_arrayBools = std::move(other->m_arrayBools);
	this->m_arrayStrings = std::move(other->m_arrayStrings);
}
