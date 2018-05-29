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
#ifndef SRC_SERIALIZATIONMAPPING_H_
#define SRC_SERIALIZATIONMAPPING_H_

#include <map>
#include <utility>
#include <vector>
#include <iterator>
#include <functional>
#include <memory>
#include <string>

#include "SerializationMappingData.h"
#include "JsonTypes.h"

namespace JsonParser {
class SerializationMapping :
	private JsonParser::SerializationMappingData
{
	public:
		DLLEXPORT explicit SerializationMapping();
		DLLEXPORT ~SerializationMapping() override {}

	public:
		DLLEXPORT bool fromString() override;
		DLLEXPORT bool fromString(const std::shared_ptr<JsonString> &str) override;

		DLLEXPORT JsonString toString() const override;
		DLLEXPORT JsonString toStringArray() const override;

	private:
		bool fromString2();
		DLLEXPORT bool fromStringArray() override;
		bool fromStringToArrayOfObjects();
		bool fromStringToArrayOfArrays();

	protected:
		template<typename T>
		DLLEXPORT T DataContract(JsonString name, T &value, const T &defaultValue)
		{
			addMember(name, value);
			return T(defaultValue);
		}

		DLLEXPORT void addMember(const JsonString &name,
			__int64 &memberVariable,
			bool optional = false);
		DLLEXPORT void addMember(const JsonString &name,
			double &memberVariable,
			bool optional = false);
		DLLEXPORT void addMember(const JsonString &name,
			JsonString &memberVariable,
			bool optional = false);
		DLLEXPORT void addMember(const JsonString &name,
			SerializationMapping &memberVariable,
			bool optional = false);
		DLLEXPORT void addMember(const JsonString &name,
			bool &memberVariable,
			bool optional = false);

		DLLEXPORT void addMember(const JsonString &name,
			JsonParser::Vector<JsonString> &memberVariable,
			bool optional = false);
		DLLEXPORT void addMember(const JsonString &name,
			JsonParser::Vector<JsonParser::Number> &memberVariable,
			bool optional = false);
		DLLEXPORT void addMember(const JsonString &name,
			JsonParser::Vector<bool> &memberVariable, bool optional = false);
		DLLEXPORT void addMember(const JsonString &name,
			JsonParser::VectorBase &memberVariable, bool optional = false);

	private:
		void addSerializableMember(const JsonString &name,
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

#endif  // SRC_SERIALIZATIONMAPPING_H_
