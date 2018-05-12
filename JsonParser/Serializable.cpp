#ifndef JSONPARSER_SERIALIZABLE_CPP_
#define JSONPARSER_SERIALIZABLE_CPP_

#include "Serializable.h"
#include <stdio.h>
#include <ctype.h>
#include <iostream>

bool JsonParser::Serializable::fromString()
{
	if (m_isParsed == true) {
		return true;
	}
	if (strLen() > 0) {
		if (checkString()) {
			std::cout << "successfully parsed" << std::endl;
			this->m_isParsed = true;
			return true;
		} else {
			std::cout << "parsing failed..." << std::endl;
			this->m_isParsed = false;
			return false;
		}
	}
	return true;
}

bool JsonParser::Serializable::fromString(const std::string & str)
{
	this->m_fullString = new std::string(str);
	return fromString();
}

size_t JsonParser::Serializable::fromString(size_t pos)
{
	size_t i = 0;
	size_t openingCount = 0;
	size_t closeCount = 0;
	for (i = pos; i < strLen(); i++) {
		if (getChar(i) == '{' && !checkEscape(i)) {
			openingCount++;
		} else if (getChar(i) == '}' && !checkEscape(i)) {
			closeCount++;
		} else {
			if ((i = addKVPair(i)) <= 0) {
				return 0;
			}
			i--;
		}
		if (openingCount == closeCount) {
			break;
		}
	}
	return i;
}

size_t JsonParser::Serializable::fromStringArray(size_t pos)
{
	size_t i = 0;
	size_t openingCount = 0;
	size_t closeCount = 0;
	for (i = pos; i < strLen(); i++) {
		if (getChar(i) == '[' && !checkEscape(i)) {
			openingCount++;
			if ((i = parseStringArray(i + 1)) <= 0) {
				return 0;
			}
			i--;
		} else if (getChar(i) == ']' && !checkEscape(i)) {
			closeCount++;
		}
		if (openingCount == closeCount) {
			break;
		}
	}
	return i;
}


size_t JsonParser::Serializable::parseStringArray(size_t pos)
{
	size_t i = 0;
	bool commaFound = true;
	if (pos > 0 && strLen() > 0 && strLen() > pos && getChar(pos - 1) == '[') {
		for (i = pos; i < strLen(); i++) {
			switch (getChar(i)) {
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
					i--;
					setType(JsonTypes::NumberArray);
					commaFound = false;
				} else if (isBool(i)) {
					if ((i = addBool(i)) <= 0 || !commaFound) {
						return 0;
					}
					setType(JsonTypes::BoolArray);
					i--;
					commaFound = false;
				}
			}
		}
	}
	return i;
}


JsonParser::Serializable::Serializable(JsonParser::Serializable* parent) : m_parent(parent)
{
}


JsonParser::Serializable::~Serializable()
{
	if (this->m_parent == nullptr && this->m_fullString != nullptr) {
		delete this->m_fullString;
	}
}


std::string JsonParser::Serializable::toString() const
{
	return std::string();
}


bool JsonParser::Serializable::checkString()
{
	size_t openingCount = 0;
	size_t closeCount = 0;
	for (size_t i = 0; i < strLen(); i++) {
		if (getChar(i) == '{' && !checkEscape(i)) {
			openingCount++;
		} else if (getChar(i) == '}' && !checkEscape(i)) {
			closeCount++;
		} else {
			if ((i = addKVPair(i)) <= 0) {
				return false;
			}
			i--;
		}
	}
	return openingCount == closeCount;
}


bool JsonParser::Serializable::matchChar(char ch1, char ch2)
{
	return ch1 == ch2;
}


bool JsonParser::Serializable::checkEscape(size_t pos)
{
	return pos > 0 && matchChar(getChar(pos - 1), '\\');
}


