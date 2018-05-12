#ifndef JSONPARSER_NUMBER_CPP_
#define JSONPARSER_NUMBER_CPP_

#include "Number.h"

JsonParser::Number::Number(const std::string & numberStr) : m_numberStr(new std::string(numberStr))
{
}

JsonParser::Number::Number(__int64 number) : m_numberStr(new std::string(std::to_string(number)))
{
}

__int64 JsonParser::Number::toNumber() const
{
	if (m_numberStr == nullptr || m_numberStr->size() <= 0) {
		return 0;
	}
	__int64 result = 0;
	size_t pos = 0;
	for (size_t i = m_numberStr->length() - 1;
		i >= 0 && i < m_numberStr->length();
		i--) {
		char ch = m_numberStr->at(i);
		if (ch == '-') {
			result = -result;
			continue;
		}
		signed int chValue = ((static_cast<unsigned int>(ch)) - 48);
		__int64 tenCounter = static_cast<__int64>(std::pow(10, pos));
		result += (tenCounter * chValue);
		pos++;
	}
	return result;
}

std::string JsonParser::Number::toString() const
{
	if (m_numberStr != nullptr) {
		return *this->m_numberStr;
	}
	return "";
}

#endif  // JSONPARSER_NUMBER_CPP_
