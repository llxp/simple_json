#pragma once
#include <map>
#include <string>
#include <vector>

#include "JsonTypes.h"
#include "Vector.h"
#include "Number.h"
#include "DeSerialization.h"

namespace JsonParser {
	class SerializationMappingData : public DeSerialization
	{
	public:
		explicit SerializationMappingData();
		DLLEXPORT virtual ~SerializationMappingData();

	protected:
		virtual bool fromStringArray() = 0;
		virtual bool fromString() = 0;

	protected:
		std::map<std::string, JsonParser::Number> m_kvPairMappingNumbers;
		std::map<std::string, bool *> m_kvPairMappingBools;
		std::map<std::string, std::string *> m_kvPairMappingStrings;
		std::map<std::string, SerializationMappingData *> m_kvPairMappingObjects;
		std::map<std::string, std::unique_ptr<SerializationMappingData>> m_kvPairMappingArrays;

		JsonParser::Vector<std::string> *m_mappingStringArrays{ nullptr };
		JsonParser::Vector<bool> *m_mappingBoolArrays{ nullptr };
		JsonParser::Vector<JsonParser::Number> *m_mappingNumberArrays{ nullptr };
		JsonParser::VectorBase *m_mappingObjectArrays{ nullptr };
		JsonParser::VectorBase *m_mappingArrayArrays{ nullptr };

		std::vector<std::pair<std::string, std::pair<JsonTypes, bool>>> m_serializableMembers;
	};
}