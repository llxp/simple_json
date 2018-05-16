#pragma once
#ifndef JSONPARSER_SERIALIZATIONMAPPING_H_
#define JSONPARSER_SERIALIZATIONMAPPING_H_

#include <map>
#include <utility>
#include <vector>
#include <iterator>
#include <functional>

#include "SerializationMappingData.h"

#define ADD(MEMBER) addMember(#MEMBER, MEMBER);

namespace JsonParser {

	class SerializationMapping :
		public JsonParser::SerializationMappingData
	{
		public:
			explicit SerializationMapping();
			~SerializationMapping() {}

		public:
			bool fromString() override;
			bool fromString(std::shared_ptr<std::string> str) override;

			virtual std::string toString() const override;
			virtual std::string toStringArray() const override;

	private:
		bool fromString2();
		bool fromStringArray() override;
		bool fromStringArrayObjects();
		bool fromStringArrayArrays();

		protected:
			template<typename T>
			T DataContract(std::string name, T &value, const T &defaultValue)
			{
				addMember(name, value);
				return T(defaultValue);
			}

			void addSerializableMember(const std::string &name, JsonTypes type, bool optional = false);

			void addMember(const std::string &name,
				__int64 &memberVariable, bool optional = false);
			void addMember(const std::string &name,
				std::string &memberVariable, bool optional = false);
			void addMember(const std::string &name,
				SerializationMapping &memberVariable, bool optional = false);
			void addMember(const std::string &name,
				bool &memberVariable, bool optional = false);

			void addMember(const std::string &name,
				JsonParser::Vector<std::string> &memberVariable, bool optional = false);
			void addMember(const std::string &name,
				JsonParser::Vector<JsonParser::Number> &memberVariable, bool optional = false);
			void addMember(const std::string &name,
				JsonParser::Vector<bool> &memberVariable, bool optional = false);
			void addMember(const std::string &name,
				JsonParser::VectorBase &memberVariable, bool optional = false);

		private:
			template<typename T1, typename T2>
			std::string makeStr3(const T2 *vec, const std::function<std::string(const std::shared_ptr<T1> & element)> &lambda) const
			{
				std::string outputStr("[");
				for (auto it = vec->begin(); it != vec->end(); it++) {
					auto currentElement = std::static_pointer_cast<T1>(*it);
					if (currentElement != nullptr) {
						outputStr += lambda(currentElement);
						if (it + 1 != vec->end()) {
							outputStr += ',';
						}
					}
				}
				outputStr += ']';
				return outputStr;
			}
			std::string makeString(const std::string &str) const;
			std::string makeKvPairStrNumber(const std::string &name,
				const JsonParser::Number &value) const;
			std::string makeKvPairStrString(const std::string &name,
				std::string *value) const;
			std::string makeKvPairStrObject(const std::string &name,
				SerializationMappingData *value) const;
			std::string makeKvPairStrBool(const std::string &name,
				bool value) const;
			std::string makeKvPairStrArray(const std::string &name,
				const std::shared_ptr<JsonParser::SerializationMapping> & value) const;

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

		private:
			void assignKvPairNumbers();
			void assignKvPairBools();
			void assignKvPairStrings();
			bool assignKvPairObjects();
			bool assignKvPairArrays();
	};
}

#endif  // JSONPARSER_SERIALIZATIONMAPPING_H_
