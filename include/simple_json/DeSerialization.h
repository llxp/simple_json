/*
MIT License

Copyright (c) 2018 Lukas L�dke

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
#include <thread>
#include <mutex>

#include "JsonTypes.h"
#include "Number.h"
#include "ErrorHandler.h"
#include "SerializationData.h"
#include "Vector.h"


namespace JsonParser {

	class DeSerialization : public SerializationData
	{

	public:
		DLLEXPORT virtual ~DeSerialization() {}
		void clearMapping();
		// Inherited via SerializationData
		DLLEXPORT JsonString toString() const override;
		// Inherited via SerializationData
		DLLEXPORT virtual bool fromString(JsonString * string) override;
		DLLEXPORT bool parseString(size_t &pos) override;
		// Inherited via SerializationData
		DLLEXPORT virtual void refresh() override;

	private:
		void clearValues();

	private:
		// Inherited via SerializationData
		bool addArrayValues(size_t &pos);
		bool addKVPair(size_t &pos);
		bool addObjectValues(size_t &pos);
		bool addValue(size_t &pos, JsonString &&name);

	private:
		bool addStringValue(size_t &pos, JsonString &&name);
		bool addObjectValue(size_t &pos, JsonString &&name);
		bool addArrayValue(size_t &pos, JsonString &&name);
		bool addNumberValue(size_t &pos, JsonString &&name);
		bool addBoolValue(size_t &pos, JsonString &&name);
		bool addNullValue(size_t &pos, JsonString &&name);

	private:
		bool addArrayToArray(size_t &pos);
		bool addStringToArray(size_t &pos);
		bool addObjectToArray(size_t &pos);
		bool addNumberToArray(size_t &pos);
		bool addBoolToArray(size_t &pos);

	private:
		JsonString makeKvPairStrBool(const JsonString &name, bool value) const;

		// Inherited via SerializationData
		virtual void serialize() override {}
	};
}  // namespace JsonParser

#endif  // SIMPLE_JSON_DESERIALIZATION_H_
