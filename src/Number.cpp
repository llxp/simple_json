#include "Number.h"
#include <cmath>
#include <string>

JsonParser::Number::~Number()
{
}

__int64 JsonParser::Number::toNumber() const
{
	if (m_numberStr.size() <= 0) {
		return 0;
	}
	size_t pos = 0;
	int result = 0;
	double afterCommaResult = 0;
	bool devide = false;
	unsigned int tenCounter = 0;
	bool negative = false;
	for (size_t i = m_numberStr.length() - 1;
		i >= 0 && i < m_numberStr.length();
		--i) {
		char ch = m_numberStr[i];
		if (ch == '-') {
			negative = true;
			continue;
		}
		if (ch == '.') {
			result = 0;
			pos = 0;
			continue;
		}
		unsigned int chValue = ((static_cast<unsigned int>(ch)) - 48);
		tenCounter = static_cast<unsigned int>(std::pow(10, pos));
		result += (tenCounter * chValue);
		pos++;
	}
	if (negative) {
		return -(result);
	}
	return result;
}

DLLEXPORT double JsonParser::Number::toNumberFP() const
{
	if (m_numberStr.size() <= 0) {
		return 0;
	}
	size_t pos = 0;
	int result = 0;
	double afterCommaResult = 0;
	unsigned int tenCounter = 0;
	bool negative = false;
	for (size_t i = m_numberStr.length() - 1;
		i >= 0 && i < m_numberStr.length();
		--i) {
		char ch = m_numberStr[i];
		if (ch == '-') {
			negative = true;
			continue;
		}
		if (ch == '.') {
			afterCommaResult = result / tenCounter;
			result = 0;
			pos = 0;
			continue;
		}
		unsigned int chValue = ((static_cast<unsigned int>(ch)) - 48);
		tenCounter = static_cast<unsigned int>(std::pow(10, pos));
		result += (tenCounter * chValue);
		pos++;
	}
	if (negative) {
		return -(result + afterCommaResult);
	}
	return result + afterCommaResult;
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
	if (this->m_numberRef != nullptr && !this->m_isFloatingPoint) {
		(*this->m_numberRef) = value.toNumber();
	} else if (this->m_numberRefFP != nullptr && this->m_isFloatingPoint) {
		(*this->m_numberRefFP) = value.toNumberFP();
	}
}
