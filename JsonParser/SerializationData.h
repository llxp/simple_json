#pragma once
#ifndef JSONPARSER_SERIALIZATIONDATA_H_
#define JSONPARSER_SERIALIZATIONDATA_H_

#include <vector>
#include <map>
#include <memory>

#include "Number.h"
#include "JsonTypes.h"

#ifdef __GNUC__
#define __cdecl __attribute__((__cdecl__))
#define __int64 long long
#endif

#define lazyInit(var, type) if(var == nullptr) { var = new type(); } return var;

namespace JsonParser {

class SerializationData
{
public:
	explicit SerializationData();
	virtual ~SerializationData();

public:
	virtual bool fromString() = 0;
	virtual std::string toString() const = 0;
	virtual std::string toStringArray() const = 0;
	void assign(const std::unique_ptr<SerializationData> &other);

public:
	void setType(const JsonTypes &type);
	JsonTypes type() const;
	void setFullString(std::string *str);
	std::string *fullString() const;
	inline std::string::const_iterator getChar(const size_t &pos) const;
	inline size_t strLen() const;
	void clearAll();
	inline bool matchChar(const size_t &i, char ch) const;

private:
	template<typename T>
	T* lazyInit2(std::unique_ptr<T> &var)
	{
		if (var == nullptr) {
			var = std::make_unique<T>();
		}
		return var.get();
	}

public:
	std::map<std::string, JsonParser::Number> *kvPairNumbers() { return lazyInit2<std::map<std::string, JsonParser::Number>>(m_kvPairNumbers); };
	std::map<std::string, bool> *kvPairBools() { return lazyInit2<std::map<std::string, bool>>(m_kvPairBools); };
	std::map<std::string, std::string> *kvPairStrings() { return lazyInit2<std::map<std::string, std::string>>(m_kvPairStrings); };
	std::map<std::string, std::unique_ptr<SerializationData>> *kvPairObjects() { return lazyInit2<std::map<std::string, std::unique_ptr<SerializationData>>>(m_kvPairObjects); };
	std::map<std::string, std::unique_ptr<SerializationData>> *kvPairArrays() { return lazyInit2<std::map<std::string, std::unique_ptr<SerializationData>>>(m_kvPairArrays); };
	std::vector<std::string> *kvPairNullValues() { return lazyInit2<std::vector<std::string>>(m_kvPairNullValues); };

	std::vector<std::unique_ptr<SerializationData>> *arrayObjects() { return lazyInit2<std::vector<std::unique_ptr<SerializationData>>>(m_arrayObjects); };
	std::vector<std::unique_ptr<SerializationData>> *arrayArrays() { return lazyInit2<std::vector<std::unique_ptr<SerializationData>>>(m_arrayArrays); };
	std::vector<JsonParser::Number> *arrayNumbers() { return lazyInit2<std::vector<JsonParser::Number>>(m_arrayNumbers); };
	std::vector<bool> *arrayBools() { return lazyInit2<std::vector<bool>>(m_arrayBools); };
	std::vector<std::string> *arrayStrings() { return lazyInit2<std::vector<std::string>>(m_arrayStrings); };

private:
	std::unique_ptr<std::map<std::string, JsonParser::Number>> m_kvPairNumbers;
	std::unique_ptr<std::map<std::string, bool>> m_kvPairBools;
	std::unique_ptr<std::map<std::string, std::string>> m_kvPairStrings;
	std::unique_ptr<std::map<std::string, std::unique_ptr<SerializationData>>> m_kvPairObjects;
	std::unique_ptr<std::map<std::string, std::unique_ptr<SerializationData>>> m_kvPairArrays;
	std::unique_ptr<std::vector<std::string>> m_kvPairNullValues;

	std::unique_ptr<std::vector<std::unique_ptr<SerializationData>>> m_arrayObjects;
	std::unique_ptr<std::vector<std::unique_ptr<SerializationData>>> m_arrayArrays;
	std::unique_ptr<std::vector<JsonParser::Number>> m_arrayNumbers;
	std::unique_ptr<std::vector<bool>> m_arrayBools;
	std::unique_ptr<std::vector<std::string>> m_arrayStrings;

private:
	std::string *m_fullString;
	size_t m_strLen { 0 };
	JsonTypes m_type{ JsonTypes::Object };
};

inline std::string::const_iterator JsonParser::SerializationData::getChar(const size_t & pos) const
{
	if (strLen() > 0) {
		return this->m_fullString->begin() + pos;
	}
	return this->m_fullString->end();
}

inline size_t JsonParser::SerializationData::strLen() const
{
	return this->m_strLen;
}

inline bool JsonParser::SerializationData::matchChar(const size_t &i, char ch) const
{
	return *getChar(i) == ch;
}

} // namespace JsonParser

#endif // JSONPARSER_SERIALIZATIONDATA_H_