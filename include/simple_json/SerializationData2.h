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
#ifndef SRC_SERIALIZATIONDATA2_H_
#define SRC_SERIALIZATIONDATA2_H_

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

	class SerializationData2
	{
	public:
		SerializationData2();
		virtual ~SerializationData2();

	protected:
		virtual bool fromString(char c = -1) = 0;
		virtual JsonString toString() const = 0;
		virtual JsonString toStringArray() const = 0;
		void assign(const std::unique_ptr<SerializationData2> &other);

	protected:
		void setType(const JsonTypes &type);
		JsonTypes type() const;
		void setFullString(std::istream *str);
		std::istream *fullString() const;
		JsonChar getNextChar() const;
		void clearAll();

	private:
		template<typename T>
		T* lazyInit2(std::unique_ptr<T> &var)
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
		std::map<JsonString, std::unique_ptr<SerializationData2>> *kvPairObjects();
		std::map<JsonString, std::unique_ptr<SerializationData2>> *kvPairArrays()
		{
			return lazyInit2<
				std::map<JsonString, std::unique_ptr<SerializationData2>>
			>(m_kvPairArrays);
		};
		std::vector<JsonString> *kvPairNullValues()
		{
			return lazyInit2<std::vector<JsonString>>(m_kvPairNullValues);
		};

		std::vector<std::unique_ptr<SerializationData2>> *arrayObjects()
		{
			return lazyInit2<
				std::vector<std::unique_ptr<SerializationData2>>
			>(m_arrayObjects);
		};
		std::vector<std::unique_ptr<SerializationData2>> *arrayArrays()
		{
			return lazyInit2<
				std::vector<std::unique_ptr<SerializationData2>>
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
			std::map<JsonString, std::unique_ptr<SerializationData2>>> m_kvPairObjects;
		std::unique_ptr<
			std::map<JsonString, std::unique_ptr<SerializationData2>>> m_kvPairArrays;
		std::unique_ptr<std::vector<JsonString>> m_kvPairNullValues;

		std::unique_ptr<
			std::vector<std::unique_ptr<SerializationData2>>> m_arrayObjects;
		std::unique_ptr<
			std::vector<std::unique_ptr<SerializationData2>>> m_arrayArrays;
		std::unique_ptr<std::vector<JsonParser::Number>> m_arrayNumbers;
		std::unique_ptr<std::vector<bool>> m_arrayBools;
		std::unique_ptr<std::vector<JsonString>> m_arrayStrings;

	private:
		std::istream *m_fullString;
		size_t m_strLen{ 0 };
		JsonTypes m_type{ JsonTypes::Object };
	};

}  // namespace JsonParser

#endif  // SRC_SERIALIZATIONDATA_H_
