#pragma once
#ifndef JSONPARSER_SERIALIZABLE_H_
#define JSONPARSER_SERIALIZABLE_H_
#include <string>
#include <map>
#include <vector>

#include "JsonTypes.h"
#include "Number.h"

namespace JsonParser {

	class SerializationMapping;

	class NotImplemented : public std::logic_error
	{
		public:
			NotImplemented() : std::logic_error("Function not yet implemented") { }
	};

	constexpr size_t constLength(const char* str)
	{
		return (*str == 0) ? 0 : constLength(str + 1) + 1;
	}

	class Serializable
	{
		private:
			friend class JsonParser::SerializationMapping;
		public:
			explicit Serializable(Serializable * parent = nullptr);
			virtual ~Serializable();
			virtual std::string toString() const;
			virtual std::string toStringArray() const;
			bool fromString(const std::string &str);

		protected:
			size_t fromString(const size_t &pos);
			virtual bool fromString();

		private:
			size_t fromStringArray(const size_t &pos);
			size_t parseStringArray(const size_t &pos);
			bool parseString();
			size_t addKVPair(const size_t &pos);
			size_t addValue(const size_t &pos, const std::string &name);

		private:
			bool isNumber(const size_t &pos) const;
			bool isBool(const size_t &pos) const;
			bool isNull(const size_t &pos) const;
			size_t strLen() const;
			char getChar(const size_t &pos) const;
			inline bool matchChar(char ch1, char ch2) const;
			bool checkEscape(const size_t &pos) const;
			size_t getName(const size_t &pos, std::string &name) const;

		private:
			size_t addStringValue(const size_t &pos, const std::string &name);
			size_t addObjectValue(const size_t &pos, const std::string &name);
			size_t addArrayValue(const size_t &pos, const std::string &name);
			size_t addIntegerValue(const size_t &pos, const std::string &name);
			size_t addBoolValue(const size_t &pos, const std::string &name);
			size_t addNullValue(const size_t &pos, const std::string &name);

		private:
			size_t addArray(const size_t &pos);
			size_t addString(const size_t &pos);
			size_t addObject(const size_t &pos);
			size_t addInteger(const size_t &pos);
			size_t addBool(const size_t &pos);

		private:
			void setType(const JsonTypes &type);
			JsonTypes type() const;

		protected:
			std::map<std::string, JsonParser::Number> m_kvPairNumbers;
			std::map<std::string, bool> m_kvPairBools;
			std::map<std::string, std::string> m_kvPairStrings;
			std::map<std::string, Serializable *> m_kvPairObjects;
			std::map<std::string, Serializable *> m_kvPairArrays;
			std::vector<std::string> m_kvPairNullValues;

			std::vector<Serializable *> m_arrayObjects;
			std::vector<Serializable *> m_arrayArrays;
			std::vector<JsonParser::Number> m_arrayNumbers;
			std::vector<bool> m_arrayBools;
			std::vector<std::string> m_arrayStrings;

		private:
			Serializable * m_parent{ nullptr };
			std::string *m_fullString{ nullptr };
			JsonTypes m_type{ JsonTypes::Object };
	};

}

#endif  // JSONPARSER_SERIALIZABLE_H_
