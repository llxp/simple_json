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

#include <simple_json/Serializable.h>

#include <memory>
#include <string>

// function used inside addMember() function to register the name for
// serialization, later used in toString() and toStringArray() methods
void simple_json::Serializable::addSerializableMember(
	JsonString &&name, JsonTypes type, bool optional)
{
	m_serializableMembers.push_back(
		std::pair<JsonString, std::pair<JsonTypes, bool>>(
			std::move(name), std::pair<JsonTypes, bool>(type, optional))
	);
}

/*
################################################################
addMember() registers the address of the class member,
so that the variable can be later assigned during deserialization
################################################################
Begin addMember() region
################################################################
*/

JsonString simple_json::Serializable::toString()
{
	this->refresh();
	return DeSerialization::toString();
}

void simple_json::Serializable::addMember(
	JsonString &&name, __int64 & memberVariable, bool optional)
{
	this->m_kvPairNumbers[name] = new JsonParser::Number(&memberVariable);
	this->m_collectibleObjects.push_back(this->m_kvPairNumbers[name]);
	addSerializableMember(std::move(name), JsonTypes::Number, optional);
}

void simple_json::Serializable::addMember(
	JsonString &&name, double & memberVariable, bool optional)
{
	this->m_kvPairNumbers[name] = new JsonParser::Number(&memberVariable);
	this->m_collectibleObjects.push_back(this->m_kvPairNumbers[name]);
	addSerializableMember(std::move(name), JsonTypes::Number, optional);
}

void simple_json::Serializable::addMember(
	JsonString &&name, JsonString & memberVariable, bool optional)
{
	this->m_kvPairStrings[name] = &memberVariable;
	addSerializableMember(std::move(name), JsonTypes::String, optional);
}

void simple_json::Serializable::addMember(
	JsonString &&name,
	simple_json::Serializable & memberVariable,
	bool optional)
{
	this->m_kvPairObjects[name] = &memberVariable;
	addSerializableMember(std::move(name), JsonTypes::Object, optional);
}

void simple_json::Serializable::addMember(
	JsonString &&name, bool & memberVariable, bool optional)
{
	this->m_kvPairBools[name] = &memberVariable;
	addSerializableMember(std::move(name), JsonTypes::Bool, optional);
}

void simple_json::Serializable::addMember(
	JsonString &&name,
	JsonParser::VectorBase &memberVariable, bool optional)
{
	this->m_kvPairArrays[name] = &memberVariable;
	addSerializableMember(std::move(name), JsonTypes::Array, optional);
}

/*################################################################
End addMember() region
################################################################*/