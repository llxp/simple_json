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

#include <simple_json/DeSerialization.h>

#include <stdio.h>
#include <ctype.h>
#include <iostream>
#include <memory>
#include <utility>
#include <string>

#include <simple_json\VectorBase.h>
#include <simple_json\SerializationUtils.h>

extern inline void printDebugMessage(const std::string &function, int line);
extern size_t lineCounter;

bool JsonParser::DeSerialization::fromString(JsonString *str)
{
	this->setString(str->data());
	this->setStrLen(str->length());
	size_t pos = 0;
	return this->parseString(pos);
}

bool JsonParser::DeSerialization::parseString(size_t &currentPos)
{
	this->clearMapping();
	this->serialize();
	size_t openingCount(0);
	const size_t tempLen(strLen());
	while(currentPos < tempLen) {
		switch (getChar(currentPos)) {
		case '\n':
			++lineCounter;
			break;
		case JsonObjectOpen:
			++openingCount;
			// Expecting position of JsonObjectClose as result
			if (!addObjectValues(currentPos)) {
				printDebugMessage(__FUNCTION__, __LINE__);
				return false;
			}
			break;
		}
		switch (getChar(currentPos)) {
		case JsonObjectClose:
			--openingCount;
			if (openingCount == 0) {
				return true;
			}
			break;
		}
		++currentPos;
	}
	printDebugMessage(__FUNCTION__, __LINE__);
	return false;
}


bool JsonParser::DeSerialization::addKVPair(size_t &currentPos)
{
	const size_t tempLen(strLen());
	JsonString name;
	while (currentPos < tempLen) {
		switch (getChar(currentPos)) {
			case '\n':
				++lineCounter;
				continue;
			case JsonStringSeparator:
			{
				// Expecting position of closing String quotes as result
				if (!getString(currentPos, name)) {
					printDebugMessage(__FUNCTION__, __LINE__);
					return false;
				}
				if (name.length() == 0) {
					printDebugMessage(__FUNCTION__, __LINE__);
					return false;
				}
			}
			break;
			case JsonKvSeparator:
				return addValue(currentPos, std::move(name));
		}
		++currentPos;
	}
	printDebugMessage(__FUNCTION__, __LINE__);
	return false;
}

bool JsonParser::DeSerialization::addObjectValues(size_t &pos)
{
	const size_t tempLen(strLen());
	bool commaFound(true);
	for (++pos; pos < tempLen; ++pos) {
		switch (getChar(pos)) {
		case JsonStringSeparator:
		{
			if (!addKVPair(pos) || !commaFound) {
				printDebugMessage(__FUNCTION__, __LINE__);
				return false;
			}
			commaFound = false;
		}
		break;
		case JsonEntrySeparator:
			commaFound = true;
			break;
		case JsonObjectClose:
			return true;
		}
	}
	return false;
}


bool JsonParser::DeSerialization::addValue(
	size_t &currentPos, JsonString &&name)
{
	bool commaFound = true;
	bool valueSet = false;
	const size_t tempLen(strLen());
	++currentPos;
	while(currentPos < tempLen) {
		char &&c = getChar(currentPos);
		switch (c) {
			case '\n':
				++lineCounter;
				break;
			case JsonStringSeparator:
				return addStringValue(currentPos, std::move(name));
			case JsonObjectOpen:
				return addObjectValue(currentPos, std::move(name));
			case JsonArrayOpen:
				return addArrayValue(currentPos, std::move(name));
			case 'n':
			case 'N':
				return addNullValue(currentPos, std::move(name));
			case 't':
			case 'T':
			case 'f':
			case 'F':
				return addBoolValue(currentPos, std::move(name));
			default:
				if (isNumber(c)) {
					return addNumberValue(currentPos, std::move(name));
				}
			break;
		}
		++currentPos;
	}
	printDebugMessage(__FUNCTION__, __LINE__);
	return false;
}

/*################################################################
		Begin name value pair parsing related functions
################################################################*/

bool JsonParser::DeSerialization::addStringValue(
	size_t &currentPos, JsonString &&name)
{
	const auto &ptr = this->m_kvPairStrings.find(name);
	if (ptr != this->m_kvPairStrings.end() && ptr->second != nullptr) {
		return this->getString(currentPos, *ptr->second);
	}
	return true;
}


bool JsonParser::DeSerialization::addObjectValue(
	size_t &currentPos, JsonString &&name)
{
	const auto &ptr = this->m_kvPairObjects.find(name);
	if (ptr != this->m_kvPairObjects.end() && ptr->second != nullptr) {
		ptr->second->assign(this);
		return ptr->second->parseString(currentPos);
	} else {
		DeSerialization *obj = new DeSerialization();
		obj->assign(this);
		return obj->parseString(currentPos);
	}
	printDebugMessage(__FUNCTION__, __LINE__);
	return false;
}


bool JsonParser::DeSerialization::addArrayValue(
	size_t &currentPos, JsonString &&name)
{
	const auto &ptr(this->m_kvPairArrays.find(name));
	if (ptr != this->m_kvPairArrays.end() && ptr->second != nullptr) {
		ptr->second->clear();
		ptr->second->assign(this);
		return ptr->second->parseString(currentPos);
	} else {
		JsonParser::Vector<JsonParser::Number> *array = new JsonParser::Vector<JsonParser::Number>();
		array->assign(this);
		return array->parseString(currentPos);
	}
	std::cout << "name: " << name;
	printDebugMessage(__FUNCTION__, __LINE__);
	return false;
}


