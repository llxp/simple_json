#pragma once
#ifndef JSONPARSER_JSONTYPES_H_
#define JSONPARSER_JSONTYPES_H_

enum JsonTypes
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

#define EmptyJsonArray "[]"
#define EmptyJsonObject "{}"
#define JsonKvSeparator ':'
#define JsonEntrySeparator ','
#define JsonObjectOpen '{'
#define JsonObjectClose '}'
#define JsonArrayOpen '['
#define JsonArrayClose ']'
#define JsonStringSeparator '"'

#endif  // JSONPARSER_JSONTYPES_H_