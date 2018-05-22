#ifndef JSONPARSER_DESERIALIZATION_H_
#define JSONPARSER_DESERIALIZATION_H_

#include <string>
#include <map>
#include <vector>
#include <memory>

#include "JsonTypes.h"
#include "Number.h"
#include "SerializationData.h"


namespace JsonParser {

	class NotImplemented : public std::logic_error
	{
		public:
			NotImplemented() : std::logic_error("Function not yet implemented") { }
	};

	constexpr size_t constLength(const char* str)
	{
		return (*str == 0) ? 0 : constLength(str + 1) + 1;
	}

	class DeSerialization : public SerializationData
	{
		public:
			explicit DeSerialization();
			virtual ~DeSerialization();
			virtual std::string toString() const override;
			virtual std::string toStringArray() const;
			virtual bool fromString(std::shared_ptr<std::string> str);
			virtual bool fromString() override;

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
			size_t addArray(const size_t &pos);
			size_t addString(const size_t &pos);
			size_t addObject(const size_t &pos);
			size_t addInteger(const size_t &pos);
			size_t addBool(const size_t &pos);
	};

}

#endif  // JSONPARSER_DESERIALIZATION_H_
