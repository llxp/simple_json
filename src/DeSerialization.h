#ifndef SRC_DESERIALIZATION_H_
#define SRC_DESERIALIZATION_H_

#include <string>
#include <map>
#include <vector>
#include <memory>

#include "JsonTypes.h"
#include "Number.h"
#include "ErrorHandler.h"
#include "SerializationData.h"


namespace JsonParser {

	constexpr size_t constLength(const char* str)
	{
		return (*str == 0) ? 0 : constLength(str + 1) + 1;
	}

class DeSerialization : public SerializationData
{
	public:
		DeSerialization();
		virtual ~DeSerialization();
		std::string toString() const override;
		std::string toStringArray() const override;
		virtual bool fromString(const std::shared_ptr<std::string> &str);
		bool fromString() override;

	protected:
		size_t fromString(const size_t &pos);

	private:
		size_t fromStringArray(const size_t &pos);
		size_t parseStringArray(const size_t &pos);
		bool parseString();
		size_t addKVPair(const size_t &pos);
		size_t addValue(const size_t &pos, const std::string &name);

	private:
		bool isNumber(const size_t &pos) const;
		bool isBool(const size_t &pos) const;
		bool isNull(const size_t &pos) const;
		bool checkEscape(const size_t &pos) const;
		size_t getName(const size_t &pos, std::string &name) const;

	private:
		size_t addStringValue(const size_t &pos, const std::string &name);
		size_t addObjectValue(const size_t &pos, const std::string &name);
		size_t addArrayValue(const size_t &pos, const std::string &name);
		size_t addIntegerValue(const size_t &pos, const std::string &name);
		size_t addBoolValue(const size_t &pos, const std::string &name);
		size_t addNullValue(const size_t &pos, const std::string &name);

	private:
		size_t addArrayToArray(const size_t &pos);
		size_t addStringToArray(const size_t &pos);
		size_t addObjectToArray(const size_t &pos);
		size_t addIntegerToArray(const size_t &pos);
		size_t addBoolToArray(const size_t &pos);
};
}  // namespace JsonParser

#endif  // SRC_DESERIALIZATION_H_
