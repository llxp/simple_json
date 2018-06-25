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

#ifndef SRC_SERIALIZATIONUTILS_H_
#define SRC_SERIALIZATIONUTILS_H_

#include <string>
#include <memory>
#include <functional>
#include <vector>

#include <simple_json/SerializationData.h>

namespace JsonParser {
class SerializationUtils
{
	public:
		static inline JsonString makeString(const JsonString &str);
		static inline JsonString makeKvPairStrNumber(const JsonString &name,
			const JsonParser::Number &value);
		static inline JsonString makeKvPairStrString(const JsonString &name,
			JsonString *value);
};
}  // namespace JsonParser

inline JsonString JsonParser::SerializationUtils::makeString(
	const JsonString & str)
{
	return JsonStringSeparator + str + JsonStringSeparator;
}

inline JsonString JsonParser::SerializationUtils::makeKvPairStrNumber(
	const JsonString & name, const JsonParser::Number & value)
{
	return SerializationUtils::makeString(name) + JsonKvSeparator +
		value.toString();
}

inline JsonString JsonParser::SerializationUtils::makeKvPairStrString(
	const JsonString & name, JsonString * value)
{
	return SerializationUtils::makeString(name) + JsonKvSeparator +
		SerializationUtils::makeString(*value);
}

#endif  // SRC_SERIALIZATIONUTILS_H_
