#ifndef SRC_NUMBER_H_
#define SRC_NUMBER_H_

#include <inttypes.h>
#include <string>
#ifdef __linux__
#define __int64 long long
#endif

#include "dll.h"

namespace JsonParser {
class Number
{
	public:
		DLLEXPORT explicit Number(
			const std::string &numberStr) : m_numberStr(numberStr), m_default(false) {}
		DLLEXPORT explicit Number(
			double number) : m_numberStr(std::to_string(number)), m_default(false) {}
		DLLEXPORT explicit Number(
			double *number) : m_numberRefFP(number), m_default(false) {}
		DLLEXPORT explicit Number(
			__int64 number) : m_numberStr(std::to_string(number)), m_default(false) {}
		DLLEXPORT explicit Number(
			__int64 *number) : m_numberRef(number), m_default(false) {}
		DLLEXPORT explicit Number() : m_numberStr("0") {}
		DLLEXPORT ~Number();
		DLLEXPORT __int64 toNumber() const;
		DLLEXPORT double toNumberFP() const;
		DLLEXPORT std::string toString() const;
		DLLEXPORT bool isDefault() const;
		DLLEXPORT void setNumberRefValue(const Number &value);

	private:
		bool m_isFloatingPoint{ false };
		std::string m_numberStr{ "" };
		__int64 * m_numberRef{ nullptr };
		double * m_numberRefFP{ nullptr };
		bool m_default{ true };
};
}  // namespace JsonParser

#endif  // SRC_NUMBER_H_
