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
	Array = 5,
	NumberArray = 6,
	StringArray = 7,
	BoolArray = 8,
	ObjectArray = 9,
	ArrayArray = 10
};

#endif  // JSONPARSER_JSONTYPES_H_