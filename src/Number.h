#pragma once
#ifndef JSONPARSER_NUMBER_H_
#define JSONPARSER_NUMBER_H_

#include <string>
#include <inttypes.h>
#ifdef __linux__
#define __int64 long long
#endif

#include "dll.h"

namespace JsonParser {

	class Number
	{
		public:
			DLLEXPORT explicit Number(const std::string &numberStr) : m_numberStr(numberStr), m_default(false) {}
			DLLEXPORT explicit Number(__int64 number) : m_numberStr(std::to_string(number)), m_default(false) {}
			DLLEXPORT explicit Number(__int64 *number) : m_numberRef(number), m_default(false) {}
			DLLEXPORT explicit Number() : m_numberStr("0") {}
			DLLEXPORT ~Number();
			DLLEXPORT __int64 toNumber() const;
			DLLEXPORT std::string toString() const;
			DLLEXPORT bool isDefault() const;
			DLLEXPORT void setNumberRefValue(const Number &value);

		private:
			std::string m_numberStr{ "" };
			__int64 * m_numberRef{ nullptr };
			bool m_default{ true };
	};
}

//#include "Number.cpp"

#endif  // JSONPARSER_NUMBER_H_
