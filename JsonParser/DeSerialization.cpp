#include "DeSerialization.h"
#include <stdio.h>
#include <ctype.h>
#include <iostream>
#include <memory>

bool JsonParser::DeSerialization::fromString()
{
	if (strLen() > 0) {
		this->clearAll();
		if (parseString()) {
			return true;
		} else {
			return false;
		}
	}
	return true;
}

bool JsonParser::DeSerialization::fromString(
	const std::shared_ptr<std::string> &str)
{
	this->setFullString(str.get());
	return this->fromString();
}

size_t JsonParser::DeSerialization::fromString(const size_t &currentPos)
{
	size_t i = 0;
	size_t openingCount = 0;
	size_t closeCount = 0;
	size_t tempLen = strLen();
	for (i = currentPos; i < tempLen; ++i) {
		if (*getChar(i) == '{') {
			openingCount++;
		} else if (*getChar(i) == '}') {
			closeCount++;
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
		if (*getChar(i) == '[') {
			openingCount++;
			if ((i = parseStringArray(i + 1)) <= 0) {
				return 0;
			}
			--i;
		} else if (*getChar(i) == ']') {
			closeCount++;
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
		&& *getChar(currentPos - 1) == '[') {
		for (i = currentPos; i < tempLen; ++i) {
			switch (*getChar(i)) {
			case '{':
			{
				if ((i = addObject(i)) <= 0 || !commaFound) {
					return 0;
				}
				setType(JsonTypes::ObjectArray);
				commaFound = false;
			}
			break;
			case '[':
			{
				if ((i = addArray(i)) <= 0 || !commaFound) {
					return 0;
				}
				setType(JsonTypes::ArrayArray);
				commaFound = false;
			}
			break;
			case '"':
			{
				if ((i = addString(i)) <= 0 || !commaFound) {
					return 0;
				}
				setType(JsonTypes::StringArray);
				commaFound = false;
			}
			break;
			case ',':
				commaFound = true;
				break;
			case ']':
				if (commaFound == false) {
					return i;
				}
				break;
			default:
				if (isNumber(i)) {
					if ((i = addInteger(i)) <= 0 || !commaFound) {
						return 0;
					}
					--i;
					setType(JsonTypes::NumberArray);
					commaFound = false;
				} else if (isBool(i)) {
					if ((i = addBool(i)) <= 0 || !commaFound) {
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


std::string JsonParser::DeSerialization::toString() const
{
	return std::string();
}

std::string JsonParser::DeSerialization::toStringArray() const
{
	return std::string();
}


bool JsonParser::DeSerialization::parseString()
{
	size_t openingCount = 0;
	size_t closeCount = 0;
	size_t tempLen = strLen();
	for (size_t i = 0; i < tempLen; ++i) {
		if (matchChar(i, '{')) {
			++openingCount;
		} else if (matchChar(i, '}')) {
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
		std::string name;
		for (i = currentPos; i < tempLen; ++i) {
			switch (*getChar(i)) {
			case '"':
				if ((i = this->getName(i + 1, name)) <= 0) {
					return 0;
				}
				break;
			case ':':
				if ((i = addValue(i + 1, name)) <= 0) {
					name = std::string();
					return 0;
				}
				--i;
				name = std::string();
				break;
			case ']':
			case '}':
			// case ',':
				return i;
			}
		}
	}
	return i;
}


size_t JsonParser::DeSerialization::getName(
	const size_t &currentPos, std::string &name) const
{
	size_t i = 0;
	size_t tempLen = strLen();
	if (currentPos > 0 && tempLen > 0 && tempLen > currentPos) {
		for (i = currentPos; i < tempLen; ++i) {
			if (matchChar(i, '"') && !checkEscape(i)) {
				return i;
			}
			name += *getChar(i);
		}
	}
	return 0;
}


size_t JsonParser::DeSerialization::addValue(
	const size_t &currentPos, const std::string &name)
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
			switch (*getChar(i)) {
			case '"':
				if ((i = addStringValue(i + 1, name)) <= 0 || commaFound == false) {
					return 0;
				}
				valueSet = true;
				commaFound = false;
				break;
			case '{':
				if ((i = addObjectValue(i, name)) <= 0 || commaFound == false) {
					return 0;
				}
				valueSet = true;
				commaFound = false;
				break;
			case '[':
				if ((i = addArrayValue(i, name)) <= 0 || commaFound == false) {
					return 0;
				}
				valueSet = true;
				commaFound = false;
				break;
			case ',':
				if (valueSet && !commaFound) {
					return i;
				}
				commaFound = true;
				break;
			case '}':
				if (commaFound == false) {
					return i;
				}
				break;
			default:
				if (isNumber(i)) {
					if ((i = addIntegerValue(i, name)) <= 0 || commaFound == false) {
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
	if ((*getChar(currentPos) >= 48
		&& *getChar(currentPos) <= 57)
		|| matchChar(currentPos, '.')
		|| matchChar(currentPos, '-')) {
		return true;
	}
	return false;
}


bool JsonParser::DeSerialization::isBool(const size_t &currentPos) const
{
	size_t tempLen = strLen();
	if (tolower(*getChar(currentPos)) == 't') {
		if (tempLen > currentPos + 3 && tolower(*getChar(currentPos + 1)) == 'r'
			&& tolower(*getChar(currentPos + 2)) == 'u'
			&& tolower(*getChar(currentPos + 3)) == 'e') {
			return true;
		}
	} else if (tolower(*getChar(currentPos)) == 'f') {
		if (tempLen > currentPos + 4 && tolower(*getChar(currentPos + 1)) == 'a'
			&& tolower(*getChar(currentPos + 2)) == 'l'
			&& tolower(*getChar(currentPos + 3)) == 's'
			&& tolower(*getChar(currentPos + 4)) == 'e') {
			return true;
		}
	}
	return false;
}

bool JsonParser::DeSerialization::isNull(const size_t &currentPos) const
{
	if (tolower(*getChar(currentPos)) == 'n') {
		if (strLen() > currentPos + 3 && tolower(*getChar(currentPos + 1)) == 'u'
			&& tolower(*getChar(currentPos + 2)) == 'l'
			&& tolower(*getChar(currentPos + 3)) == 'l') {
			return true;
		}
	}
	return false;
}


size_t JsonParser::DeSerialization::addStringValue(
	const size_t &currentPos, const std::string &name)
{
	size_t i = currentPos;
	std::string value;
	if ((i = this->getName(i, value)) <= 0) {
		return 0;
	}
	this->m_kvPairStrings[name] = value;
	return i;
}


size_t JsonParser::DeSerialization::addObjectValue(
	const size_t &currentPos, const std::string &name)
{
	size_t i = currentPos;
	size_t tempLen = strLen();
	if (i > 0 && tempLen > 0 && tempLen > i && matchChar(i, '{')) {
		auto child = std::make_unique<JsonParser::DeSerialization>();
		child->setFullString(this->fullString());
		if ((i = child->fromString(i)) <= 0) {
			return 0;
		}
		if (m_kvPairObjects.find(name) == m_kvPairObjects.end()) {
			m_kvPairObjects[name] = std::move(child);
		}
	}
	return i;
}


size_t JsonParser::DeSerialization::addArrayValue(
	const size_t &currentPos, const std::string &name)
{
	size_t pos = currentPos;
	size_t tempLen = strLen();
	if (pos > 0 && tempLen > 0 && tempLen > pos && matchChar(pos, '[')) {
		auto child = std::make_unique<JsonParser::DeSerialization>();
		child->setFullString(this->fullString());
		if ((pos = child->fromStringArray(pos)) <= 0) {
			return 0;
		}
		if (m_kvPairArrays.find(name) == m_kvPairArrays.end()) {
			m_kvPairArrays[name] = std::move(child);
		}
	}
	return pos;
}


size_t JsonParser::DeSerialization::addIntegerValue(
	const size_t &currentPos, const std::string & name)
{
	size_t i = 0;
	std::string numberStr;
	size_t tempLen = strLen();
	if (currentPos > 0 && tempLen > 0 && tempLen > currentPos) {
		for (i = currentPos; i < tempLen; ++i) {
			if (isNumber(i)) {
				numberStr += *getChar(i);
			} else {
				if (m_kvPairNumbers.find(name) == m_kvPairNumbers.end()) {
					JsonParser::Number number(numberStr);
					m_kvPairNumbers[name] = number;
				}
				return i;
			}
		}
	}
	return i;
}


size_t JsonParser::DeSerialization::addBoolValue(
	const size_t &currentPos, const std::string & name)
{
	if (tolower(*getChar(currentPos)) == 't') {
		m_kvPairBools[name] = true;
		return currentPos + constLength("True");
	} else if (tolower(*getChar(currentPos)) == 'f') {
		m_kvPairBools[name] = false;
		return currentPos + constLength("False");
	}
	return 0;
}

size_t JsonParser::DeSerialization::addNullValue(
	const size_t & currentPos, const std::string & name)
{
	m_kvPairNullValues.push_back(name);
	return currentPos + constLength("null");
}


size_t JsonParser::DeSerialization::addArray(const size_t &currentPos)
{
	size_t i = currentPos;
	size_t tempLen = strLen();
	if (i > 0 && tempLen > 0 && tempLen > i && *getChar(i) == '{') {
		auto child = std::make_unique<JsonParser::DeSerialization>();
		child->setFullString(this->fullString());
		if ((i = child->fromString(i)) <= 0) {
			return 0;
		}
		m_arrayArrays.push_back(std::move(child));
	}
	return i;
}


size_t JsonParser::DeSerialization::addString(const size_t &currentPos)
{
	size_t i = currentPos;
	std::string value;
	if ((i = this->getName(i + 1, value)) <= 0) {
		return 0;
	}
	m_arrayStrings.push_back(value);
	return i;
}


size_t JsonParser::DeSerialization::addObject(const size_t &currentPos)
{
	size_t i = currentPos;
	size_t tempLen = strLen();
	if (i > 0 && tempLen > 0 && tempLen > i && *getChar(i) == '{') {
		auto child = std::make_unique<JsonParser::DeSerialization>();
		child->setFullString(this->fullString());
		if ((i = child->fromString(i)) <= 0) {
			return 0;
		}
		m_arrayObjects.push_back(std::move(child));
	}
	return i;
}


size_t JsonParser::DeSerialization::addInteger(const size_t &currentPos)
{
	size_t i = 0;
	std::string numberStr = "";
	size_t tempLen = strLen();
	for (i = currentPos; i < tempLen; ++i) {
		if (isNumber(i)) {
			numberStr += *getChar(i);
		} else {
			JsonParser::Number number(numberStr);
			m_arrayNumbers.push_back(number);
			return i;
		}
	}
	return i;
}

size_t JsonParser::DeSerialization::addBool(const size_t &currentPos)
{
	if (tolower(*getChar(currentPos)) == 't') {
		m_arrayBools.push_back(true);
		return currentPos + constLength("True");
	} else if (tolower(*getChar(currentPos)) == 'f') {
		m_arrayBools.push_back(false);
		return currentPos + constLength("False");
	}
	return 0;
}