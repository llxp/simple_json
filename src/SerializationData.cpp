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
#include <map>

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

/*void JsonParser::SerializationData::addNumberValue(std::string &&name, std::string &&numberStr)
{
	JsonParser::Number number(numberStr);
	if (this->m_kvPairMappingNumbers.find(name) != this->m_kvPairMappingNumbers.end()) {
		(this->m_kvPairMappingNumbers)[name].setNumberRefValue(std::move(number));
	}
}

void JsonParser::SerializationData::addBoolValue(std::string && name, bool value)
{
	if (this->m_kvPairMappingBools.find(name) != this->m_kvPairMappingBools.end()) {
		(*this->m_kvPairMappingBools[name]) = value;
	}
}

void JsonParser::SerializationData::addStringValue(std::string && name, std::string && str)
{
	if (this->m_kvPairMappingStrings.find(name) != this->m_kvPairMappingStrings.end()) {
		(*this->m_kvPairMappingStrings[name]) = std::move(str);
	}
}

bool JsonParser::SerializationData::objectIsMapped(const std::string & name) const
{
	if (this->m_kvPairMappingObjects.find(name) != this->m_kvPairMappingObjects.end()) {
		return true;
	}
	return false;
}

bool JsonParser::SerializationData::arrayIsMapped(const std::string & name) const
{
	if (this->m_kvPairMappingArrays.find(name) != this->m_kvPairMappingArrays.end()) {
		return true;
	}
	return false;
}

JsonParser::SerializationData * JsonParser::SerializationData::getPointerToObject(std::string && name) const
{
	if (!objectIsMapped(name)) {
		return nullptr;
	}
	return this->m_kvPairMappingObjects.at(name);
}

JsonParser::SerializationData * JsonParser::SerializationData::getPointerToArray(std::string && name) const
{
	if (!arrayIsMapped(name)) {
		return nullptr;
	}
	return this->m_kvPairMappingArrays.at(name).get();
}*/