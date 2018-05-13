#pragma once
#ifndef JSONPARSER_SERIALIZATIONMAPPING_H_
#define JSONPARSER_SERIALIZATIONMAPPING_H_

#include <map>
#include <utility>
#include <vector>
#include <iterator>

#include "Serializable.h"
#include "Vector.h"

#define ADD(MEMBER) addMember(#MEMBER, MEMBER);

namespace JsonParser {

	class SerializationMapping :
		private JsonParser::Serializable
	{
		public:
			explicit SerializationMapping(
				SerializationMapping *parent = nullptr);
			~SerializationMapping() {}

		private:
			std::map<std::string, JsonParser::Number *> m_kvPairMappingNumbers;
			std::map<std::string, bool *> m_kvPairMappingBools;
			std::map<std::string, std::string *> m_kvPairMappingStrings;
			std::map<std::string, SerializationMapping *> m_kvPairMappingObjects;
			std::map<std::string, SerializationMapping *> m_kvPairMappingArrays;

			JsonParser::Vector<std::string> *m_mappingStringArrays{ nullptr };
			JsonParser::Vector<bool> *m_mappingBoolArrays{ nullptr };
			JsonParser::Vector<JsonParser::Number> *m_mappingNumberArrays{ nullptr };
			JsonParser::VectorBase *m_mappingObjectArrays{ nullptr };
			JsonParser::VectorBase *m_mappingArrayArrays{ nullptr };

			std::vector<std::pair<std::string, JsonTypes>> m_serializableMembers;

		public:
			bool fromString() override;
			bool fromStringArray();
			bool fromString(const std::string &str);

			virtual std::string toString() const override;
			virtual std::string toStringArray() const override;

		protected:
			template<typename T>
			auto DataContract(std::string name, T value)
			{
				T* newObj = new T(value);
				addMember(name, *newObj);
				return newObj;
			}

			void addMember(const std::string &name,
				__int64 &memberVariable);
			void addMember(const std::string &name,
				std::string &memberVariable);
			void addMember(const std::string &name,
				SerializationMapping &memberVariable);
			void addMember(const std::string &name,
				bool &memberVariable);

			void addMember(const std::string &name,
				JsonParser::Vector<std::string> &memberVariable);
			void addMember(const std::string &name,
				JsonParser::Vector<JsonParser::Number> &memberVariable);
			void addMember(const std::string &name,
				JsonParser::Vector<bool> &memberVariable);
			void addMember(const std::string &name,
				JsonParser::VectorBase *memberVariable);

		private:
			std::string makeString(const std::string &str) const;
			std::string makeKvPairStrNumber(const std::string &name,
				JsonParser::Number *value) const;
			std::string makeKvPairStrString(const std::string &name,
				std::string *value) const;
			std::string makeKvPairStrObject(const std::string &name,
				SerializationMapping *value) const;
			std::string makeKvPairStrBool(const std::string &name,
				bool value) const;
			std::string makeKvPairStrArray(const std::string &name,
				SerializationMapping *value) const;

			std::string makeStrObjectArray(
				const JsonParser::VectorBase *value) const;
			std::string makeStrNumberArray(
				const JsonParser::Vector<JsonParser::Number> *value) const;
			std::string makeStrBoolArray(
				const JsonParser::Vector<bool> *value) const;
			std::string makeStrStringArray(
				const JsonParser::Vector<std::string> *value) const;
			std::string makeStrArrayArray(
				const JsonParser::VectorBase *value) const;

			SerializationMapping *assign(
				Serializable *other)
			{
				//this->m_isParsed = other->m_isParsed;
				this->m_type = other->m_type;
				this->m_parent = other->m_parent;
				this->m_fullString = other->m_fullString;

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
				return this;
			}
	};
}

#endif  // JSONPARSER_SERIALIZATIONMAPPING_H_
