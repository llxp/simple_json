/*
MIT License

Copyright (c) 2018 Lukas L�dke

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
#ifndef SRC_NUMBER_H_
#define SRC_NUMBER_H_

#include <inttypes.h>
#include <string>
#ifdef __linux__
#define __int64 long long
#endif

#include "dll.h"
#include "JsonTypes.h"

namespace JsonParser {
class Number
{
	public:
		DLLEXPORT explicit Number(
			const JsonString &&numberStr) : m_numberStr(std::move(numberStr)), m_default(false) {}
		DLLEXPORT explicit Number(
			double number) : m_numberStr(ToString(number)), m_default(false) {}
		DLLEXPORT explicit Number(
			double *number) : m_numberRefFP(number), m_default(false) {}
		DLLEXPORT explicit Number(
			__int64 number) : m_numberStr(ToString(number)), m_default(false) {}
		DLLEXPORT explicit Number(
			__int64 *number) : m_numberRef(number), m_default(false) {}
		DLLEXPORT explicit Number() : m_numberStr(Stringify(0)) {}
		DLLEXPORT ~Number();
		DLLEXPORT __int64 toNumber() const;
		DLLEXPORT double toNumberFP() const;
		DLLEXPORT JsonString toString() const;
		DLLEXPORT bool isDefault() const;
		DLLEXPORT void setNumberRefValue(const Number &value);

	private:
		bool m_isFloatingPoint{ false };
		JsonString m_numberStr{ EmptyString };
		__int64 * m_numberRef{ nullptr };
		double * m_numberRefFP{ nullptr };
		bool m_default{ true };
};
}  // namespace JsonParser

#endif  // SRC_NUMBER_H_
