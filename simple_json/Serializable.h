#pragma once
#include "SerializationMapping.h"

namespace simple_json {
	class Serializable : public JsonParser::SerializationMapping
	{
	public:
		Serializable() {}
		~Serializable() {}

	public:
		DLLEXPORT bool fromString();
		DLLEXPORT bool fromString(const std::shared_ptr<std::string> &str);

		DLLEXPORT virtual std::string toString() const;
		DLLEXPORT virtual std::string toStringArray() const;

	protected:
		DLLEXPORT void addMember(const std::string &name,
			__int64 &memberVariable,
			bool optional = false);
		DLLEXPORT void addMember(const std::string &name,
			std::string &memberVariable,
			bool optional = false);
		DLLEXPORT void addMember(const std::string &name,
			SerializationMapping &memberVariable,
			bool optional = false);
		DLLEXPORT void addMember(const std::string &name,
			bool &memberVariable,
			bool optional = false);

		DLLEXPORT void addMember(const std::string &name,
			JsonParser::Vector<std::string> &memberVariable,
			bool optional = false);
		DLLEXPORT void addMember(const std::string &name,
			JsonParser::Vector<JsonParser::Number> &memberVariable,
			bool optional = false);
		DLLEXPORT void addMember(const std::string &name,
			JsonParser::Vector<bool> &memberVariable, bool optional = false);
		DLLEXPORT void addMember(const std::string &name,
			JsonParser::VectorBase &memberVariable, bool optional = false);

		template<typename T>
		DLLEXPORT T DataContract(std::string name, T &value, const T &defaultValue)
		{
			addMember(name, value);
			return T(defaultValue);
		}
	};

}