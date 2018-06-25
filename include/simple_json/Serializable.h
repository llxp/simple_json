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
#ifndef SRC_SERIALIZABLE_H_
#define SRC_SERIALIZABLE_H_

#include "DeSerialization.h"
#include <string>
#include <simple_json\VectorBase.h>
#include <simple_json\Vector.h>

namespace simple_json {
	class Serializable : public JsonParser::DeSerialization
	{
	public:
		JsonString toString();
	protected:
		DLLEXPORT void addMember(JsonString &&name,
			__int64 &memberVariable,
			bool optional = false);
		DLLEXPORT void addMember(JsonString &&name,
			double &memberVariable,
			bool optional = false);
		DLLEXPORT void addMember(JsonString &&name,
			JsonString &memberVariable,
			bool optional = false);
		DLLEXPORT void addMember(JsonString &&name,
			Serializable &memberVariable,
			bool optional = false);
		DLLEXPORT void addMember(JsonString &&name,
			bool &memberVariable,
			bool optional = false);

		/*DLLEXPORT void addMember(JsonString &&name,
			JsonParser::Vector<JsonString> &memberVariable,
			bool optional = false);
		DLLEXPORT void addMember(JsonString &&name,
			JsonParser::Vector<JsonParser::Number> &memberVariable,
			bool optional = false);
		DLLEXPORT void addMember(JsonString &&name,
			JsonParser::Vector<bool> &memberVariable, bool optional = false);*/
		DLLEXPORT void addMember(JsonString &&name,
			JsonParser::VectorBase &memberVariable, bool optional = false);

	private:
		void addSerializableMember(JsonString &&name,
			JsonTypes type,
			bool optional = false);
	};
}  // namespace simple_json

#endif  // SRC_SERIALIZABLE_H_