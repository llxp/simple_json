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

#ifndef JSON_SERIALIZATIONDATA_H_
#define JSON_SERIALIZATIONDATA_H_

#include <vector>
#include <map>
#include <memory>
#include <string>

#include "Number.h"
#include "JsonTypes.h"

#ifdef __GNUC__
#define __cdecl __attribute__((__cdecl__))
#define __int64 long long
#endif

#define lazyInit(var, type)\
if (var == nullptr)\
{ var = new type(); }\
return var;

namespace JsonParser {

class SerializationData
{
	public:
		SerializationData();
		virtual ~SerializationData();

	protected:
		virtual bool fromString() = 0;
		virtual JsonString toString() const = 0;
		virtual JsonString toStringArray() const = 0;
		void assign(const std::unique_ptr<SerializationData> &other);

	protected:
		void setType(const JsonTypes &type);
		JsonTypes type() const;
		void setFullString(JsonString *str);
		JsonString *fullString() const;
		inline JsonChar getChar(const size_t &pos) const;
		inline size_t strLen() const;
		void clearAll();
		inline bool matchChar(const size_t &i, JsonChar ch) const;
		inline JsonString substr(const size_t &start, const size_t &stop) const;

	private:
		template<typename T>
		auto lazyInit2(std::unique_ptr<T> &var)
		{
			if (var == nullptr) {
				var = std::make_unique<T>();
			}
			return var.get();
		}

	protected:
		std::map<JsonString, JsonParser::Number> *kvPairNumbers();
		std::map<JsonString, bool> *kvPairBools();
		std::map<JsonString, JsonString> *kvPairStrings();
		std::map<JsonString, std::unique_ptr<SerializationData>> *kvPairObjects();
		std::map<JsonString, std::unique_ptr<SerializationData>> *kvPairArrays();
		std::vector<JsonString> *kvPairNullValues();

		std::vector<std::unique_ptr<SerializationData>> *arrayObjects();
		std::vector<std::unique_ptr<SerializationData>> *arrayArrays()
		{
			return lazyInit2<
				std::vector<std::unique_ptr<SerializationData>>
			>(m_arrayArrays);
		};
		std::vector<JsonParser::Number> *arrayNumbers()
		{
			return lazyInit2<std::vector<JsonParser::Number>>(m_arrayNumbers);
		};
		std::vector<bool> *arrayBools()
		{
			return lazyInit2<std::vector<bool>>(m_arrayBools);
		};
		std::vector<JsonString> *arrayStrings()
		{
			return lazyInit2<std::vector<JsonString>>(m_arrayStrings);
		};

	private:
		std::unique_ptr<std::map<JsonString, JsonParser::Number>> m_kvPairNumbers;
		std::unique_ptr<std::map<JsonString, bool>> m_kvPairBools;
		std::unique_ptr<std::map<JsonString, JsonString>> m_kvPairStrings;
		std::unique_ptr<
			std::map<JsonString, std::unique_ptr<SerializationData>>> m_kvPairObjects;
		std::unique_ptr<
			std::map<JsonString, std::unique_ptr<SerializationData>>> m_kvPairArrays;
		std::unique_ptr<std::vector<JsonString>> m_kvPairNullValues;

		std::unique_ptr<
			std::vector<std::unique_ptr<SerializationData>>> m_arrayObjects;
		std::unique_ptr<
			std::vector<std::unique_ptr<SerializationData>>> m_arrayArrays;
		std::unique_ptr<std::vector<JsonParser::Number>> m_arrayNumbers;
		std::unique_ptr<std::vector<bool>> m_arrayBools;
		std::unique_ptr<std::vector<JsonString>> m_arrayStrings;

	private:
		JsonString *m_fullString;
		size_t m_strLen { 0 };
		JsonTypes m_type{ JsonTypes::Object };
};

inline JsonChar JsonParser::SerializationData::getChar(const size_t & pos) const
{
	if (pos > 0 && strLen() > 0 && pos < strLen()) {
		return this->m_fullString->at(pos);
	}
	return 0;
}
}  // namespace JsonParser

inline size_t JsonParser::SerializationData::strLen() const
{
	return this->m_strLen;
}

inline bool JsonParser::SerializationData::matchChar(
	const size_t &i, JsonChar ch) const
{
	return this->m_fullString->at(i) == ch;
}

inline JsonString JsonParser::SerializationData::substr(
	const size_t & start, const size_t & stop) const
{
	return this->m_fullString->substr(start, stop);
}

#endif  // JSON_SERIALIZATIONDATA_H_
