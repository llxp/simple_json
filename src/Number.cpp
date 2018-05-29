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

#include <simple_json/Number.h>
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
		JsonChar ch = m_numberStr[i];
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
		JsonChar ch = m_numberStr[i];
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

JsonString JsonParser::Number::toString() const
{
	if (m_numberStr.length() > 0) {
		return this->m_numberStr;
	}
	if (this->m_numberRef != nullptr) {
		return ToString(*this->m_numberRef);
	}
	return Stringify(0);
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
