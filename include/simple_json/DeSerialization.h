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

#ifndef SIMPLE_JSON_DESERIALIZATION_H_
#define SIMPLE_JSON_DESERIALIZATION_H_

#include <string>
#include <map>
#include <vector>
#include <memory>

#include "JsonTypes.h"
#include "Number.h"
#include "ErrorHandler.h"
#include "SerializationData.h"
#include "Vector.h"


namespace JsonParser {

	class DeSerialization : public SerializationData
	{
	protected:
		DLLEXPORT JsonString toString() const override;
		DLLEXPORT JsonString toStringArray() const override;

		DLLEXPORT virtual bool fromString(const std::shared_ptr<JsonString> &str);
		DLLEXPORT bool fromString() override;

	protected:
		void clearMapping();
		void serialize() override {}

		void addMember(JsonString &&name,
			__int64 &memberVariable,
			bool optional = false);
		void addMember(JsonString &&name,
			double &memberVariable,
			bool optional = false);
		void addMember(JsonString &&name,
			JsonString &memberVariable,
			bool optional = false);
		void addMember(JsonString &&name,
			DeSerialization &memberVariable,
			bool optional = false);
		void addMember(JsonString &&name,
			bool &memberVariable,
			bool optional = false);

		void addMember(JsonString &&name,
			JsonParser::Vector<JsonString> &memberVariable,
			bool optional = false);
		void addMember(JsonString &&name,
			JsonParser::Vector<JsonParser::Number> &memberVariable,
			bool optional = false);
		void addMember(JsonString &&name,
			JsonParser::Vector<bool> &memberVariable, bool optional = false);
		void addMember(JsonString &&name,
			JsonParser::VectorBase &memberVariable, bool optional = false);

	private:
		void addSerializableMember(JsonString &&name,
			JsonTypes type,
			bool optional = false);
		void clearValues();

	private:
		size_t fromString(const size_t &pos);
		size_t fromStringArray(const size_t &pos);
		size_t addArrayValues(const size_t &pos);
		size_t addKVPair(const size_t &pos);
		size_t addValue(const size_t &pos, JsonString &&name);

	private:
		static inline bool isNumber(char number);
		static inline bool isBool(char c);
		static inline bool isNull(char c);
		inline bool isEscape(const size_t &pos) const;
		inline size_t getString(const size_t &pos, JsonString &name) const;

	private:
		size_t addStringValue(const size_t &pos, JsonString &&name);
		size_t addObjectValue(const size_t &pos, JsonString &&name);
		size_t addArrayValue(const size_t &pos, JsonString &&name);
		size_t addNumberValue(const size_t &pos, const JsonString &name);
		size_t addBoolValue(const size_t &pos, JsonString &&name);
		void addNullValue(JsonString &&name);

	private:
		size_t addArrayToArray(const size_t &pos);
		size_t addStringToArray(const size_t &pos);
		size_t addObjectToArray(const size_t &pos);
		size_t addNumberToArray(const size_t &pos);
		size_t addBoolToArray(const size_t &pos);
	};
}  // namespace JsonParser

#endif  // SIMPLE_JSON_DESERIALIZATION_H_
