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

#include <simple_json\DeSerialization.h>
#include <stdio.h>
#include <ctype.h>
#include <iostream>
#include <memory>
#include <utility>
#include <string>

bool JsonParser::DeSerialization::fromString()
{
	if (strLen() > 0) {
		bool parsed = fromString(0) == this->strLen() - 1;
		if (parsed) {
			return true;
		} else {
			this->clearAll();
			return false;
		}
	}
	return true;
}

bool JsonParser::DeSerialization::fromString(
	const std::shared_ptr<JsonString> &str)
{
	this->setFullString(str.get());
	return this->fromString();
}

size_t JsonParser::DeSerialization::fromString(const size_t &currentPos)
{
	size_t openingCount = 0;
	size_t closeCount = 0;
	size_t tempLen = strLen();
	for (size_t i = currentPos; i < tempLen; ++i) {
		if (getChar(i) == JsonObjectOpen) {
			++openingCount;
			if ((i = addKVPair(i)) <= 0) {
				// Expecting position of JsonObjectClose as result
				return 0;
			}
		}
		switch (getChar(i)) {
		case JsonObjectClose:
			++closeCount;
			break;
		}
		if (openingCount == closeCount) {
			return i;
		}
	}
	return 0;
}

size_t JsonParser::DeSerialization::fromStringArray(const size_t &currentPos)
{
	size_t openingCount = 0;
	size_t closeCount = 0;
	size_t tempLen = strLen();
	for (size_t i = currentPos; i < tempLen; ++i) {
		if (getChar(i) == JsonArrayOpen) {
			++openingCount;
			if ((i = addArrayValues(i)) <= 0
				// Expecting position of JsonArrayClose as result
				) {
				return 0;
			}
		}
		switch (getChar(i)) {
		case JsonArrayClose:
			++closeCount;
		}
		if (openingCount == closeCount) {
			return i;
		}
	}
	return 0;
}