size_t JsonParser::Serializable::addKVPair(size_t pos)
{
	size_t i = 0;
	if (pos > 0 && strLen() > 0 && strLen() > pos) {
		std::string name;
		for (i = pos; i < strLen(); i++) {
			switch (getChar(i)) {
			case '"':
				if ((i = addName(i + 1, &name)) <= 0) {
					return 0;
				}
				break;
			case ':':
				if ((i = addValue(i + 1, name)) <= 0) {
					name = std::string();
					return 0;
				}
				i--;
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


size_t JsonParser::Serializable::addName(size_t pos, std::string *name)
{
	size_t i = 0;
	if (pos > 0 && strLen() > 0 && strLen() > pos) {
		for (i = pos; i < strLen(); i++) {
			if (getChar(i) == '"' && !checkEscape(i)) {
				return i;
			}
			(*name) += getChar(i);
		}
	}
	return 0;
}


size_t JsonParser::Serializable::addValue(size_t pos, const std::string &name)
{
	if (name.length() <= 0) {
		return 0;
	}
	size_t i = 0;
	bool commaFound = true;
	bool valueSet = false;
	if (pos > 0 && strLen() > 0 && strLen() > pos) {
		for (i = pos; i < strLen(); i++) {
			switch (getChar(i)) {
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
					i--;
					valueSet = true;
					commaFound = false;
				} else if (isBool(i)) {
					if ((i = addBoolValue(i, name)) <= 0 || commaFound == false) {
						return 0;
					}
					i--;
					valueSet = true;
					commaFound = false;
				}
			}
			if (i >= strLen()) {
				return 0;
			}
		}
	}
	return i;
}


bool JsonParser::Serializable::isNumber(size_t pos)
{
	if ((getChar(pos) >= 48
		&& getChar(pos) <= 57)
		|| getChar(pos) == '.'
		|| getChar(pos) == '-') {
		return true;
	}
	return false;
}


bool JsonParser::Serializable::isBool(size_t pos)
{
	if (tolower(getChar(pos)) == 't') {
		if (strLen() > pos + 3 && tolower(getChar(pos + 1)) == 'r'
			&& tolower(getChar(pos + 2)) == 'u'
			&& tolower(getChar(pos + 3)) == 'e') {
			return true;
		}
	} else if (tolower(getChar(pos)) == 'f') {
		if (strLen() > pos + 4 && tolower(getChar(pos + 1)) == 'a'
			&& tolower(getChar(pos + 2)) == 'l'
			&& tolower(getChar(pos + 3)) == 's'
			&& tolower(getChar(pos + 4)) == 'e') {
			return true;
		}
	}
	return false;
}


size_t JsonParser::Serializable::strLen() const
{
	if (this->m_fullString != nullptr) {
		return this->m_fullString->length();
	}
	return 0;
}


char JsonParser::Serializable::getChar(size_t pos) const
{
	if (this->m_fullString != nullptr) {
		return this->m_fullString->at(pos);
	}
	return 0;
}


size_t JsonParser::Serializable::addStringValue(size_t pos, const std::string &name)
{
	size_t i = pos;
	std::string value = std::string();
	if ((i = addName(i, &value)) <= 0) {
		return 0;
	}
	this->m_kvPairStrings[name] = value;
	return i;
}


size_t JsonParser::Serializable::addObjectValue(size_t pos, const std::string &name)
{
	if (name.length() <= 0) {
		return 0;
	}
	size_t i = pos;
	if (pos > 0 && strLen() > 0 && strLen() > pos && getChar(pos) == '{') {
		JsonParser::Serializable *child = new JsonParser::Serializable(this);
		child->m_fullString = this->m_fullString;
		m_kvPairObjects[name] = child;
		if ((i = child->fromString(i)) <= 0) {
			return 0;
		}
	}
	return i;
}


size_t JsonParser::Serializable::addArrayValue(size_t pos, const std::string &name)
{
	if (name.length() <= 0) {
		return 0;
	}
	if (pos > 0 && strLen() > 0 && strLen() > pos && getChar(pos) == '[') {
		JsonParser::Serializable *child = new JsonParser::Serializable(this);
		child->m_fullString = this->m_fullString;
		m_kvPairArrays[name] = child;
		if ((pos = child->fromStringArray(pos)) <= 0) {
			return 0;
		}
	}
	return pos;
}


size_t JsonParser::Serializable::addIntegerValue(size_t pos, const std::string & name)
{
	if (name.length() <= 0) {
		return 0;
	}
	size_t i = 0;
	std::string numberStr = std::string();
	if (pos > 0 && strLen() > 0 && strLen() > pos) {
		for (i = pos; i < strLen(); i++) {
			if (isNumber(i)) {
				numberStr += getChar(i);
			} else if (getChar(i) == ' '
				|| getChar(i) == '\t'
				|| getChar(i) == ','
				|| getChar(i) == 'L'
				|| getChar(i) == 'l') {
				JsonParser::Number number(numberStr);
				m_kvPairNumbers[name] = number;
				return i;
			} else {
				return 0;
			}
		}
	}
	return i;
}


size_t JsonParser::Serializable::addBoolValue(size_t pos, const std::string & name)
{
	if (tolower(getChar(pos)) == 't') {
		m_kvPairBools[name] = true;
		return pos + constLength("True");
	} else if (tolower(getChar(pos)) == 'f') {
		m_kvPairBools[name] = false;
		return pos + constLength("False");
	}
	return 0;
}


size_t JsonParser::Serializable::addArray(size_t pos)
{
	size_t i = pos;
	if (pos > 0 && strLen() > 0 && strLen() > pos && getChar(pos) == '{') {
		JsonParser::Serializable *child = new JsonParser::Serializable(this);
		child->m_fullString = this->m_fullString;
		if ((i = child->fromStringArray(i)) <= 0) {
			return 0;
		}
		m_arrayArrays.push_back(child);
	}
	return i;
}


size_t JsonParser::Serializable::addString(size_t pos)
{
	size_t i = pos;
	std::string value = std::string();
	if ((i = addName(i + 1, &value)) <= 0) {
		return 0;
	}
	m_arrayStrings.push_back(value);
	return i;
}


size_t JsonParser::Serializable::addObject(size_t pos)
{
	size_t i = pos;
	if (pos > 0 && strLen() > 0 && strLen() > pos && getChar(pos) == '{') {
		JsonParser::Serializable *child = new JsonParser::Serializable(this);
		child->m_fullString = this->m_fullString;
		m_arrayObjects.push_back(child);
		if ((i = child->fromString(i)) <= 0) {
			return 0;
		}
	}
	return i;
}


size_t JsonParser::Serializable::addInteger(size_t pos)
{
	size_t i = 0;
	std::string numberStr = "";
	for (i = pos; i < strLen(); i++) {
		if (isNumber(i)) {
			numberStr += getChar(i);
		} else if (getChar(i) == ' '
			|| getChar(i) == '\t'
			|| getChar(i) == ','
			|| getChar(i) == 'L'
			|| getChar(i) == 'l') {
			JsonParser::Number number(numberStr);
			m_arrayNumbers.push_back(number);
			return i;
		} else {
			return 0;
		}
	}
	return i;
}

size_t JsonParser::Serializable::addBool(size_t pos)
{
	if (tolower(getChar(pos)) == 't') {
		m_arrayBools.push_back(true);
		return pos + constLength("True");
	} else if (tolower(getChar(pos)) == 'f') {
		m_arrayBools.push_back(false);
		return pos + constLength("False");
	}
	return 0;
}

void JsonParser::Serializable::setType(const JsonTypes & type)
{
	this->m_type = type;
}

#endif  // JSONPARSER_SERIALIZABLE_CPP_
