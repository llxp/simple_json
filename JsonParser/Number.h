#pragma once
#ifndef JSONPARSER_NUMBER_H_
#define JSONPARSER_NUMBER_H_

#include <string>

namespace JsonParser {

	class Number
	{
		public:
			explicit Number(const std::string &numberStr) : m_numberStr(numberStr), m_default(false) {}
			explicit Number(__int64 number) : m_numberStr(std::to_string(number)), m_default(false) {}
			explicit Number(__int64 *number) : m_numberRef(number), m_default(false) {}
			explicit Number() : m_numberStr("0") {}
			~Number();
			__int64 toNumber() const;
			std::string toString() const;
			bool isDefault() const;

		private:
			std::string m_numberStr{ "0" };
			__int64 * m_numberRef{ nullptr };
			bool m_default{ true };
	};
}

//#include "Number.cpp"

#endif  // JSONPARSER_NUMBER_H_
