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

#include <simple_json\DeSerialization2.h>
#include <stdio.h>
#include <ctype.h>
#include <iostream>
#include <memory>
#include <utility>
#include <string>
#include <sstream>

JsonString JsonParser::DeSerialization2::toString() const
{
	return JsonString();
}

JsonString JsonParser::DeSerialization2::toStringArray() const
{
	return JsonString();
}

bool JsonParser::DeSerialization2::fromString(const std::shared_ptr<JsonString>& str)
{
	this->setFullString(new std::istringstream(*(str.get())));
	return this->fromString();
}

bool JsonParser::DeSerialization2::fromString(std::istream * str)
{
	this->setFullString(str);
	return this->fromString();
}
bool JsonParser::DeSerialization2::fromString(char c)
{
	size_t openingCount = 0;
	size_t closeCount = 0;
	do {
		if (c == JsonObjectOpen) {
			openingCount++;
		}
		if (c != JsonObjectOpen && c != JsonObjectClose) {
			if ((c = addKVPair(c)) <= 0) {
				return false;
			}
		}
		if (c == JsonObjectClose) {
			closeCount++;
		}
		if (openingCount == closeCount) {
			return true;
		}
	} while ((c = getNextChar()) >= 0);
	return false;
}

char JsonParser::DeSerialization2::addKVPair(char c)
{
	JsonString name;
	do {
		if (c == JsonStringSeparator) {
			name = JsonString();
			if (!this->getName(&name)) {
				return 0;
			}
		}
		else if (c == JsonKvSeparator) {
			if ((c = addValue(name)) <= 0) {
				return 0;
			}
		}
		if (c == JsonArrayClose || c == JsonObjectClose) {
			return c;
		}
	} while ((c = getNextChar()) >= 0);
	return c;
}

char JsonParser::DeSerialization2::addValue(const JsonString &name)
{
	if (name.length() <= 0) {
		return 0;
	}
	char c = 0;
	bool commaFound = true;
	bool valueSet = false;
	while ((c = getNextChar()) >= 0) {
		switch (c) {
		case JsonStringSeparator:
			if (!addStringValue(name) || commaFound == false) {
				return 0;
			}
			valueSet = true;
			commaFound = false;
			break;
		case JsonObjectOpen:
			if (!addObjectValue(c, name) || commaFound == false) {
				return 0;
			}
			valueSet = true;
			commaFound = false;
			break;
		case JsonArrayOpen:
			if (!addArrayValue(name) || commaFound == false) {
				return 0;
			}
			valueSet = true;
			commaFound = false;
			break;
		case JsonEntrySeparator:
			if (valueSet && !commaFound) {
				return c;
			}
			commaFound = true;
			break;
		case JsonObjectClose:
			if (commaFound == false) {
				return c;
			}
			break;
		default:
			if (!isNumber(c)) {
				if ((c = addNumberValue(c, name)) <= 0
					|| commaFound == false) {
					return 0;
				}
				valueSet = true;
				commaFound = false;
				if (c == JsonEntrySeparator) {
					return c;
				}
			}
			else if (!isBool(c)) {
				if (addBoolValue(c, name) || commaFound == false) {
					return 0;
				}
				valueSet = true;
				commaFound = false;
			}
			else if (!isNull(c)) {
				if (commaFound == false) {
					return 0;
				}
				addNullValue(name);
				valueSet = true;
				commaFound = false;
			}
		}
	}
}

bool JsonParser::DeSerialization2::isBool(char c) const
{
	if (c == 't' || c == 'T' || c == 'f' || c == 'F') {
		return true;
	}
	return false;
}

bool JsonParser::DeSerialization2::isNumber(char c) const
{
	if (c >= 48 && c <= 57 || c == '-' || c == '.') {
		return true;
	}
	return false;
}

bool JsonParser::DeSerialization2::isNull(char c) const
{
	if (c == 'n' || c == 'N') {
		return true;
	}
	return false;
}

bool JsonParser::DeSerialization2::addStringValue(const JsonString &name)
{
	JsonString value;
	if (!this->getName(&value)) {
		return false;
	}
	(*this->kvPairStrings())[name] = std::move(value);
	return true;
}

bool JsonParser::DeSerialization2::getName(JsonString *name) const
{
	if (name == nullptr) {
		return false;
	}
	char c = 0;
	char ch1 = -1;
	char ch2 = -1;
	while ((c = getNextChar()) >= 0) {
		if (c != JsonStringSeparator || checkEscape(ch1, ch2)) {
			*name += c;
		}
		else {
			return true;
		}
		if (ch1 == -1) {
			ch1 = c;
		}
		else {
			ch2 = ch1;
			ch1 = c;
		}
	}
	return false;
}

bool JsonParser::DeSerialization2::addObjectValue(
	char c, const JsonString &name)
{
	auto child = std::make_unique<JsonParser::DeSerialization2>();
	child->setFullString(this->fullString());
	if (!child->fromString(c)) {
		return false;
	}
	if (this->kvPairObjects()->find(name) == this->kvPairObjects()->end()) {
		(*this->kvPairObjects())[name] = std::move(child);
	}
	return true;
}

