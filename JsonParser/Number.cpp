#ifndef JSONPARSER_NUMBER_CPP_
#define JSONPARSER_NUMBER_CPP_

#include "Number.h"
#include <cmath>

JsonParser::Number::~Number()
{
}

__int64 JsonParser::Number::toNumber() const
{
	if (this->m_numberRef != nullptr) {
		return *this->m_numberRef;
	}
	if (m_numberStr.size() <= 0) {
		return 0;
	}
	__int64 result = 0;
	size_t pos = 0;
	for (size_t i = m_numberStr.length() - 1;
		i >= 0 && i < m_numberStr.length();
		i--) {
		char ch = m_numberStr.at(i);
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
	if (m_numberStr.length() > 0) {
		return this->m_numberStr;
	}
	if (this->m_numberRef != nullptr) {
		return std::to_string(*this->m_numberRef);
	}
	return "0";
}

bool JsonParser::Number::isDefault() const
{
	return this->m_default;
}

void JsonParser::Number::setNumberRefValue(const Number & value)
{
	if (this->m_numberRef != nullptr) {
		(*this->m_numberRef) = value.toNumber();
	}
}

#endif  // JSONPARSER_NUMBER_CPP_
