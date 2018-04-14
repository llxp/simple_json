#include "stdafx.h"
#include "Serializable.h"
#include <stdio.h>
#include <ctype.h>
#include <iostream>

Serializable::Serializable(Serializable * parent) : m_parent(parent), m_fullString("")
{
}

Serializable::Serializable(const std::string & str, Serializable * parent) : m_parent(parent), m_fullString(str)
{
}

std::string Serializable::getValue(const std::string & name) const
{
	if (m_kvPairStrings.count(name) > 0) {
		return m_kvPairStrings.at(name);
	}
	return "";
}

void Serializable::setValue(const std::string & name, const std::string & value)
{
	m_kvPairStrings[name] = value;
}

bool Serializable::fromString()
{
	if (m_fullString.length() > 0) {
		if (checkString()) {
			std::cout << "successfully parsed" << std::endl;
			return true;
		}
		else {
			std::cout << "parsing failed..." << std::endl;
		}
	}
	return false;
}

size_t Serializable::fromString(size_t pos)
{
	size_t i = 0;
	size_t openingCount = 0;
	size_t closeCount = 0;
	for (i = pos; i < m_fullString.length(); i++) {
		if (m_fullString[i] == '{' && !checkEscape(i)) {
			openingCount++;
		}
		else if (m_fullString[i] == '}' && !checkEscape(i)) {
			closeCount++;
		}
		else {
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

size_t Serializable::fromStringArray(size_t pos)
{
	size_t i = 0;
	size_t openingCount = 0;
	size_t closeCount = 0;
	for (i = pos; i < m_fullString.length(); i++) {
		if (m_fullString[i] == '[' && !checkEscape(i)) {
			openingCount++;
			if ((i = parseStringArray(i + 1)) <= 0) {
				return 0;
			}
			i--;
		}
		else if (m_fullString[i] == ']' && !checkEscape(i)) {
			closeCount++;
		}
		if (openingCount == closeCount) {
			break;
		}
	}
	return i;
}

size_t Serializable::parseStringArray(size_t pos)
{
	size_t i = 0;
	bool commaFound = true;
	if (pos > 0 && m_fullString.length() > 0 && m_fullString.length() > pos && m_fullString[pos - 1] == '[') {
		for (i = pos; i < m_fullString.length(); i++) {
			switch (m_fullString[i]) {
			case '{':
				if ((i = addObject(i)) <= 0 || !commaFound) {
					return 0;
				}
				commaFound = false;
				break;
			case '[':
				if ((i = addArray(i)) <= 0 || !commaFound) {
					return 0;
				}
				commaFound = false;
				break;
			case '"':
				if ((i = addString(i)) <= 0 || !commaFound) {
					return 0;
				}
				commaFound = false;
				break;
			case ',':
				commaFound = true;
				break;
			case ']':
				if (commaFound == false) {
					return i;
				}
				break;
			}
		}
	}
	return i;
}

std::string Serializable::toString() const
{
	return "";
}

bool Serializable::checkString()
{
	size_t openingCount = 0;
	size_t closeCount = 0;
	for (size_t i = 0; i < m_fullString.length();i++) {
		if (m_fullString[i] == '{' && !checkEscape(i)) {
			openingCount++;
		}
		else if(m_fullString[i] == '}' && !checkEscape(i)) {
			closeCount++;
		}
		else {
			if ((i = addKVPair(i)) <= 0) {
				return false;
			}
			i--;
		}
	}
	return openingCount == closeCount;
}

bool Serializable::matchChar(char ch1, char ch2)
{
	return ch1 == ch2;
}

bool Serializable::checkEscape(size_t pos)
{
	return pos > 0 && matchChar(m_fullString[pos - 1], '\\');
}

size_t Serializable::addKVPair(size_t pos)
{
	size_t i = 0;
	if (pos > 0 && m_fullString.length() > 0 && m_fullString.length() > pos) {
		std::string name = "";
		for (i = pos; i < m_fullString.length(); i++) {
			switch (m_fullString[i]) {
			case '"':
				if ((i = addName(i + 1, name)) <= 0) {
					return 0;
				}
				if (name == "nextLink") {
					std::cout << "..." << std::endl;
				}
				break;
			case ':':
				if ((i = addValue(i + 1, name)) <= 0) {
					name = "";
					return 0;
				}
				i--;
				name = "";
				break;
			case ']':
			case '}':
			//case ',':
				return i;
			}
		}
	}
	return i;
}

size_t Serializable::addName(size_t pos, std::string &name)
{
	size_t i = 0;
	if (pos > 0 && m_fullString.length() > 0 && m_fullString.length() > pos) {
		for (i = pos; i < m_fullString.length(); i++) {
			if (m_fullString[i] == '"' && !checkEscape(i)) {
				return i;
			}
			name += m_fullString[i];
		}
	}
	return 0;
}

size_t Serializable::addValue(size_t pos, const std::string &name)
{
	if (name.length() <= 0) {
		return 0;
	}
	size_t i = 0;
	bool commaFound = true;
	bool valueSet = false;
	if (pos > 0 && m_fullString.length() > 0 && m_fullString.length() > pos) {
		for (i = pos; i < m_fullString.length(); i++) {
			switch (m_fullString[i]) {
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
				//i--;//!!!!!!!!!!!!!!!!!REMOVE IF NOT NOT NEEDED!!!
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
			if (i >= m_fullString.length()) {
				return 0;
			}
		}
	}
	return i;
}

bool Serializable::isNumber(size_t pos)
{
	if (m_fullString[pos] >= 48 && m_fullString[pos] <= 57 || m_fullString[pos] == '.' || m_fullString[pos] == '-') {
		return true;
	}
	return false;
}

bool Serializable::isBool(size_t pos)
{
	if (tolower(m_fullString[pos]) == 't') {
		if (m_fullString.length() > pos + 3 && tolower(m_fullString[pos + 1]) == 'r' && tolower(m_fullString[pos + 2]) == 'u' && tolower(m_fullString[pos + 3]) == 'e') {
			return true;
		}
	} else if (tolower(m_fullString[pos]) == 'f') {
		if (m_fullString.length() > pos + 4 && tolower(m_fullString[pos + 1]) == 'a' && tolower(m_fullString[pos + 2]) == 'l' && tolower(m_fullString[pos + 3]) == 's' && tolower(m_fullString[pos + 4]) == 'e') {
			return true;
		}
	}
	return false;
}

size_t Serializable::toNumber(const std::string & numberStr)
{
	size_t result = 0;
	size_t pos = 0;
	for (size_t i = numberStr.length() - 1; i >= 0 && i < numberStr.length(); i--) {
		char ch = numberStr[i];
		size_t chValue = (((unsigned int)ch) - 48);
		size_t tenCounter = std::pow(10, pos);
		result += (tenCounter * chValue);
		pos++;
	}
	return result;
}

size_t Serializable::addStringValue(size_t pos, const std::string &name)
{
	size_t i = pos;
	std::string value = "";
	if ((i = addName(i, value)) <= 0) {
		return 0;
	}
	this->m_kvPairStrings[name] = value;
	return i;
}

size_t Serializable::addObjectValue(size_t pos, const std::string &name)
{
	if (name.length() <= 0) {
		return 0;
	}
	size_t i = pos;
	if (pos > 0 && m_fullString.length() > 0 && m_fullString.length() > pos && m_fullString[pos] == '{') {
		Serializable *child = new Serializable(m_fullString, this);
		m_kvPairObjects[name] = child;
		if ((i = child->fromString(i)) <= 0) {
			return 0;
		}
	}
	return i;
}

size_t Serializable::addArrayValue(size_t pos, const std::string &name)
{
	if (name.length() <= 0) {
		return 0;
	}
	size_t i = pos;
	if (pos > 0 && m_fullString.length() > 0 && m_fullString.length() > pos && m_fullString[pos] == '[') {
		Serializable *child = new Serializable(m_fullString, this);
		m_kvPairArrays[name] = child;
		if ((i = child->fromStringArray(i)) <= 0) {
			return 0;
		}
	}
	return i;
}

size_t Serializable::addIntegerValue(size_t pos, const std::string & name)
{
	if (name.length() <= 0) {
		return 0;
	}
	size_t i = 0;
	std::string number = "";
	if (pos > 0 && m_fullString.length() > 0 && m_fullString.length() > pos) {
		for (i = pos; i < m_fullString.length(); i++) {
			if (isNumber(i)) {
				number += m_fullString[i];
			}
			else if (m_fullString[i] == ' ' || m_fullString[i] == '\t' || m_fullString[i] == ',' || m_fullString[i] == 'L' || m_fullString[i] == 'l') {
				size_t numberValue = toNumber(number);
				m_kvPairNumbers[name] = numberValue;
				return i;
			}
			else {
				return 0;
			}
		}
	}
	return i;
}

size_t Serializable::addBoolValue(size_t pos, const std::string & name)
{
	if (tolower(m_fullString[pos]) == 't') {
		m_kvPairBools[name] = true;
		return pos + constLength("True");
	}
	else if (tolower(m_fullString[pos]) == 'f') {
		m_kvPairBools[name] = false;
		return pos + constLength("False");
	}
	return 0;
}

size_t Serializable::addArray(size_t pos)
{
	size_t i = pos;
	if (pos > 0 && m_fullString.length() > 0 && m_fullString.length() > pos && m_fullString[pos] == '{') {
		Serializable *child = new Serializable(m_fullString, this);
		if ((i = child->fromStringArray(i)) <= 0) {
			return 0;
		}
		m_arrayArrays.push_back(child);
	}
	return i;
}

size_t Serializable::addString(size_t pos)
{
	size_t i = pos;
	std::string value = "";
	if ((i = addName(i + 1, value)) <= 0) {
		return 0;
	}
	m_arrayStrings.push_back(value);
	return i;
}

size_t Serializable::addObject(size_t pos)
{
	size_t i = pos;
	if (pos > 0 && m_fullString.length() > 0 && m_fullString.length() > pos && m_fullString[pos] == '{') {
		Serializable *child = new Serializable(m_fullString, this);
		m_arrayObjects.push_back(child);
		if ((i = child->fromString(i)) <= 0) {
			return 0;
		}
	}
	return i;
}

size_t Serializable::addInteger(size_t pos)
{
	size_t i = 0;
	std::string number = "";
	for (i = pos; i < m_fullString.length(); i++) {
		if (isNumber(i)) {
			number += m_fullString[i];
		}
		else if (m_fullString[i] == ' ' || m_fullString[i] == '\t' || m_fullString[i] == ',' || m_fullString[i] == 'L' || m_fullString[i] == 'l') {
			size_t numberValue = toNumber(number);
			m_arrayNumbers.push_back(numberValue);
			return i;
		}
		else {
			return 0;
		}
	}
	return i;
}

size_t Serializable::addBool(size_t pos)
{
	if (tolower(m_fullString[pos]) == 't') {
		m_arrayBools.push_back(true);
		return pos + constLength("True");
	} else if (tolower(m_fullString[pos]) == 'f') {
		m_arrayBools.push_back(false);
		return pos + constLength("False");
	}
	return 0;
}
