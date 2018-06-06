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
#include "Vector.h"

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
		DLLEXPORT ~SerializationData();
		virtual void serialize() = 0;
		virtual bool fromString() = 0;
		virtual JsonString toString() const = 0;
		virtual JsonString toStringArray() const = 0;

	protected:
		void setType(const JsonTypes &type);
		JsonTypes type() const;
		void setFullString(JsonString *str);
		JsonString *fullString() const;
		inline JsonChar getChar(const size_t &pos) const;
		inline size_t strLen() const;
		inline bool matchChar(const size_t &i, JsonChar ch) const;
		inline JsonString substr(const size_t &start, const size_t &stop) const;

	protected:
		std::map<JsonString, void *> m_kvPairMapping;

		JsonParser::Vector<JsonString> *m_mappingStringArrays{ nullptr };
		JsonParser::Vector<bool> *m_mappingBoolArrays{ nullptr };
		JsonParser::Vector<JsonParser::Number> *m_mappingNumberArrays{ nullptr };
		JsonParser::VectorBase *m_mappingObjectArrays{ nullptr };
		JsonParser::VectorBase *m_mappingArrayArrays{ nullptr };

		std::vector<std::pair<JsonString,
			std::pair<JsonTypes, bool>>> m_serializableMembers;
		std::vector<void *> m_collectibleObjects;

	private:
		JsonString * m_fullString;
		size_t m_strLen{ 0 };
		JsonTypes m_type{ JsonTypes::Object };
	};

	inline JsonChar JsonParser::SerializationData::getChar(const size_t & pos) const
	{
		if (pos >= 0 && strLen() > 0 && pos < strLen()) {
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