bool JsonParser::DeSerialization::addNumberValue(
	size_t &currentPos, JsonString &&name)
{
	const size_t tempLen(strLen());
	for (size_t i = currentPos; i < tempLen; ++i) {
		if (!isNumber(getChar(i))) {
			const auto &ptr = this->m_kvPairNumbers.find(name);
			if (ptr != this->m_kvPairNumbers.end() && ptr->second != nullptr) {
				ptr->second->setNumberRefValue(substr(currentPos, i - currentPos));
			}
			currentPos = i - 1;
			return true;
		}
	}
	printDebugMessage(__FUNCTION__, __LINE__);
	return false;
}


bool JsonParser::DeSerialization::addBoolValue(
	size_t &currentPos, JsonString &&name)
{
	const auto &ptr(this->m_kvPairBools.find(name));
	switch (getChar(currentPos)) {
		case 't':  // assigning true to the name
		{
			if (ptr != this->m_kvPairBools.end() && ptr->second != nullptr) {
				(*ptr->second) = true;
			}
			currentPos += constLength("True") - 1;
			return true;
		}
		case 'f':  // assigning false to the name
		{
			if (ptr != this->m_kvPairBools.end() && ptr->second != nullptr) {
				(*ptr->second) = false;
			}
			currentPos += constLength("False") - 1;
			return true;
		}
	}
	printDebugMessage(__FUNCTION__, __LINE__);
	return false;
}

bool JsonParser::DeSerialization::addNullValue(size_t &pos, JsonString &&name)
{
	pos += constLength("null") - 1;
	// this->kvPairNullValues()->push_back(std::move(name));
	return true;
}

/*################################################################
		End name value pair parsing related functions
################################################################*/

void JsonParser::DeSerialization::clearValues()
{
	for (auto it = this->m_kvPairArrays.begin(); it != this->m_kvPairArrays.end(); ++it) {
		if (it->second != nullptr) {
			it->second->clear();
		}
	}
}

// unregisters all mapped member variables
void JsonParser::DeSerialization::clearMapping()
{
	this->m_kvPairArrays.clear();
	this->m_kvPairObjects.clear();
	this->m_kvPairBools.clear();
	this->m_kvPairNumbers.clear();
	this->m_kvPairStrings.clear();

	this->m_serializableMembers.clear();
}

JsonString JsonParser::DeSerialization::toString() const
{
	JsonString outputStr(JsonObjectOpenStr);
	auto endPos = this->m_serializableMembers.end();
	for (auto &&it = this->m_serializableMembers.begin();
		it != endPos;
		++it) {
		JsonString index(it->first);
		switch (it->second.first) {
			case JsonTypes::Number: {
				const auto &ptr = this->m_kvPairNumbers.find(index);
				if (ptr != this->m_kvPairNumbers.end() && ptr->second != nullptr) {
					if ((!(it->second.second && ptr->second->toNumber() == 0)
						|| !it->second.second)) {
						outputStr += SerializationUtils::makeKvPairStrNumber(
								index, *ptr->second);
					} else {
						continue;
					}
				} else {
					continue;
				}
			}
			break;
			case JsonTypes::Bool: {
				const auto &ptr = this->m_kvPairBools.find(index);
				if (ptr != this->m_kvPairBools.end()
					&& ptr->second != nullptr) {
					outputStr += makeKvPairStrBool(index, *ptr->second);
				} else {
					continue;
				}
			}
			break;
			case JsonTypes::String: {
				const auto &ptr = this->m_kvPairStrings.find(index);
				if (ptr != this->m_kvPairStrings.end() && ptr->second != nullptr) {
					if ((!(it->second.second && ptr->second->length() <= 0)
						|| !it->second.second)) {
						outputStr += SerializationUtils::makeKvPairStrString(index, ptr->second);
					} else {
						continue;
					}
				} else {
					continue;
				}
			}
			break;
			case JsonTypes::Object: {
				const auto &ptr = this->m_kvPairObjects.find(index);
				if (ptr != this->m_kvPairObjects.end()
					&& ptr->second != nullptr) {
					auto &&tempStr = ptr->second->toString();
					if (tempStr.length() == 2) {
						continue;
					}
					outputStr += SerializationUtils::makeString(index)
						+ JsonKvSeparator + tempStr;
				}
			}
			break;
			case Array: {
				const auto &ptr = this->m_kvPairArrays.find(index);
				if (ptr != this->m_kvPairArrays.end() && ptr->second != nullptr) {
					outputStr += SerializationUtils::makeString(index)
						+ JsonKvSeparator + ptr->second->toString();
				} else {
					continue;
				}
			}
			break;
			default:
				break;
		}

		if (it + 1 != endPos) {
			outputStr += JsonEntrySeparator;
		}
	}
	if (outputStr[outputStr.length() - 1] == JsonEntrySeparator) {
		outputStr = outputStr.substr(0, outputStr.length() - 1);
	}
	outputStr += JsonObjectClose;
	return outputStr;
}

JsonString JsonParser::DeSerialization::makeKvPairStrBool(const JsonString & name, bool value) const
{
	return SerializationUtils::makeString(name) + JsonKvSeparator +
		(value ? Stringify(true) : Stringify(false));
}

DLLEXPORT void JsonParser::DeSerialization::refresh()
{
	this->clearMapping();
	this->serialize();
}