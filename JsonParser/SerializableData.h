#pragma once

#include <vector>
#include <map>
#include <memory>

#include "Number.h"
#include "JsonTypes.h"

namespace JsonParser {

	class SerializationMapping;

	class SerializableData
	{
	public:
		explicit SerializableData();
		~SerializableData();

	private:
		friend class JsonParser::SerializationMapping;

	public:
		virtual bool fromString() = 0;

	public:
		void setType(const JsonTypes &type);
		JsonTypes type() const;
		void setFullString(std::string *str);
		std::string * const fullString() const;
		char getChar(const size_t &pos) const;
		size_t strLen() const;

		void assign(std::shared_ptr<SerializableData> other)
		{
			this->setType(other->type());
			this->setFullString(other->fullString());

			this->m_kvPairNumbers = other->m_kvPairNumbers;
			this->m_kvPairStrings = other->m_kvPairStrings;
			this->m_kvPairBools = other->m_kvPairBools;
			this->m_kvPairObjects = other->m_kvPairObjects;
			this->m_kvPairArrays = other->m_kvPairArrays;

			this->m_arrayObjects = other->m_arrayObjects;
			this->m_arrayArrays = other->m_arrayArrays;
			this->m_arrayNumbers = other->m_arrayNumbers;
			this->m_arrayBools = other->m_arrayBools;
			this->m_arrayStrings = other->m_arrayStrings;
		}

	public:
		std::map<std::string, JsonParser::Number> m_kvPairNumbers;
		std::map<std::string, bool> m_kvPairBools;
		std::map<std::string, std::string> m_kvPairStrings;
		std::map<std::string, std::shared_ptr<SerializableData>> m_kvPairObjects;
		std::map<std::string, std::shared_ptr<SerializableData>> m_kvPairArrays;
		std::vector<std::string> m_kvPairNullValues;

		std::vector<std::shared_ptr<SerializableData>> m_arrayObjects;
		std::vector<std::shared_ptr<SerializableData>> m_arrayArrays;
		std::vector<JsonParser::Number> m_arrayNumbers;
		std::vector<bool> m_arrayBools;
		std::vector<std::string> m_arrayStrings;

	private:
		std::string * m_fullString{ nullptr };
		JsonTypes m_type{ JsonTypes::Object };
	};
}

