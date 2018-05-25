#include "Serializable.h"

DLLEXPORT bool simple_json::Serializable::fromString()
{
	return JsonParser::SerializationMapping::fromString();
}

DLLEXPORT bool simple_json::Serializable::fromString(const std::shared_ptr<std::string>& str)
{
	return JsonParser::SerializationMapping::fromString(str);
}

DLLEXPORT std::string simple_json::Serializable::toString() const
{
	return JsonParser::SerializationMapping::toString();
}

DLLEXPORT std::string simple_json::Serializable::toStringArray() const
{
	return JsonParser::SerializationMapping::toStringArray();
}

void simple_json::Serializable::addMember(
	const std::string & name, __int64 & memberVariable, bool optional)
{
	JsonParser::SerializationMapping::addMember(name, memberVariable, optional);
}

void simple_json::Serializable::addMember(
	const std::string & name, std::string & memberVariable, bool optional)
{
	JsonParser::SerializationMapping::addMember(name, memberVariable, optional);
}

void simple_json::Serializable::addMember(
	const std::string & name,
	SerializationMapping & memberVariable,
	bool optional)
{
	JsonParser::SerializationMapping::addMember(name, memberVariable, optional);
}

void simple_json::Serializable::addMember(
	const std::string & name, bool & memberVariable, bool optional)
{
	JsonParser::SerializationMapping::addMember(name, memberVariable, optional);
}

void simple_json::Serializable::addMember(
	const std::string & name,
	JsonParser::Vector<std::string>& memberVariable, bool optional)
{
	JsonParser::SerializationMapping::addMember(name, memberVariable, optional);
}

void simple_json::Serializable::addMember(
	const std::string & name,
	JsonParser::Vector<bool> & memberVariable, bool optional)
{
	JsonParser::SerializationMapping::addMember(name, memberVariable, optional);
}

void simple_json::Serializable::addMember(
	const std::string & name,
	JsonParser::Vector<JsonParser::Number> &memberVariable, bool optional)
{
	JsonParser::SerializationMapping::addMember(name, memberVariable, optional);
}

void simple_json::Serializable::addMember(
	const std::string & name,
	JsonParser::VectorBase &memberVariable, bool optional)
{
	JsonParser::SerializationMapping::addMember(name, memberVariable, optional);
}