/*
MIT License

Copyright (c) 2018 Lukas L�dke

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

#include <simple_json/Serializable.h>

#include <memory>
#include <string>

DLLEXPORT bool simple_json::Serializable::fromString()
{
	return JsonParser::DeSerialization::fromString();
}

DLLEXPORT bool simple_json::Serializable::fromString(
	const std::shared_ptr<JsonString>& str)
{
	return JsonParser::DeSerialization::fromString(str);
}

DLLEXPORT JsonString simple_json::Serializable::toString()
{
	this->clearMapping();
	this->serialize();
	return std::move(JsonParser::DeSerialization::toString());
}

DLLEXPORT JsonString simple_json::Serializable::toStringArray()
{
	return std::move(JsonParser::DeSerialization::toStringArray());
}

void simple_json::Serializable::addMember(
	JsonString &&name, __int64 & memberVariable, bool optional)
{
	JsonParser::DeSerialization::addMember(std::move(name), memberVariable, optional);
}

void simple_json::Serializable::addMember(
	JsonString &&name, JsonString & memberVariable, bool optional)
{
	JsonParser::DeSerialization::addMember(std::move(name), memberVariable, optional);
}

void simple_json::Serializable::addMember(
	JsonString &&name,
	simple_json::Serializable & memberVariable,
	bool optional)
{
	JsonParser::DeSerialization::addMember(std::move(name), memberVariable, optional);
}

void simple_json::Serializable::addMember(
	JsonString &&name, bool & memberVariable, bool optional)
{
	JsonParser::DeSerialization::addMember(std::move(name), memberVariable, optional);
}

void simple_json::Serializable::addMember(
	JsonString &&name,
	JsonParser::Vector<JsonString>& memberVariable, bool optional)
{
	JsonParser::DeSerialization::addMember(std::move(name), memberVariable, optional);
}

void simple_json::Serializable::addMember(
	JsonString &&name,
	JsonParser::Vector<bool> & memberVariable, bool optional)
{
	JsonParser::DeSerialization::addMember(std::move(name), memberVariable, optional);
}

void simple_json::Serializable::addMember(
	JsonString &&name,
	JsonParser::Vector<JsonParser::Number> &memberVariable, bool optional)
{
	JsonParser::DeSerialization::addMember(std::move(name), memberVariable, optional);
}

void simple_json::Serializable::addMember(
	JsonString &&name,
	JsonParser::VectorBase &memberVariable, bool optional)
{
	JsonParser::DeSerialization::addMember(std::move(name), memberVariable, optional);
}