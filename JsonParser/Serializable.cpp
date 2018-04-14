#include "stdafx.h"
#include "Serializable.h"
#include <stdio.h>
#include <ctype.h>
#include <iostream>

/*template<typename T>
T Serializable<T>::getValue(const T & name) const
{
	if (m_kvPairStrings.count(name) > 0) {
		return m_kvPairStrings.at(name);
	}
	return "";
}

template<typename T>
void Serializable<T>::setValue(const T & name, const T & value)
{
	m_kvPairStrings[name] = value;
}*/

template<typename T>
bool Serializable<T>::fromString()
{
	if (strLen() > 0) {
		if (checkString()) {
			std::cout << "successfully parsed" << std::endl;
			return true;
		}
		else {
			std::cout << "parsing failed..." << std::endl;
			return false;
		}
	}
	return true;
}

template<typename T>
bool Serializable<T>::fromString(const T & str)
{
	this->m_fullString = new T(str);
	return fromString();
}

template<typename T>
size_t Serializable<T>::fromString(size_t pos)
{
	size_t i = 0;
	size_t openingCount = 0;
	size_t closeCount = 0;
	for (i = pos; i < strLen(); i++) {
		if (getChar(i) == '{' && !checkEscape(i)) {
			openingCount++;
		}
		else if (getChar(i) == '}' && !checkEscape(i)) {
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

template<typename T>
size_t Serializable<T>::fromStringArray(size_t pos)
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
		}
		else if (getChar(i) == ']' && !checkEscape(i)) {
			closeCount++;
		}
		if (openingCount == closeCount) {
			break;
		}
	}
	return i;
}

