#include <simple_json/StringContainer.h>

#include <simple_json/dll.h>

DLLEXPORT void JsonParser::StringContainer::setString(const char * string)
{
	this->m_string = string;
}

DLLEXPORT void JsonParser::StringContainer::setStrLen(size_t string)
{
	this->m_len = string;
}

DLLEXPORT const char * JsonParser::StringContainer::string() const
{
	return this->m_string;
}

DLLEXPORT bool JsonParser::StringContainer::getString(size_t &currentPos, JsonString &name) const
{
	const size_t tempLen(strLen());
	char cOld1(0);
	char cOld2(0);
	bool beginFound(false);
	for (size_t i = currentPos; i < tempLen; ++i) {
		char &&c = getChar(i);
		if (c == JsonStringSeparator
			&& !isEscape(cOld1, cOld2)
			&& !beginFound) {
			beginFound = true;
		} else if (c == JsonStringSeparator
			&& !isEscape(cOld1, cOld2)
			&& beginFound) {
			size_t len(i - (currentPos + 1));
			if (len > 0 && currentPos + 1 + len < tempLen) {
				name = std::move(substr(currentPos + 1, len));
			}
			currentPos = i;
			return true;
		}
		cOld2 = cOld1;
		cOld1 = c;
	}
	printDebugMessage(__FUNCTION__, __LINE__);
	return false;
}

bool JsonParser::StringContainer::isEscape(
	char ch1, char ch2)
{
	bool currentPosIsEscape(ch1 == '\\');
	if (currentPosIsEscape) {
		bool previousPosIsEscaped(ch2 == '\\');
		return !previousPosIsEscaped;
	}
	return currentPosIsEscape;
}

DLLEXPORT void JsonParser::StringContainer::assign(const StringContainer * other)
{
	this->m_string = other->m_string;
	this->m_len = other->m_len;
}

DLLEXPORT char JsonParser::StringContainer::getChar(size_t pos) const
{
	return (this->m_string)[pos];
}

DLLEXPORT size_t JsonParser::StringContainer::strLen() const
{
	return this->m_len;
}

DLLEXPORT char * JsonParser::StringContainer::substr(size_t pos, size_t len) const
{
	char *subbuff(static_cast<char *>(malloc(sizeof(char) * len + 1)));
	memcpy(subbuff, &this->m_string[pos], len);
	subbuff[len] = '\0';
	return subbuff;
}

DLLEXPORT bool JsonParser::StringContainer::isNumber(char c)
{
	switch (c) {
	case '.':
	case '-':
	case 'e':
		return true;
	default:
		if ((c >= 48
			&& c <= 57)) {
			return true;
		}
		return false;
	}
}