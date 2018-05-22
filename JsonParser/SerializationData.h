#pragma once
#ifndef JSONPARSER_SERIALIZATIONDATA_H_
#define JSONPARSER_SERIALIZATIONDATA_H_

#include <vector>
#include <map>
#include <memory>

#include "Number.h"
#include "JsonTypes.h"

#ifdef __linux__
#define __int64 long long
#endif

namespace JsonParser {

class SerializationMapping;

class SerializationData
{
public:
	explicit SerializationData();
	~SerializationData();

public:
	virtual bool fromString() = 0;
	virtual std::string toString() const = 0;

protected:
	void setType(const JsonTypes &type);
	JsonTypes type() const;
	void setFullString(std::string *str);
	std::string *fullString() const;
	inline char getChar(const size_t &pos) const;
	inline size_t strLen() const;
	void clearAll();
	inline bool matchChar(const size_t &i, char ch) const;

	void assign(const std::unique_ptr<SerializationData> &other);

protected:
	std::map<std::string, JsonParser::Number> m_kvPairNumbers;
	std::map<std::string, bool> m_kvPairBools;
	std::map<std::string, std::string> m_kvPairStrings;
	std::map<std::string, std::unique_ptr<SerializationData>> m_kvPairObjects;
	std::map<std::string, std::unique_ptr<SerializationData>> m_kvPairArrays;
	std::vector<std::string> m_kvPairNullValues;

	std::vector<std::unique_ptr<SerializationData>> m_arrayObjects;
	std::vector<std::unique_ptr<SerializationData>> m_arrayArrays;
	std::vector<JsonParser::Number> m_arrayNumbers;
	std::vector<bool> m_arrayBools;
	std::vector<std::string> m_arrayStrings;

private:
	std::string *m_fullString;
	size_t m_strLen { 0 };
	JsonTypes m_type{ JsonTypes::Object };
};

inline char JsonParser::SerializationData::getChar(const size_t & pos) const
{
	if (strLen() > 0) {
		return this->m_fullString->at(pos);
	}
	return 0;
}

inline size_t JsonParser::SerializationData::strLen() const
{
	return this->m_strLen;//this->m_fullString->size();
}

inline bool JsonParser::SerializationData::matchChar(const size_t &i, char ch) const
{
	return getChar(i) == ch;
}

} // namespace JsonParser

#endif // JSONPARSER_SERIALIZATIONDATA_H_