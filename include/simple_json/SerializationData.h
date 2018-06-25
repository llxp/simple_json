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

#ifndef JSON_SERIALIZATIONDATA_H_
#define JSON_SERIALIZATIONDATA_H_

#include <vector>
#include <map>
#include <memory>
#include <string>

#include "Number.h"
#include "JsonTypes.h"
#include <simple_json/SerializableBase.h>
#include <simple_json/StringContainer.h>
#include <simple_json/VectorBase.h>

#ifdef __GNUC__
#define __cdecl __attribute__((__cdecl__))
#define __int64 long long
#endif

namespace JsonParser {

	class SerializationData : public SerializableBase, public StringContainer
	{
	public:
		DLLEXPORT virtual ~SerializationData();
		DLLEXPORT virtual void serialize() = 0;
		DLLEXPORT virtual void refresh() = 0;

	protected:
		std::map<JsonString, JsonParser::SerializationData *> m_kvPairObjects;
		std::map<JsonString, bool *> m_kvPairBools;
		std::map<JsonString, JsonString *> m_kvPairStrings;
		std::map<JsonString, JsonParser::Number *> m_kvPairNumbers;
		std::map<JsonString, JsonParser::VectorBase *> m_kvPairArrays;
		std::vector<void *> m_collectibleObjects;

		std::vector<std::pair<JsonString,
			std::pair<JsonTypes, bool>>> m_serializableMembers;
		
	protected:
		void setType(const JsonTypes &type);
		JsonTypes type() const;

	private:
		JsonTypes m_type{ JsonTypes::Object };
	};
}  // namespace JsonParser
#endif  // JSON_SERIALIZATIONDATA_H_
