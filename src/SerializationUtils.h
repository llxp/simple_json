#pragma once

#include <string>
#include <memory>
#include <functional>

#include "SerializationMappingData.h"

namespace JsonParser {
	class SerializationUtils
	{
	public:
		template<typename T1, typename T2>
		static std::string makeStr3(
			const T2 *vec,
			const std::function<std::string(T1 *)> &lambda)
		{
			std::string outputStr;
			outputStr += JsonArrayOpen;
			for (auto it = vec->begin(); it != vec->end(); it++) {
				auto currentElement = static_cast<T1 *>((*it).get());
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

		static std::string makeStrVector(
			const std::vector<void *> *vec,
			const std::function<std::string(SerializationMappingData *)> &lambda)
		{
			std::string outputStr;
			outputStr += JsonArrayOpen;
			for (auto it = vec->begin(); it != vec->end(); it++) {
				auto currentElement = static_cast<SerializationMappingData *>(*it);
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

		static inline std::string makeString(const std::string &str);
		static inline std::string makeKvPairStrNumber(const std::string &name,
			const JsonParser::Number &value);
		static inline std::string makeKvPairStrString(const std::string &name,
			std::string *value);
		static inline std::string makeKvPairStrObject(const std::string &name,
			SerializationMappingData *value);
		static inline std::string makeKvPairStrBool(const std::string &name,
			bool value);
		static inline std::string makeKvPairStrArray(const std::string &name,
			JsonParser::SerializationMappingData *value);

		static inline std::string makeStrObjectArray(
			const JsonParser::VectorBase *value);
		static inline std::string makeStrNumberArray(
			const JsonParser::Vector<JsonParser::Number> *value);
		static inline std::string makeStrBoolArray(
			const JsonParser::Vector<bool> *value);
		static inline std::string makeStrStringArray(
			const JsonParser::Vector<std::string> *value);
		static inline std::string makeStrArrayArray(
			const JsonParser::VectorBase *value);
	};

}

inline std::string JsonParser::SerializationUtils::makeString(
	const std::string & str)
{
	return JsonStringSeparator + str + JsonStringSeparator;
}

inline std::string JsonParser::SerializationUtils::makeKvPairStrNumber(
	const std::string & name, const JsonParser::Number & value)
{
	return SerializationUtils::makeString(name) + JsonKvSeparator +
		value.toString();
}

inline std::string JsonParser::SerializationUtils::makeKvPairStrString(
	const std::string & name, std::string * value)
{
	return SerializationUtils::makeString(name) + JsonKvSeparator +
		SerializationUtils::makeString(*value);
}

inline std::string JsonParser::SerializationUtils::makeKvPairStrObject(
	const std::string & name, SerializationMappingData * value)
{
	return SerializationUtils::makeString(name) + JsonKvSeparator +
		value->toString();
}

inline std::string JsonParser::SerializationUtils::makeKvPairStrBool(
	const std::string & name, bool value)
{
	return SerializationUtils::makeString(name) + JsonKvSeparator +
		(value ? "true" : "false");
}

inline std::string JsonParser::SerializationUtils::makeKvPairStrArray(
	const std::string & name, JsonParser::SerializationMappingData *value)
{
	return SerializationUtils::makeString(name) + JsonKvSeparator +
		value->toStringArray();
}

inline std::string JsonParser::SerializationUtils::makeStrObjectArray(
	const JsonParser::VectorBase * value)
{
	if (value == nullptr) {
		return EmptyJsonArray;
	}
	auto elements = value->getElements();
	return JsonParser::SerializationUtils::makeStrVector(
		&elements, [=](auto currentElement)->std::string {
		return currentElement->toString();
	});
}

inline std::string JsonParser::SerializationUtils::makeStrNumberArray(
	const JsonParser::Vector<JsonParser::Number>* value)
{
	if (value == nullptr) {
		return EmptyJsonArray;
	}
	return makeStr3<JsonParser::Number, JsonParser::Vector<JsonParser::Number>>(
		value, [=](auto currentElement)->std::string {
		return currentElement->toString();
	});
}

inline std::string JsonParser::SerializationUtils::makeStrBoolArray(
	const JsonParser::Vector<bool>* value)
{
	if (value == nullptr) {
		return EmptyJsonArray;
	}
	return makeStr3<bool, JsonParser::Vector<bool>>(
		value, [=](auto currentElement)->std::string {
		return std::to_string(*currentElement);
	});
}

inline std::string JsonParser::SerializationUtils::makeStrStringArray(
	const JsonParser::Vector<std::string>* value)
{
	if (value == nullptr) {
		return EmptyJsonArray;
	}
	return makeStr3<std::string, JsonParser::Vector<std::string>>
		(value, [=](auto currentElement)->std::string {
		return SerializationUtils::makeString(*currentElement);
	});
}

inline std::string JsonParser::SerializationUtils::makeStrArrayArray(
	const JsonParser::VectorBase * value)
{
	if (value == nullptr) {
		return EmptyJsonArray;
	}
	auto elements = value->getElements();
	return JsonParser::SerializationUtils::makeStrVector(
		&elements, [=](auto currentElement)->std::string {
		return currentElement->toStringArray();
	});
}