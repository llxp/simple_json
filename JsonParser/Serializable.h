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
			size_t fromString(size_t pos);
			virtual bool fromString();

		private:
			size_t fromStringArray(size_t pos);
			size_t parseStringArray(size_t pos);
			bool checkString();
			bool matchChar(char ch1, char ch2);
			bool checkEscape(size_t pos);
			size_t addKVPair(size_t pos);
			size_t addName(size_t pos, std::string *name);
			size_t addValue(size_t pos, const std::string &name);
			bool isNumber(size_t pos);
			bool isBool(size_t pos);
			size_t strLen() const;
			char getChar(size_t pos) const;

		private:
			size_t addStringValue(size_t pos, const std::string &name);
			size_t addObjectValue(size_t pos, const std::string &name);
			size_t addArrayValue(size_t pos, const std::string &name);
			size_t addIntegerValue(size_t pos, const std::string &name);
			size_t addBoolValue(size_t pos, const std::string &name);

		private:
			size_t addArray(size_t pos);
			size_t addString(size_t pos);
			size_t addObject(size_t pos);
			size_t addInteger(size_t pos);
			size_t addBool(size_t pos);

		private:
			void setType(const JsonTypes &type);

		protected:
			std::map<std::string, JsonParser::Number> m_kvPairNumbers;
			std::map<std::string, bool> m_kvPairBools;
			std::map<std::string, std::string> m_kvPairStrings;
			std::map<std::string, Serializable *> m_kvPairObjects;
			std::map<std::string, Serializable *> m_kvPairArrays;

			std::vector<Serializable *> m_arrayObjects;
			std::vector<Serializable *> m_arrayArrays;
			std::vector<JsonParser::Number *> m_arrayNumbers;
			std::vector<bool *> m_arrayBools;
			std::vector<std::string *> m_arrayStrings;

			Serializable * m_parent{ nullptr };
			std::string *m_fullString{ nullptr };
			JsonTypes m_type{ JsonTypes::Object };
	};

}

#endif  // JSONPARSER_SERIALIZABLE_H_
