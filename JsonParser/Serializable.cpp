#ifndef JSONPARSER_SERIALIZABLE_CPP_
#define JSONPARSER_SERIALIZABLE_CPP_

#include "Serializable.h"
#include <stdio.h>
#include <ctype.h>
#include <iostream>

bool JsonParser::Serializable::fromString()
{
	if (strLen() > 0) {
		if (parseString()) {
			return true;
		} else {
			return false;
		}
	}
	return true;
}

bool JsonParser::Serializable::fromString(std::shared_ptr<std::string> str)
{
	/*if (this->fullString() != nullptr) {
		delete this->fullString();
	}*/
	this->setFullString(str);
	return this->fromString();
}

size_t JsonParser::Serializable::fromString(const size_t &pos)
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

size_t JsonParser::Serializable::fromStringArray(const size_t &pos)
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


size_t JsonParser::Serializable::parseStringArray(const size_t &pos)
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


JsonParser::Serializable::Serializable()
{
}


JsonParser::Serializable::~Serializable()
{
}


std::string JsonParser::Serializable::toString() const
{
	return std::string();
}

std::string JsonParser::Serializable::toStringArray() const
{
	return std::string();
}


bool JsonParser::Serializable::parseString()
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


inline bool JsonParser::Serializable::matchChar(char ch1, char ch2) const
{
	return ch1 == ch2;
}


bool JsonParser::Serializable::checkEscape(const size_t &pos) const
{
	return pos > 0 && matchChar(getChar(pos - 1), '\\');
}


size_t JsonParser::Serializable::addKVPair(const size_t &pos)
{
	size_t i = 0;
	if (pos > 0 && strLen() > 0 && strLen() > pos) {
		std::string name;
		for (i = pos; i < strLen(); i++) {
			switch (getChar(i)) {
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


size_t JsonParser::Serializable::getName(const size_t &pos, std::string &name) const
{
	size_t i = 0;
	if (pos > 0 && strLen() > 0 && strLen() > pos) {
		for (i = pos; i < strLen(); i++) {
			if (getChar(i) == '"' && !checkEscape(i)) {
				return i;
			}
			name += getChar(i);
		}
	}
	return 0;
}


size_t JsonParser::Serializable::addValue(const size_t &pos, const std::string &name)
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
				} else if (isNull(i)) {
					if ((i = addNullValue(i, name)) <= 0 || commaFound == false) {
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


bool JsonParser::Serializable::isNumber(const size_t &pos) const
{
	if ((getChar(pos) >= 48
		&& getChar(pos) <= 57)
		|| getChar(pos) == '.'
		|| getChar(pos) == '-') {
		return true;
	}
	return false;
}


bool JsonParser::Serializable::isBool(const size_t &pos) const
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

bool JsonParser::Serializable::isNull(const size_t &pos) const
{
	if (tolower(getChar(pos)) == 'n') {
		if (strLen() > pos + 3 && tolower(getChar(pos + 1)) == 'u'
			&& tolower(getChar(pos + 2)) == 'l'
			&& tolower(getChar(pos + 3)) == 'l') {
			return true;
		}
	}
	return false;
}


size_t JsonParser::Serializable::addStringValue(const size_t &pos, const std::string &name)
{
	size_t i = pos;
	std::string value;
	if ((i = this->getName(i, value)) <= 0) {
		return 0;
	}
	this->m_kvPairStrings[name] = value;
	return i;
}


size_t JsonParser::Serializable::addObjectValue(const size_t &pos, const std::string &name)
{
	if (name.length() <= 0) {
		return 0;
	}
	size_t i = pos;
	if (pos > 0 && strLen() > 0 && strLen() > pos && getChar(pos) == '{') {
		auto child = std::make_shared<JsonParser::Serializable>();
		child->setFullString(this->fullString());
		if ((i = child->fromString(i)) <= 0) {
			return 0;
		}
		if (m_kvPairObjects.find(name) == m_kvPairObjects.end()) {
			m_kvPairObjects[name] = child;
		}
	}
	return i;
}


size_t JsonParser::Serializable::addArrayValue(const size_t &pos, const std::string &name)
{
	if (name.length() <= 0) {
		return 0;
	}
	size_t pos_ = pos;
	if (pos_ > 0 && strLen() > 0 && strLen() > pos_ && getChar(pos_) == '[') {
		auto child = std::make_shared<JsonParser::Serializable>();
		child->setFullString(this->fullString());
		if ((pos_ = child->fromStringArray(pos_)) <= 0) {
			return 0;
		}
		if (m_kvPairArrays.find(name) == m_kvPairArrays.end()) {
			m_kvPairArrays[name] = child;
		}
	}
	return pos_;
}


size_t JsonParser::Serializable::addIntegerValue(const size_t &pos, const std::string & name)
{
	if (name.length() <= 0) {
		return 0;
	}
	size_t i = 0;
	std::string numberStr;
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


size_t JsonParser::Serializable::addBoolValue(const size_t &pos, const std::string & name)
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

size_t JsonParser::Serializable::addNullValue(const size_t & pos, const std::string & name)
{
	if (tolower(getChar(pos)) == 'n') {
		m_kvPairNullValues.push_back(name);
		return pos + constLength("null");
	}
	return 0;
}


size_t JsonParser::Serializable::addArray(const size_t &pos)
{
	size_t i = pos;
	if (pos > 0 && strLen() > 0 && strLen() > pos && getChar(pos) == '{') {
		auto child = std::make_shared<JsonParser::Serializable>();
		child->setFullString(this->fullString());
		if ((i = child->fromString(i)) <= 0) {
			return 0;
		}
		m_arrayArrays.push_back(child);
	}
	return i;
}


size_t JsonParser::Serializable::addString(const size_t &pos)
{
	size_t i = pos;
	std::string value;
	if ((i = this->getName(i + 1, value)) <= 0) {
		return 0;
	}
	m_arrayStrings.push_back(value);
	return i;
}


size_t JsonParser::Serializable::addObject(const size_t &pos)
{
	size_t i = pos;
	if (pos > 0 && strLen() > 0 && strLen() > pos && getChar(pos) == '{') {
		auto child = std::make_shared<JsonParser::Serializable>();
		child->setFullString(this->fullString());
		if ((i = child->fromString(i)) <= 0) {
			return 0;
		}
		m_arrayObjects.push_back(child);
	}
	return i;
}


size_t JsonParser::Serializable::addInteger(const size_t &pos)
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

size_t JsonParser::Serializable::addBool(const size_t &pos)
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

#endif  // JSONPARSER_SERIALIZABLE_CPP_
