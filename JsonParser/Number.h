#pragma once
#ifndef JSONPARSER_NUMBER_H_
#define JSONPARSER_NUMBER_H_

#include <string>

namespace JsonParser {

	class Number
	{
		public:
			explicit Number(const std::string &numberStr);
			explicit Number(__int64 number);
			explicit Number(__int64 *number) : m_numberRef(number) {}
			explicit Number() {}
			__int64 toNumber() const;
			std::string toString() const;

		private:
			std::string * m_numberStr{ nullptr };
			__int64 * m_numberRef{ nullptr };
	};
}

//#include "Number.cpp"

#endif  // JSONPARSER_NUMBER_H_
