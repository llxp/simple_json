/*
MIT License

Copyright (c) 2018 Lukas Lüdke

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <simple_json\SerializationData.h>

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

void JsonParser::SerializationData::setFullString(JsonString *str)
{
	this->m_fullString = str;
	this->m_strLen = str->length();
}

JsonString *JsonParser::SerializationData::fullString() const
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