bool JsonParser::DeSerialization2::addArrayValue(const JsonString &name)
{
	auto child = std::make_unique<JsonParser::DeSerialization2>();
	child->setFullString(this->fullString());
	if (!child->fromStringArray()) {
		return false;
	}
	if (this->kvPairArrays()->find(name) == this->kvPairArrays()->end()) {
		(*this->kvPairArrays())[name] = std::move(child);
	}
	return true;
}

char JsonParser::DeSerialization2::addNumberValue(
	char c, const JsonString & name)
{
	std::string numberStr;
	do {
		if (isNumber(c)) {
			numberStr += c;
		}
		else {
			if (this->kvPairNumbers()->find(name) ==
				this->kvPairNumbers()->end()
				&& numberStr.length() <= MAX_JSON_NUMBER_LENGTH) {
				JsonParser::Number number(numberStr);
				(*this->kvPairNumbers())[name] = std::move(number);
			}
			return c;
		}
	} while ((c = getNextChar()) >= 0);
	return c;
}

bool JsonParser::DeSerialization2::addBoolValue(
	char c, const JsonString & name)
{
	if (c == 't') {
		(*this->kvPairBools())[name] = true;
		return true;
	}
	else if (c == 'f') {
		(*this->kvPairBools())[name] = false;
		return true;
	}
	return false;
}

void JsonParser::DeSerialization2::addNullValue(const JsonString & name)
{
	this->kvPairNullValues()->push_back(name);
}

bool JsonParser::DeSerialization2::checkEscape(char ch1, char ch2) const
{
	bool currentPosIsEscaped = ch1 == '\\';
	if (currentPosIsEscaped) {
		bool previousPosIsEscaped = ch2 != -1 && ch2 == '\\';
		return !previousPosIsEscaped;
	}
	return currentPosIsEscaped;
}

bool JsonParser::DeSerialization2::fromStringArray(char c)
{
	size_t openingCount = 0;
	size_t closeCount = 0;
	do {
		if (c == JsonArrayOpen) {
			++openingCount;
			if ((c = parseStringArray()) <= 0) {
				return false;
			}
		}
		if (c == JsonArrayClose) {
			++closeCount;
		}
		if (openingCount == closeCount) {
			return true;
		}
	} while ((c = getNextChar()) >= 0);
	return false;
}

char JsonParser::DeSerialization2::parseStringArray()
{
	bool commaFound = true;
	char c = 0;
	while ((c = getNextChar()) >= 0) {
		switch (c) {
		case JsonObjectOpen:
		{
			if (!addObjectToArray() || !commaFound) {
				return 0;
			}
			setType(JsonTypes::ObjectArray);
			commaFound = false;
		}
		break;
		case JsonArrayOpen:
		{
			if (!addArrayToArray() || !commaFound) {
				return 0;
			}
			setType(JsonTypes::ArrayArray);
			commaFound = false;
		}
		break;
		case JsonStringSeparator:
		{
			if (!addStringToArray() || !commaFound) {
				return 0;
			}
			setType(JsonTypes::StringArray);
			commaFound = false;
		}
		break;
		case JsonEntrySeparator:
			commaFound = true;
			break;
		case JsonArrayClose:
			if (commaFound == false) {
				return c;
			}
			break;
		default:
			if (isNumber(c)) {
				if ((c = addNumberToArray(c)) <= 0 || !commaFound) {
					return 0;
				}
				setType(JsonTypes::NumberArray);
				commaFound = false;
				if (c == JsonArrayClose) {
					return c;
				}
			}
			else if (isBool(c)) {
				if (!addBoolToArray(c) || !commaFound) {
					return 0;
				}
				setType(JsonTypes::BoolArray);
				commaFound = false;
			}
		}
	}
	return c;
}

char JsonParser::DeSerialization2::addNumberToArray(char c)
{
	JsonString numberStr;
	do {
		if (isNumber(c)) {
			numberStr += c;
		}
		else {
			JsonParser::Number number(numberStr);
			this->arrayNumbers()->push_back(std::move(number));
			return c;
		}
	} while ((c = getNextChar()) >= 0);
	return c;
}

bool JsonParser::DeSerialization2::addBoolToArray(char c)
{
	if (c == 't') {
		this->arrayBools()->push_back(true);
		return true;
	}
	else if (c == 'f') {
		this->arrayBools()->push_back(false);
		return true;
	}
	return false;
}

bool JsonParser::DeSerialization2::addStringToArray()
{
	JsonString value;
	if (!this->getName(&value)) {
		return false;
	}
	this->arrayStrings()->push_back(std::move(value));
	return true;
}


bool JsonParser::DeSerialization2::addObjectToArray()
{
	auto child = std::make_unique<JsonParser::DeSerialization2>();
	child->setFullString(this->fullString());
	if (!child->fromString()) {
		return false;
	}
	this->arrayObjects()->push_back(std::move(child));
	return true;
}

bool JsonParser::DeSerialization2::addArrayToArray()
{
	auto child = std::make_unique<JsonParser::DeSerialization2>();
	child->setFullString(this->fullString());
	if (!child->fromStringArray()) {
		return false;
	}
	this->arrayArrays()->push_back(std::move(child));
	return true;
}