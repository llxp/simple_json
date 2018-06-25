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

#include <string>
#include <iostream>

enum JsonTypes : uint8_t
{
	None,
	Number,
	String,
	Bool,
	Object,
	Null,
	Array
	/*NumberArray = 6,
	StringArray = 7,
	BoolArray = 8,
	ObjectArray = 9,
	ArrayArray = 10*/
};

#define Stringify(val) #val

static const char * EmptyJsonArray = Stringify([]);
static const char * EmptyJsonObject = Stringify({});
static constexpr char JsonKvSeparator = ':';
static constexpr char JsonEntrySeparator = ',';
static constexpr char JsonObjectOpen = '{';
static constexpr char JsonObjectClose = '}';
static constexpr char JsonArrayOpen = '[';
static constexpr char JsonArrayClose = ']';
static constexpr char JsonStringSeparator = '"';

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

#define ADD(MEMBER) addMember(#MEMBER, MEMBER);
#define ADD2(MEMBERNAME, MEMBER, OPT) addMember(MEMBERNAME, MEMBER, OPT)

static constexpr short MAX_JSON_NUMBER_LENGTH = 325;

static size_t lineCounter = 0;

constexpr size_t constLength(const char* str)
{
	return (*str == 0) ? 0 : constLength(str + 1) + 1;
}

inline void printDebugMessage(const std::string &function, int line)
{
#ifdef _DEBUG
	std::cout << "line: " << lineCounter << std::endl;
	std::cout << function << " : " << line << std::endl;
#else
	function;
	line;
#endif
}

inline void* operator new(std::size_t sz) {
	try {
		//std::printf("global op new called, size = %zu\n", sz);
		return std::malloc(sz);
	}
	catch (std::bad_alloc) {
		return nullptr;
	}
	return nullptr;
}

#endif  // SRC_JSONTYPES_H_
