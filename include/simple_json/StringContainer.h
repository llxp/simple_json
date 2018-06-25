#ifndef SIMPLE_JSON_STRING_CONTAINER_H_
#define SIMPLE_JSON_STRING_CONTAINER_H_

#include <string>

#include <simple_json/dll.h>
#include <simple_json/JsonTypes.h>

namespace JsonParser {
	class StringContainer
	{
	public:
		DLLEXPORT virtual ~StringContainer() {}

	protected:
		DLLEXPORT inline char getChar(size_t pos) const;
		DLLEXPORT inline size_t strLen() const;
		DLLEXPORT inline char * substr(size_t pos, size_t len) const;
		DLLEXPORT bool getString(size_t &currentPos, JsonString &name) const;
		DLLEXPORT static bool isNumber(char c);
		DLLEXPORT void setString(const char *string);
		DLLEXPORT void setStrLen(size_t string);
		DLLEXPORT const char * string() const;

	private:
		static inline bool isEscape(char ch1, char ch2);

	public:
		DLLEXPORT void assign(const StringContainer *other);

	private:
		const char * m_string{ nullptr };
		size_t m_len;
	};
}  // namespace JsonParser

#endif  // SIMPLE_JSON_STRING_CONTAINER_H_