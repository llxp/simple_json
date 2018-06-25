#ifndef SIMPLE_JSON_SERIALIZABLE_BASE_H_
#define SIMPLE_JSON_SERIALIZABLE_BASE_H_

#include <iostream>

#include <simple_json/dll.h>
#include <simple_json/JsonTypes.h>

namespace JsonParser {
class SerializableBase
{
public:
	DLLEXPORT virtual ~SerializableBase() {}

public:
	DLLEXPORT virtual JsonString toString() const = 0;
	DLLEXPORT virtual bool fromString(JsonString *string) = 0;
	DLLEXPORT virtual bool parseString(size_t &) = 0;
};
}  // namespace JsonParser

#endif  // SIMPLE_JSON_SERIALIZABLE_BASE_H_