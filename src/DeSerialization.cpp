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
		bool parsed = parseString();
		if(parsed == true) {
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

bool JsonParser::DeSerialization::fromString(const std::istream *str)
{
	return false;
}

size_t JsonParser::DeSerialization::fromString(const size_t &currentPos)
{
	size_t i = 0;
	size_t openingCount = 0;
	size_t closeCount = 0;
	size_t tempLen = strLen();
	for (i = currentPos; i < tempLen; ++i) {
		if (getChar(i) == JsonObjectOpen) {
			++openingCount;
		} else if (getChar(i) == JsonObjectClose) {
			++closeCount;
		} else {
			if ((i = addKVPair(i)) <= 0) {
				return 0;
			}
			--i;
		}
		if (openingCount == closeCount) {
			break;
		}
	}
	return i;
}

size_t JsonParser::DeSerialization::fromStringArray(const size_t &currentPos)
{
	size_t i = 0;
	size_t openingCount = 0;
	size_t closeCount = 0;
	size_t tempLen = strLen();
	for (i = currentPos; i < tempLen; ++i) {
		if (getChar(i) == JsonArrayOpen) {
			++openingCount;
			if ((i = parseStringArray(i + 1)) <= 0) {
				return 0;
			}
			--i;
		} else if (getChar(i) == JsonArrayClose) {
			++closeCount;
		}
		if (openingCount == closeCount) {
			break;
		}
	}
	return i;
}


size_t JsonParser::DeSerialization::parseStringArray(const size_t &currentPos)
{
	size_t i = 0;
	bool commaFound = true;
	size_t tempLen = strLen();
	if (currentPos > 0
		&& tempLen > 0
		&& tempLen > currentPos
		&& getChar(currentPos - 1) == JsonArrayOpen) {
		for (i = currentPos; i < tempLen; ++i) {
			switch (getChar(i)) {
			case JsonObjectOpen:
			{
				if ((i = addObjectToArray(i)) <= 0 || !commaFound) {
					return 0;
				}
				setType(JsonTypes::ObjectArray);
				commaFound = false;
			}
			break;
			case JsonArrayOpen:
			{
				if ((i = addArrayToArray(i)) <= 0 || !commaFound) {
					return 0;
				}
				setType(JsonTypes::ArrayArray);
				commaFound = false;
			}
			break;
			case JsonStringSeparator:
			{
				if ((i = addStringToArray(i)) <= 0 || !commaFound) {
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
					return i;
				}
				break;
			default:
				if (isNumber(i)) {
					if ((i = addNumberToArray(i)) <= 0 || !commaFound) {
						return 0;
					}
					--i;
					setType(JsonTypes::NumberArray);
					commaFound = false;
				} else if (isBool(i)) {
					if ((i = addBoolToArray(i)) <= 0 || !commaFound) {
						return 0;
					}
					setType(JsonTypes::BoolArray);
					--i;
					commaFound = false;
				}
			}
		}
	}
	return i;
}


JsonParser::DeSerialization::DeSerialization()
{
}


JsonParser::DeSerialization::~DeSerialization()
{
}


JsonString JsonParser::DeSerialization::toString() const
{
	return JsonString();
}

JsonString JsonParser::DeSerialization::toStringArray() const
{
	return JsonString();
}


bool JsonParser::DeSerialization::parseString()
{
	size_t openingCount = 0;
	size_t closeCount = 0;
	size_t tempLen = strLen();
	for (size_t i = 0; i < tempLen; ++i) {
		if (matchChar(i, JsonObjectOpen)) {
			++openingCount;
		} else if (matchChar(i, JsonObjectClose)) {
			++closeCount;
		} else {
			if ((i = addKVPair(i)) <= 0) {
				return false;
			}
			--i;
		}
	}
	return openingCount == closeCount;
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
	size_t i = 0;
	size_t tempLen = strLen();
	if (currentPos > 0 && tempLen > 0 && tempLen > currentPos) {
		JsonString name;
		for (i = currentPos; i < tempLen; ++i) {
			switch (getChar(i)) {
			case JsonStringSeparator:
				if ((i = this->getName(i + 1, &name)) <= 0) {
					return 0;
				}
				break;
			case JsonKvSeparator:
				if ((i = addValue(i + 1, name)) <= 0) {
					name = JsonString();
					return 0;
				}
				--i;
				name = JsonString();
				break;
			case JsonArrayClose:
			case JsonObjectClose:
			// case JsonEntrySeparator:
				return i;
			}
		}
	}
	return i;
}


size_t JsonParser::DeSerialization::getName(
	const size_t &currentPos, JsonString *name) const
{
	if (name == nullptr) {
		return 0;
	}
	size_t i = 0;
	size_t tempLen = strLen();
	if (currentPos > 0 && tempLen > 0 && tempLen > currentPos) {
		for (i = currentPos; i < tempLen; ++i) {
			if (getChar(i) == JsonStringSeparator && !checkEscape(i)) {
				*name = substr(currentPos, i - currentPos);
				return i;
			}
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
	size_t i = 0;
	bool commaFound = true;
	bool valueSet = false;
	size_t tempLen = strLen();
	if (currentPos > 0 && tempLen > 0 && tempLen > currentPos) {
		for (i = currentPos; i < tempLen; ++i) {
			switch (getChar(i)) {
			case JsonStringSeparator:
				if ((i = addStringValue(i + 1, name)) <= 0 || commaFound == false) {
					return 0;
				}
				valueSet = true;
				commaFound = false;
				break;
			case JsonObjectOpen:
				if ((i = addObjectValue(i, name)) <= 0 || commaFound == false) {
					return 0;
				}
				valueSet = true;
				commaFound = false;
				break;
			case JsonArrayOpen:
				if ((i = addArrayValue(i, name)) <= 0 || commaFound == false) {
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
				if (commaFound == false) {
					return i;
				}
				break;
			default:
				if (isNumber(i)) {
					if ((i = addNumberValue(i, name)) <= 0 || commaFound == false) {
						return 0;
					}
					--i;
					valueSet = true;
					commaFound = false;
				} else if (isBool(i)) {
					if ((i = addBoolValue(i, name)) <= 0 || commaFound == false) {
						return 0;
					}
					--i;
					valueSet = true;
					commaFound = false;
				} else if (isNull(i)) {
					if ((i = addNullValue(i, name)) <= 0 || commaFound == false) {
						return 0;
					}
					--i;
					valueSet = true;
					commaFound = false;
				}
			}
			if (i >= tempLen) {
				return 0;
			}
		}
	}
	return i;
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
	size_t tempLen = strLen();
	if (getChar(currentPos) == 't' || getChar(currentPos) == 'T') {
		if (tempLen > currentPos + 3
			&& ((getChar(currentPos + 1)) == 'r' || (getChar(currentPos + 1)) == 'R')
			&& ((getChar(currentPos + 2)) == 'u' || (getChar(currentPos + 2)) == 'U')
			&& ((getChar(currentPos + 3)) == 'e' || (getChar(currentPos + 3)) == 'E')) {
			return true;
		}
	} else if (getChar(currentPos) == 'f' || getChar(currentPos) == 'F') {
		if (tempLen > currentPos + 4
			&& ((getChar(currentPos + 1)) == 'a' || (getChar(currentPos + 1)) == 'A')
			&& ((getChar(currentPos + 2)) == 'l' || (getChar(currentPos + 2)) == 'L')
			&& ((getChar(currentPos + 3)) == 's' || (getChar(currentPos + 3)) == 'S')
			&& ((getChar(currentPos + 4)) == 'e' || (getChar(currentPos + 4)) == 'E')) {
			return true;
		}
	}
	return false;
}

bool JsonParser::DeSerialization::isNull(const size_t &currentPos) const
{
	if ((getChar(currentPos)) == 'n' || (getChar(currentPos)) == 'N') {
		if (strLen() > currentPos + 3 && (getChar(currentPos + 1)) == 'u'
			&& (getChar(currentPos + 2)) == 'l'
			&& (getChar(currentPos + 3)) == 'l') {
			return true;
		}
	}
	return false;
}


size_t JsonParser::DeSerialization::addStringValue(
	const size_t &currentPos, const JsonString &name)
{
	size_t i = currentPos;
	JsonString value;
	if ((i = this->getName(i, &value)) <= 0) {
		return 0;
	}
	this->kvPairStrings()->operator[](name) = value;
	return i;
}


size_t JsonParser::DeSerialization::addObjectValue(
	const size_t &currentPos, const JsonString &name)
{
	size_t i = currentPos;
	size_t tempLen = strLen();
	if (i > 0 && tempLen > 0 && tempLen > i && matchChar(i, JsonObjectOpen)) {
		auto child = std::make_unique<JsonParser::DeSerialization>();
		child->setFullString(this->fullString());
		if ((i = child->fromString(i)) <= 0) {
			return 0;
		}
		if (this->kvPairObjects()->find(name) == this->kvPairObjects()->end()) {
			this->kvPairObjects()->operator[](name) = std::move(child);
		}
	}
	return i;
}


size_t JsonParser::DeSerialization::addArrayValue(
	const size_t &currentPos, const JsonString &name)
{
	size_t pos = currentPos;
	size_t tempLen = strLen();
	if (pos > 0 && tempLen > 0 && tempLen > pos && matchChar(pos, JsonArrayOpen)) {
		auto child = std::make_unique<JsonParser::DeSerialization>();
		child->setFullString(this->fullString());
		if ((pos = child->fromStringArray(pos)) <= 0) {
			return 0;
		}
		if (this->kvPairArrays()->find(name) == this->kvPairArrays()->end()) {
			this->kvPairArrays()->operator[](name) = std::move(child);
		}
	}
	return pos;
}


size_t JsonParser::DeSerialization::addNumberValue(
	const size_t &currentPos, const JsonString & name)
{
	size_t i = 0;
	size_t tempLen = strLen();
	if (currentPos > 0 && tempLen > 0 && tempLen > currentPos) {
		for (i = currentPos; i < tempLen; ++i) {
			if (!isNumber(i)) {
				if (this->kvPairNumbers()->find(name) == this->kvPairNumbers()->end()) {
					JsonParser::Number number(substr(currentPos, i - currentPos));
					this->kvPairNumbers()->operator[](name) = std::move(number);
				}
				return i;
			}
		}
	}
	return i;
}


size_t JsonParser::DeSerialization::addBoolValue(
	const size_t &currentPos, const JsonString & name)
{
	if ((getChar(currentPos)) == 't') {
		this->kvPairBools()->operator[](name) = true;
		return currentPos + constLength("True");
	} else if ((getChar(currentPos)) == 'f') {
		this->kvPairBools()->operator[](name) = false;
		return currentPos + constLength("False");
	}
	return 0;
}

size_t JsonParser::DeSerialization::addNullValue(
	const size_t & currentPos, const JsonString & name)
{
	this->kvPairNullValues()->push_back(name);
	return currentPos + constLength("null");
}


size_t JsonParser::DeSerialization::addArrayToArray(const size_t &currentPos)
{
	size_t i = currentPos;
	size_t tempLen = strLen();
	if (i > 0 && tempLen > 0 && tempLen > i && getChar(i) == JsonObjectOpen) {
		auto child = std::make_unique<JsonParser::DeSerialization>();
		child->setFullString(this->fullString());
		if ((i = child->fromStringArray(i)) <= 0) {
			return 0;
		}
		this->arrayArrays()->push_back(std::move(child));
	}
	return i;
}


size_t JsonParser::DeSerialization::addStringToArray(const size_t &currentPos)
{
	size_t i = currentPos;
	JsonString value;
	if ((i = this->getName(i + 1, &value)) <= 0) {
		return 0;
	}
	this->arrayStrings()->push_back(std::move(value));
	return i;
}


size_t JsonParser::DeSerialization::addObjectToArray(const size_t &currentPos)
{
	size_t i = currentPos;
	size_t tempLen = strLen();
	if (i > 0 && tempLen > 0 && tempLen > i && getChar(i) == JsonObjectOpen) {
		auto child = std::make_unique<JsonParser::DeSerialization>();
		child->setFullString(this->fullString());
		if ((i = child->fromString(i)) <= 0) {
			return 0;
		}
		this->arrayObjects()->push_back(std::move(child));
	}
	return i;
}


size_t JsonParser::DeSerialization::addNumberToArray(const size_t &currentPos)
{
	size_t i = 0;
	size_t tempLen = strLen();
	for (i = currentPos; i < tempLen; ++i) {
		if (!isNumber(i)) {
			JsonParser::Number number(substr(currentPos, i - currentPos));
			this->arrayNumbers()->push_back(std::move(number));
			return i;
		}
	}
	return i;
}

size_t JsonParser::DeSerialization::addBoolToArray(const size_t &currentPos)
{
	if ((getChar(currentPos)) == 't') {
		this->arrayBools()->push_back(true);
		return currentPos + constLength("True");
	} else if ((getChar(currentPos)) == 'f') {
		this->arrayBools()->push_back(false);
		return currentPos + constLength("False");
	}
	return 0;
}
