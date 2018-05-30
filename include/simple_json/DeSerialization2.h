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

#ifndef SIMPLE_JSON_DESERIALIZATION2_H_
#define SIMPLE_JSON_DESERIALIZATION2_H_

#include <string>
#include <map>
#include <vector>
#include <memory>

#include "JsonTypes.h"
#include "Number.h"
#include "ErrorHandler.h"
#include "SerializationData2.h"


namespace JsonParser {

	constexpr size_t constLength(const char* str)
	{
		return (*str == 0) ? 0 : constLength(str + 1) + 1;
	}

	class DeSerialization2 : public SerializationData2
	{
	public:
		DeSerialization2() {}
		virtual ~DeSerialization2() {}
		JsonString toString() const override;
		JsonString toStringArray() const override;

		virtual bool fromString(const std::shared_ptr<JsonString> &str);
		virtual bool fromString(std::istream *str);

		DLLEXPORT virtual bool fromString(char c = -1) override;

	private:
		bool fromStringArray(char c = -1);
		char parseStringArray();
		char addKVPair(char c);
		char addValue(const JsonString &name);

	private:
		bool isNumber(char c) const;
		bool isBool(char c) const;
		bool isNull(char c) const;
		bool checkEscape(char ch1, char ch2) const;
		bool getName(JsonString *name) const;

	private:
		bool addStringValue(const JsonString &name);
		bool addObjectValue(char c, const JsonString &name);
		bool addArrayValue(const JsonString &name);
		char addNumberValue(char c, const JsonString & name);
		bool addBoolValue(char c, const JsonString & name);
		void addNullValue(const JsonString & name);

	private:
		bool addArrayToArray();
		bool addStringToArray();
		bool addObjectToArray();
		char addNumberToArray(char c);
		bool addBoolToArray(char c);
	};
}  // namespace JsonParser

#endif  // SIMPLE_JSON_DESERIALIZATION_H_
