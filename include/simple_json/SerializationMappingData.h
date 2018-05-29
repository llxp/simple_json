/*
MIT License

Copyright (c) 2018 Lukas Lüdke

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef SRC_SERIALIZATIONMAPPINGDATA_H_
#define SRC_SERIALIZATIONMAPPINGDATA_H_

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
		std::map<JsonString, JsonParser::Number> m_kvPairMappingNumbers;
		std::map<JsonString, bool *> m_kvPairMappingBools;
		std::map<JsonString, JsonString *> m_kvPairMappingStrings;
		std::map<JsonString, SerializationMappingData *> m_kvPairMappingObjects;
		std::map<JsonString, std::unique_ptr<SerializationMappingData>> m_kvPairMappingArrays;

		JsonParser::Vector<JsonString> *m_mappingStringArrays{ nullptr };
		JsonParser::Vector<bool> *m_mappingBoolArrays{ nullptr };
		JsonParser::Vector<JsonParser::Number> *m_mappingNumberArrays{ nullptr };
		JsonParser::VectorBase *m_mappingObjectArrays{ nullptr };
		JsonParser::VectorBase *m_mappingArrayArrays{ nullptr };

		std::vector<std::pair<JsonString, std::pair<JsonTypes, bool>>> m_serializableMembers;
};
}  // namespace JsonParser

#endif  // SRC_SERIALIZATIONMAPPINGDATA_H_