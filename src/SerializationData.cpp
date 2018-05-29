#include "SerializationData.h"

#include <utility>
#include <memory>
#include <string>

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

void JsonParser::SerializationData::setFullString(std::string *str)
{
	this->m_fullString = str;
	this->m_strLen = str->length();
}

std::string *JsonParser::SerializationData::fullString() const
{
	return this->m_fullString;
}

void JsonParser::SerializationData::clearAll()
{
	this->m_kvPairNumbers.reset();
	this->m_kvPairStrings.reset();
	this->m_kvPairBools.reset();
	this->m_kvPairObjects.reset();
	this->m_kvPairArrays.reset();
}

void JsonParser::SerializationData::assign(
	const std::unique_ptr<SerializationData> &other)
{
	this->m_type = std::move(other->m_type);
	this->setFullString(std::move(other->fullString()));

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
