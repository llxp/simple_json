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

#include "SerializationData.h"

namespace JsonParser {
	class SerializationUtils
	{
	public:
		template<typename T1, typename T2>
		static JsonString makeStr3(
			const T2 *vec,
			const std::function<JsonString(T1 *)> &lambda)
		{
			JsonString outputStr;
			outputStr += JsonArrayOpen;
			auto endPos = vec->end();
			for (auto it = vec->begin(); it != endPos; it++) {
				auto currentElement = static_cast<T1>(*it);
				//if (currentElement != nullptr) {
					outputStr += lambda(&currentElement);
					if (it + 1 != vec->end()) {
						outputStr += JsonEntrySeparator;
					}
				//}
			}
			outputStr += JsonArrayClose;
			return outputStr;
		}

		static JsonString makeStrVector(
			const std::vector<const void *> *vec,
			const std::function<JsonString(const SerializationData *)> &lambda)
		{
			JsonString outputStr;
			outputStr += JsonArrayOpen;
			auto endPos = vec->end();
			for (auto it = vec->begin(); it != endPos; it++) {
				auto currentElement = static_cast<const SerializationData *>(*it);
				if (currentElement != nullptr) {
					outputStr += lambda(currentElement);
					if (it + 1 != vec->end()) {
						outputStr += JsonEntrySeparator;
					}
				}
			}
			outputStr += JsonArrayClose;
			return outputStr;
		}

		static inline JsonString makeString(const JsonString &str);
		static inline JsonString makeKvPairStrNumber(const JsonString &name,
			const JsonParser::Number &value);
		static inline JsonString makeKvPairStrString(const JsonString &name,
			const JsonString &value);
		static inline JsonString makeKvPairStrObject(const JsonString &name,
			SerializationData *value);
		static inline JsonString makeKvPairStrBool(const JsonString &name,
			bool value);
		static inline JsonString makeKvPairStrArray(const JsonString &name,
			JsonParser::SerializationData *value);

		static inline JsonString makeStrObjectArray(
			const JsonParser::VectorBase *value);
		static inline JsonString makeStrNumberArray(
			const JsonParser::Vector<JsonParser::Number> *value);
		static inline JsonString makeStrBoolArray(
			const JsonParser::Vector<bool> *value);
		static inline JsonString makeStrStringArray(
			const JsonParser::Vector<JsonString> *value);
		static inline JsonString makeStrArrayArray(
			const JsonParser::VectorBase *value);
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
	const JsonString & name, const JsonString & value)
{
	return SerializationUtils::makeString(name) + JsonKvSeparator +
		SerializationUtils::makeString(value);
}

inline JsonString JsonParser::SerializationUtils::makeKvPairStrObject(
	const JsonString & name, SerializationData * value)
{
	return SerializationUtils::makeString(name) + JsonKvSeparator +
		value->toString();
}

inline JsonString JsonParser::SerializationUtils::makeKvPairStrBool(
	const JsonString & name, bool value)
{
	return SerializationUtils::makeString(name) + JsonKvSeparator +
		(value ? Stringify(true) : Stringify(false));
}

inline JsonString JsonParser::SerializationUtils::makeKvPairStrArray(
	const JsonString & name, JsonParser::SerializationData *value)
{
	return SerializationUtils::makeString(name) + JsonKvSeparator +
		value->toStringArray();
}

inline JsonString JsonParser::SerializationUtils::makeStrObjectArray(
	const JsonParser::VectorBase * value)
{
	if (value == nullptr) {
		return EmptyJsonArray;
	}
	auto elements = value->getElements();
	return JsonParser::SerializationUtils::makeStrVector(
		&elements, [=](auto currentElement)->JsonString {
		return currentElement->toString();
	});
}

inline JsonString JsonParser::SerializationUtils::makeStrNumberArray(
	const JsonParser::Vector<JsonParser::Number>* value)
{
	if (value == nullptr) {
		return EmptyJsonArray;
	}
	return makeStr3<JsonParser::Number, JsonParser::Vector<JsonParser::Number>>(
		value, [=](auto currentElement)->JsonString {
		return currentElement->toString();
	});
}

inline JsonString JsonParser::SerializationUtils::makeStrBoolArray(
	const JsonParser::Vector<bool>* value)
{
	if (value == nullptr) {
		return EmptyJsonArray;
	}
	return makeStr3<bool, JsonParser::Vector<bool>>(
		value, [=](auto currentElement)->JsonString {
		return ToString(*currentElement);
	});
}

inline JsonString JsonParser::SerializationUtils::makeStrStringArray(
	const JsonParser::Vector<JsonString>* value)
{
	if (value == nullptr) {
		return EmptyJsonArray;
	}
	return makeStr3<JsonString, JsonParser::Vector<JsonString>>
		(value, [=](auto currentElement)->JsonString {
		return SerializationUtils::makeString(*currentElement);
	});
}

inline JsonString JsonParser::SerializationUtils::makeStrArrayArray(
	const JsonParser::VectorBase * value)
{
	if (value == nullptr) {
		return EmptyJsonArray;
	}
	auto elements = value->getElements();
	return JsonParser::SerializationUtils::makeStrVector(
		&elements, [=](auto currentElement)->JsonString {
		return currentElement->toStringArray();
	});
}
#endif  // SRC_SERIALIZATIONUTILS_H_
