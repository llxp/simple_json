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
#ifndef SRC_JSONTYPES_H_
#define SRC_JSONTYPES_H_

enum JsonTypes : short
{
	None = 0,
	Number = 1,
	String = 2,
	Bool = 3,
	Object = 4,
	Null = 5,
	NumberArray = 6,
	StringArray = 7,
	BoolArray = 8,
	ObjectArray = 9,
	ArrayArray = 10
};

#define Stringify(val) #val

#define EmptyJsonArray Stringify([])
#define EmptyJsonObject Stringify({})
#define JsonKvSeparator ':'
#define JsonEntrySeparator ','
#define JsonObjectOpen '{'
#define JsonObjectClose '}'
#define JsonArrayOpen '['
#define JsonArrayClose ']'
#define JsonStringSeparator '"'

#define JsonKvSeparatorStr ":"
#define JsonEntrySeparatorStr ","
#define JsonObjectOpenStr "{"
#define JsonObjectCloseStr "}"
#define JsonArrayOpenStr "["
#define JsonArrayCloseStr "]"
#define JsonStringSeparatorStr "\""

typedef std::string JsonString;
typedef char JsonChar;

#define EmptyString ""
#define ToString std::to_string

#define ADD(MEMBER) addMember(std::move(#MEMBER), MEMBER);
#define ADD2(MEMBERNAME, MEMBER, OPT) addMember(MEMBERNAME, MEMBER, OPT)

constexpr short MAX_JSON_NUMBER_LENGTH = 325;

#endif  // SRC_JSONTYPES_H_
