#pragma once

#include <vector>
#include <map>
#include <memory>

#include "Number.h"
#include "JsonTypes.h"

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

	public:
		void setType(const JsonTypes &type);
		JsonTypes type() const;
		void setFullString(std::shared_ptr<std::string> str);
		std::shared_ptr<std::string> fullString() const;
		char getChar(const size_t &pos) const;
		size_t strLen() const;
		void clearAll();
		bool matchChar(const size_t &i, char ch) const;

		void assign(const std::shared_ptr<SerializationData> &other);

	protected:
		std::map<std::string, JsonParser::Number> m_kvPairNumbers;
		std::map<std::string, bool> m_kvPairBools;
		std::map<std::string, std::string> m_kvPairStrings;
		std::map<std::string, std::shared_ptr<SerializationData>> m_kvPairObjects;
		std::map<std::string, std::shared_ptr<SerializationData>> m_kvPairArrays;
		std::vector<std::string> m_kvPairNullValues;

		std::vector<std::shared_ptr<SerializationData>> m_arrayObjects;
		std::vector<std::shared_ptr<SerializationData>> m_arrayArrays;
		std::vector<JsonParser::Number> m_arrayNumbers;
		std::vector<bool> m_arrayBools;
		std::vector<std::string> m_arrayStrings;

	private:
		std::shared_ptr<std::string> m_fullString{ nullptr };
		JsonTypes m_type{ JsonTypes::Object };
	};
}

