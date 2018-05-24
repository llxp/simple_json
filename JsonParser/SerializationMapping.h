#ifndef JSONPARSER_SERIALIZATIONMAPPING_H_
#define JSONPARSER_SERIALIZATIONMAPPING_H_

#include <map>
#include <utility>
#include <vector>
#include <iterator>
#include <functional>
#include <memory>

#include "SerializationMappingData.h"

#define ADD(MEMBER) addMember(#MEMBER, MEMBER);

namespace JsonParser {

	class SerializationMapping :
		private JsonParser::SerializationMappingData
	{
		
		public:
			explicit SerializationMapping();
			~SerializationMapping() override {}

		public:
			bool fromString() override;
			bool fromString(const std::shared_ptr<std::string> &str) override;

			virtual std::string toString() const override;
			virtual std::string toStringArray() const override;

	private:
		bool fromString2();
		bool fromStringArray() override;
		bool fromStringToArrayOfObjects();
		bool fromStringToArrayOfArrays();

		protected:
			template<typename T>
			T DataContract(std::string name, T &value, const T &defaultValue)
			{
				addMember(name, value);
				return T(defaultValue);
			}

			void addMember(const std::string &name,
				__int64 &memberVariable,
				bool optional = false);
			void addMember(const std::string &name,
				std::string &memberVariable,
				bool optional = false);
			void addMember(const std::string &name,
				SerializationMapping &memberVariable,
				bool optional = false);
			void addMember(const std::string &name,
				bool &memberVariable,
				bool optional = false);

			void addMember(const std::string &name,
				JsonParser::Vector<std::string> &memberVariable,
				bool optional = false);
			void addMember(const std::string &name,
				JsonParser::Vector<JsonParser::Number> &memberVariable,
				bool optional = false);
			void addMember(const std::string &name,
				JsonParser::Vector<bool> &memberVariable, bool optional = false);
			void addMember(const std::string &name,
				JsonParser::VectorBase &memberVariable, bool optional = false);

		private:
			void addSerializableMember(const std::string &name,
				JsonTypes type,
				bool optional = false);

		private:
			void assignKvPairNumbers();
			void assignKvPairBools();
			void assignKvPairStrings();
			bool assignKvPairObjects();
			bool assignKvPairArrays();
	};
}  // namespace JsonParser

#endif  // JSONPARSER_SERIALIZATIONMAPPING_H_
