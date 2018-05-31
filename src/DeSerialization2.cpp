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
#include <iterator>

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
	std::unique_ptr<std::istringstream> streamPtr = std::make_unique<std::istringstream>(*(str.get()));
	std::unique_ptr<std::istreambuf_iterator<char>> streambufPtr = std::make_unique<std::istreambuf_iterator<char>>(*streamPtr);
	this->setFullString(streambufPtr.get());
	return this->fromString();
	//return false;
}

/*bool JsonParser::DeSerialization2::fromString(std::istreambuf_iterator<char> * str)
{
	this->setFullString(str);
	return this->fromString();
}*/

bool JsonParser::DeSerialization2::fromString(char c)
{
	size_t openingCount = 0;
	size_t closeCount = 0;
	do {
		switch(c) {
		case JsonObjectOpen:
			++openingCount;
			if ((c = addKVPair()) <= 0) {
				return false;
			}
			break;
		}
		switch(c) {
		case JsonObjectClose:
			++closeCount;
			break;
		}
		if (openingCount > 0 && openingCount == closeCount) {
			return true;
		}
	} while ((c = getNextChar()) > 0);
	return false;
}

char JsonParser::DeSerialization2::addKVPair()
{
	JsonString name;
	char c = 0;
	while ((c = getNextChar()) > 0) {
		switch (c) {
		case JsonStringSeparator:
			name = JsonString();
			if (!this->getString(&name)) {
				return 0;
			}
			break;
		case JsonKvSeparator:
			if ((c = addValue(name)) <= 0) {
				return 0;
			}
			break;
		}
		switch(c) {
		case JsonArrayClose:
		case JsonObjectClose:
			return c;
		}
	};
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
	while ((c = getNextChar()) > 0) {
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
			if (!addArrayValue(c, name) || commaFound == false) {
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
			if (isNumber(c)) {
				if ((c = addNumberValue(c, name)) <= 0
					|| !commaFound) {
					return 0;
				}
				valueSet = true;
				commaFound = false;
				if (c == JsonEntrySeparator) {
					return c;
				}
			}
			else if (isBool(c)) {
				if (!addBoolValue(c, name)
					|| !commaFound) {
					return 0;
				}
				valueSet = true;
				commaFound = false;
			}
			else if (isNull(c)) {
				if (!commaFound) {
					return 0;
				}
				addNullValue(name);
				valueSet = true;
				commaFound = false;
			}
		}
	}
	return 0;
}

bool JsonParser::DeSerialization2::isBool(char c) const
{
	switch (c) {
	case 't':
	case 'T':
	case 'f':
	case 'F':
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
	switch (c) {
	case 'n':
	case 'N':
		return true;
	}
	return false;
}

bool JsonParser::DeSerialization2::addStringValue(const JsonString &name)
{
	JsonString value;
	if (!this->getString(&value)) {
		return false;
	}
	(*this->kvPairStrings())[name] = std::move(value);
	return true;
}

bool JsonParser::DeSerialization2::getString(JsonString *name) const
{
	if (name == nullptr) {
		return false;
	}
	char c = 0;
	char ch1 = -1;
	char ch2 = -1;
	std::string tempStr;
	while ((c = getNextChar()) > 0) {
		if (c != JsonStringSeparator || checkEscape(ch1, ch2)) {
			tempStr += c;
		}
		else {
			*name = std::move(tempStr);
			return true;
		}
		ch2 = ch1;
		ch1 = c;
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
	//if (this->kvPairObjects()->find(name) == this->kvPairObjects()->end()) {
		(*this->kvPairObjects())[name] = std::move(child);
	//}
	return true;
}

bool JsonParser::DeSerialization2::addArrayValue(char c, const JsonString &name)
{
	auto child = std::make_unique<JsonParser::DeSerialization2>();
	child->setFullString(this->fullString());
	if (!child->fromStringArray(c)) {
		return false;
	}
	//if (this->kvPairArrays()->find(name) == this->kvPairArrays()->end()) {
		(*this->kvPairArrays())[name] = std::move(child);
	//}
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
	} while ((c = getNextChar()) > 0);
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
		bool previousPosIsEscaped = ch2 == '\\';
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
		if (openingCount > 0 && openingCount == closeCount) {
			return true;
		}
	} while ((c = getNextChar()) > 0);
	return false;
}

char JsonParser::DeSerialization2::parseStringArray()
{
	bool commaFound = true;
	char c = 0;
	while ((c = getNextChar()) > 0) {
		switch (c) {
		case JsonObjectOpen:
		{
			if (!addObjectToArray(c) || !commaFound) {
				return 0;
			}
			setType(JsonTypes::ObjectArray);
			commaFound = false;
		}
		break;
		case JsonArrayOpen:
		{
			if (!addArrayToArray(c) || !commaFound) {
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
	} while ((c = getNextChar()) > 0);
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
	if (!this->getString(&value)) {
		return false;
	}
	this->arrayStrings()->push_back(std::move(value));
	return true;
}


bool JsonParser::DeSerialization2::addObjectToArray(char c)
{
	auto child = std::make_unique<JsonParser::DeSerialization2>();
	child->setFullString(this->fullString());
	if (!child->fromString(c)) {
		return false;
	}
	this->arrayObjects()->push_back(std::move(child));
	return true;
}

bool JsonParser::DeSerialization2::addArrayToArray(char c)
{
	auto child = std::make_unique<JsonParser::DeSerialization2>();
	child->setFullString(this->fullString());
	if (!child->fromStringArray(c)) {
		return false;
	}
	this->arrayArrays()->push_back(std::move(child));
	return true;
}