template<typename T>
size_t Serializable<T>::parseStringArray(size_t pos)
{
	size_t i = 0;
	bool commaFound = true;
	if (pos > 0 && strLen() > 0 && strLen() > pos && getChar(pos - 1) == '[') {
		for (i = pos; i < strLen(); i++) {
			switch (getChar(i)) {
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

template<typename T>
T Serializable<T>::toString() const
{
	return T();
}

template<typename T>
bool Serializable<T>::checkString()
{
	size_t openingCount = 0;
	size_t closeCount = 0;
	for (size_t i = 0; i < strLen();i++) {
		if (getChar(i) == '{' && !checkEscape(i)) {
			openingCount++;
		}
		else if(getChar(i) == '}' && !checkEscape(i)) {
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

template<typename T>
bool Serializable<T>::matchChar(char ch1, char ch2)
{
	return ch1 == ch2;
}

template<typename T>
bool Serializable<T>::checkEscape(size_t pos)
{
	return pos > 0 && matchChar(getChar(pos - 1), '\\');
}

template<typename T>
size_t Serializable<T>::addKVPair(size_t pos)
{
	size_t i = 0;
	if (pos > 0 && strLen() > 0 && strLen() > pos) {
		T name;
		for (i = pos; i < strLen(); i++) {
			switch (getChar(i)) {
			case '"':
				if ((i = addName(i + 1, name)) <= 0) {
					return 0;
				}
				break;
			case ':':
				if ((i = addValue(i + 1, name)) <= 0) {
					name = T();
					return 0;
				}
				i--;
				name = T();
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

template<typename T>
size_t Serializable<T>::addName(size_t pos, T &name)
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

template<typename T>
size_t Serializable<T>::addValue(size_t pos, const T &name)
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

template<typename T>
bool Serializable<T>::isNumber(size_t pos)
{
	if (getChar(pos) >= 48 && getChar(pos) <= 57 || getChar(pos) == '.' || getChar(pos) == '-') {
		return true;
	}
	return false;
}

template<typename T>
bool Serializable<T>::isBool(size_t pos)
{
	if (tolower(getChar(pos)) == 't') {
		if (strLen() > pos + 3 && tolower(getChar(pos + 1)) == 'r' && tolower(getChar(pos + 2)) == 'u' && tolower(getChar(pos + 3)) == 'e') {
			return true;
		}
	} else if (tolower(getChar(pos)) == 'f') {
		if (strLen() > pos + 4 && tolower(getChar(pos + 1)) == 'a' && tolower(getChar(pos + 2)) == 'l' && tolower(getChar(pos + 3)) == 's' && tolower(getChar(pos + 4)) == 'e') {
			return true;
		}
	}
	return false;
}

template<typename T>
size_t Serializable<T>::toNumber(const T & numberStr)
{
	size_t result = 0;
	size_t pos = 0;
	for (size_t i = numberStr.length() - 1; i >= 0 && i < numberStr.length(); i--) {
		char ch = numberStr[i];
		size_t chValue = (((unsigned int)ch) - 48);
		size_t tenCounter = static_cast<size_t>(std::pow(10, pos));
		result += (tenCounter * chValue);
		pos++;
	}
	return result;
}

template<typename T>
size_t Serializable<T>::strLen() const
{
	if (this->m_fullString != nullptr) {
		return this->m_fullString->length();
	}
	return 0;
}

template<typename T>
char Serializable<T>::getChar(size_t pos) const
{
	if (this->m_fullString != nullptr) {
		return this->m_fullString->at(pos);
	}
	return 0;
}

template<typename T>
size_t Serializable<T>::addStringValue(size_t pos, const T &name)
{
	size_t i = pos;
	T value = T();
	if ((i = addName(i, value)) <= 0) {
		return 0;
	}
	this->m_kvPairStrings[name] = value;
	return i;
}

template<typename T>
size_t Serializable<T>::addObjectValue(size_t pos, const T &name)
{
	if (name.length() <= 0) {
		return 0;
	}
	size_t i = pos;
	if (pos > 0 && strLen() > 0 && strLen() > pos && getChar(pos) == '{') {
		Serializable<T> *child = new Serializable<T>(this);
		child->m_fullString = this->m_fullString;
		m_kvPairObjects[name] = child;
		if ((i = child->fromString(i)) <= 0) {
			return 0;
		}
	}
	return i;
}

template<typename T>
size_t Serializable<T>::addArrayValue(size_t pos, const T &name)
{
	if (name.length() <= 0) {
		return 0;
	}
	if (pos > 0 && strLen() > 0 && strLen() > pos && getChar(pos) == '[') {
		Serializable<T> *child = new Serializable<T>(this);
		child->m_fullString = this->m_fullString;
		m_kvPairArrays[name] = child;
		if ((pos = child->fromStringArray(pos)) <= 0) {
			return 0;
		}
	}
	return pos;
}

template<typename T>
size_t Serializable<T>::addIntegerValue(size_t pos, const T & name)
{
	if (name.length() <= 0) {
		return 0;
	}
	size_t i = 0;
	T number = T();
	if (pos > 0 && strLen() > 0 && strLen() > pos) {
		for (i = pos; i < strLen(); i++) {
			if (isNumber(i)) {
				number += getChar(i);
			}
			else if (getChar(i) == ' ' || getChar(i) == '\t' || getChar(i) == ',' || getChar(i) == 'L' || getChar(i) == 'l') {
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

template<typename T>
size_t Serializable<T>::addBoolValue(size_t pos, const T & name)
{
	if (tolower(getChar(pos)) == 't') {
		m_kvPairBools[name] = true;
		return pos + constLength("True");
	}
	else if (tolower(getChar(pos)) == 'f') {
		m_kvPairBools[name] = false;
		return pos + constLength("False");
	}
	return 0;
}

template<typename T>
size_t Serializable<T>::addArray(size_t pos)
{
	size_t i = pos;
	if (pos > 0 && strLen() > 0 && strLen() > pos && getChar(pos) == '{') {
		Serializable<T> *child = new Serializable<T>(this);
		child->m_fullString = this->m_fullString;
		if ((i = child->fromStringArray(i)) <= 0) {
			return 0;
		}
		m_arrayArrays.push_back(child);
	}
	return i;
}

template<typename T>
size_t Serializable<T>::addString(size_t pos)
{
	size_t i = pos;
	T value = T();
	if ((i = addName(i + 1, value)) <= 0) {
		return 0;
	}
	m_arrayStrings.push_back(value);
	return i;
}

template<typename T>
size_t Serializable<T>::addObject(size_t pos)
{
	size_t i = pos;
	if (pos > 0 && strLen() > 0 && strLen() > pos && getChar(pos) == '{') {
		Serializable<T> *child = new Serializable<T>(this);
		child->m_fullString = this->m_fullString;
		m_arrayObjects.push_back(child);
		if ((i = child->fromString(i)) <= 0) {
			return 0;
		}
	}
	return i;
}

template<typename T>
size_t Serializable<T>::addInteger(size_t pos)
{
	size_t i = 0;
	T number = "";
	for (i = pos; i < strLen(); i++) {
		if (isNumber(i)) {
			number += getChar(i);
		}
		else if (getChar(i) == ' ' || getChar(i) == '\t' || getChar(i) == ',' || getChar(i) == 'L' || getChar(i) == 'l') {
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

template<typename T>
size_t Serializable<T>::addBool(size_t pos)
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