size_t JsonParser::DeSerialization::addArrayValues(const size_t &currentPos)
{
	bool commaFound = true;
	size_t tempLen = strLen();
	for (size_t i = currentPos + 1; i < tempLen; ++i) {
		switch (getChar(i)) {
		case JsonObjectOpen:
		{
			if ((i = addObjectToArray(i)) <= 0
				// Expecting position of JsonObjectClose as result
				|| !commaFound) {
				return 0;
			}
			setType(JsonTypes::ObjectArray);
			commaFound = false;
		}
		break;
		case JsonArrayOpen:
		{
			if ((i = addArrayToArray(i)) <= 0
				|| !commaFound) {
				return 0;
			}
			setType(JsonTypes::ArrayArray);
			commaFound = false;
		}
		break;
		case JsonStringSeparator:
		{
			if ((i = addStringToArray(i)) <= 0
				|| !commaFound) {
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
			if (!commaFound) {
				return i;
			}
			break;
		default:
			if (isNumber(i)) {
				if ((i = addNumberToArray(i)) <= 0
					|| !commaFound) {
					return 0;
				}
				setType(JsonTypes::NumberArray);
				commaFound = false;
			} else if (isBool(i)) {
				if ((i = addBoolToArray(i)) <= 0
					|| !commaFound) {
					return 0;
				}
				setType(JsonTypes::BoolArray);
				commaFound = false;
			}
		}
	}
	return 0;
}


bool JsonParser::DeSerialization::checkEscape(const size_t &currentPos) const
{
	bool currentPosIsEscape = currentPos > 0 && matchChar(currentPos - 1, '\\');
	if (currentPosIsEscape) {
		bool previousPosIsEscaped =
			currentPos > 1 && matchChar(currentPos - 2, '\\');
		return !previousPosIsEscaped;
	}
	return currentPosIsEscape;
}


size_t JsonParser::DeSerialization::addKVPair(const size_t &currentPos)
{
	size_t tempLen = strLen();
	JsonString name;
	for (size_t i = currentPos; i < tempLen; ++i) {
		switch (getChar(i)) {
		case JsonStringSeparator:
			name = JsonString();
			if ((i = this->getString(i, &name)) <= 0
				/* Expecting position of closing String quotes as result */) {
				return 0;
			}
			break;
		case JsonKvSeparator:
			if ((i = addValue(i, std::move(name))) <= 0
				// Expecting position of JsonObjectClose or
				// JsonArrayClose as result
				) {
				return 0;
			}
			break;
		}
		switch (getChar(i)) {
		case JsonArrayClose:
		case JsonObjectClose:
			return i;
		}
	}
	return 0;
}


size_t JsonParser::DeSerialization::getString(
	const size_t &currentPos, JsonString *name) const
{
	if (name == nullptr) {
		return 0;
	}
	size_t tempLen = strLen();
	for (size_t i = currentPos + 1; i < tempLen; ++i) {
		if (getChar(i) == JsonStringSeparator && !checkEscape(i)) {
			*name = std::move(substr(currentPos + 1, i - (currentPos + 1)));
			return i;
		}
	}
	return 0;
}


size_t JsonParser::DeSerialization::addValue(
	const size_t &currentPos, const JsonString &name)
{
	if (name.length() <= 0) {
		return 0;
	}
	bool commaFound = true;
	bool valueSet = false;
	const size_t tempLen = strLen();
	for (size_t i = currentPos + 1; i < tempLen; ++i) {
		switch (getChar(i)) {
		case JsonStringSeparator:
			if ((i = addStringValue(i, std::move(name))) <= 0
				|| !commaFound) {
				return 0;
			}
			valueSet = true;
			commaFound = false;
			break;
		case JsonObjectOpen:
			if ((i = addObjectValue(i, std::move(name))) <= 0
				|| !commaFound) {
				return 0;
			}
			valueSet = true;
			commaFound = false;
			break;
		case JsonArrayOpen:
			if ((i = addArrayValue(i, std::move(name))) <= 0
				|| !commaFound) {
				return 0;
			}
			valueSet = true;
			commaFound = false;
			break;
		case JsonEntrySeparator:
			if (valueSet && !commaFound) {
				return i;
			}
			commaFound = true;
			break;
		case JsonObjectClose:
			if (!commaFound) {
				return i;
			}
			break;
		default:
			if (isNumber(i)) {
				if ((i = addNumberValue(i, std::move(name))) <= 0 || !commaFound) {
					return 0;
				}
				valueSet = true;
				commaFound = false;
			} else if (isBool(i)) {
				if ((i = addBoolValue(i, std::move(name))) <= 0 || !commaFound) {
					return 0;
				}
				valueSet = true;
				commaFound = false;
			} else if (isNull(i)) {
				if (!commaFound) {
					return 0;
				}
				addNullValue(std::move(name));
				i = i + constLength("null") - 1;
				valueSet = true;
				commaFound = false;
			}
		}
		if (i >= tempLen) {
			return 0;
		}
	}
	return 0;
}


bool JsonParser::DeSerialization::isNumber(const size_t &currentPos) const
{
	if ((getChar(currentPos) >= 48
		&& getChar(currentPos) <= 57)
		|| getChar(currentPos) == '.'
		|| getChar(currentPos) == '-') {
		return true;
	}
	return false;
}


bool JsonParser::DeSerialization::isBool(const size_t &currentPos) const
{
	switch (getChar(currentPos)) {
	case 't':
	case 'T':
	case 'f':
	case 'F':
		return true;
	}
	return false;
}

bool JsonParser::DeSerialization::isNull(const size_t &currentPos) const
{
	switch (getChar(currentPos)) {
	case 'n':
	case 'N':
		return true;
	}
	return false;
}


size_t JsonParser::DeSerialization::addStringValue(
	const size_t &currentPos, const JsonString &name)
{
	size_t i = currentPos;
	JsonString value;
	if ((i = this->getString(i, &value)) <= 0) {
		return 0;
	}
	this->kvPairStrings()->operator[](std::move(name)) = std::move(value);
	return i;
}


size_t JsonParser::DeSerialization::addObjectValue(
	const size_t &currentPos, const JsonString &name)
{
	size_t i = currentPos;
	auto child = std::make_unique<JsonParser::DeSerialization>();
	child->setFullString(this->fullString());
	if ((i = child->fromString(i)) <= 0) {
		return 0;
	}
	this->kvPairObjects()->operator[](std::move(name)) = std::move(child);
	return i;
}


size_t JsonParser::DeSerialization::addArrayValue(
	const size_t &currentPos, const JsonString &name)
{
	size_t pos = currentPos;
	auto child = std::make_unique<JsonParser::DeSerialization>();
	child->setFullString(this->fullString());
	if ((pos = child->fromStringArray(currentPos)) <= 0) {
		return 0;
	}
	this->kvPairArrays()->operator[](std::move(name)) = std::move(child);
	return pos;
}


size_t JsonParser::DeSerialization::addNumberValue(
	const size_t &currentPos, const JsonString & name)
{
	const size_t tempLen = strLen();
	for (size_t i = currentPos; i < tempLen; ++i) {
		if (!isNumber(i)) {
			JsonParser::Number number(std::move(substr(currentPos, i - currentPos)));
			(*this->kvPairNumbers())[std::move(name)] = std::move(number);
			return i - 1;
		}
	}
	return 0;
}


size_t JsonParser::DeSerialization::addBoolValue(
	const size_t &currentPos, const JsonString & name)
{
	switch(getChar(currentPos)) {
	case 't':  // assigning true to the name
		this->kvPairBools()->operator[](std::move(name)) = true;
		return currentPos + constLength("True") - 1;
	case 'f':  // assigning false to the name
		this->kvPairBools()->operator[](std::move(name)) = false;
		return currentPos + constLength("False") - 1;
	}
	return 0;
}

void JsonParser::DeSerialization::addNullValue(const JsonString & name)
{
	this->kvPairNullValues()->push_back(std::move(name));
}


size_t JsonParser::DeSerialization::addArrayToArray(const size_t &currentPos)
{
	size_t i = currentPos;
	auto child = std::make_unique<JsonParser::DeSerialization>();
	child->setFullString(this->fullString());
	if ((i = child->fromStringArray(i)) <= 0) {
		return 0;
	}
	this->arrayArrays()->push_back(std::move(child));
	return i;
}


size_t JsonParser::DeSerialization::addStringToArray(const size_t &currentPos)
{
	size_t i = currentPos;
	JsonString value;
	if ((i = this->getString(i, &value)) <= 0) {
		return 0;
	}
	this->arrayStrings()->push_back(std::move(value));
	return i;
}


size_t JsonParser::DeSerialization::addObjectToArray(const size_t &currentPos)
{
	size_t i = currentPos;
	auto child = std::make_unique<JsonParser::DeSerialization>();
	child->setFullString(this->fullString());
	if ((i = child->fromString(i)) <= 0) {
		return 0;
	}
	this->arrayObjects()->push_back(std::move(child));
	return i;
}


size_t JsonParser::DeSerialization::addNumberToArray(const size_t &currentPos)
{
	const size_t tempLen = strLen();
	// iterate as long there is a number, otherwise,
	// copy the substring and create a number
	for (size_t i = currentPos; i < tempLen; ++i) {
		if (!isNumber(i)) {
			JsonParser::Number number(std::move(substr(currentPos, i - currentPos)));
			this->arrayNumbers()->push_back(std::move(number));
			return i - 1;
		}
	}
	return 0;
}

size_t JsonParser::DeSerialization::addBoolToArray(const size_t &currentPos)
{
	switch(getChar(currentPos)) {
	case 't':  // adding true to the array
		this->arrayBools()->push_back(std::move(true));
		return currentPos + constLength("True") - 1;
	case 'f':  // adding false to the array
		this->arrayBools()->push_back(std::move(false));
		return currentPos + constLength("False") - 1;
	}
	return 0;
}
