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

#define MEMBER(TYPE, NAME)\
    private:\
        TYPE m_##NAME;\
    public:\
        TYPE NAME(void) const\
        {\
            return m_##NAME;\
        }\
        void set##NAME(const TYPE &value)\
        {\
        m_##NAME = value;\
        }

#define MEMBERDEFAULT(TYPE, NAME, DEFAULT)\
    private:\
        TYPE m_##NAME = { DEFAULT };\
    public:\
        TYPE NAME(void) const\
        {\
            return m_##NAME;\
        }\
        void set##NAME(const TYPE &value)\
        {\
            m_##NAME = value;\
        }
#define PROTMEMBERDEFAULT(TYPE, NAME, DEFAULT)\
    protected:\
        TYPE m_##NAME { DEFAULT };\
    public:\
        TYPE NAME(void) const\
        {\
            return m_##NAME;\
        }\
        void set##NAME(const TYPE &value)\
        {\
            m_##NAME = value;\
        }

#define PUBMEMBER(TYPE, NAME) public: TYPE m_##NAME; TYPE NAME(void) const { return m_##NAME; } void set##NAME(const TYPE &value) { m_##NAME = value; }
#define PMEMBER(TYPE, NAME) private: TYPE m_##NAME; TYPE NAME(void) const { return m_##NAME; } void set##NAME(const TYPE &value) { m_##NAME = value; }
#define PSMEMBER(TYPE, NAME) private: TYPE m_##NAME; TYPE NAME(void) const { return m_##NAME; } private: void set##NAME(const TYPE &value) { m_##NAME = value; }
#define PGMEMBER(TYPE, NAME) private: TYPE m_##NAME; private: TYPE NAME(void) const { return m_##NAME; } public: void set##NAME(const TYPE &value) { m_##NAME = value; }
#define PROTMEMBER(TYPE, NAME) protected: TYPE m_##NAME; public: TYPE NAME(void) const { return m_##NAME; } void set##NAME(const TYPE &value) { m_##NAME = value; }
#define NVP(NAME) ar( cereal::make_nvp(#NAME, m_##NAME) );
#define RESTCLASS(NAME, ...) class NAME : public simple_json::Serializable { __VA_ARGS__ };

#include "SerializationMapping.h"

namespace simple_json {
class Serializable : public JsonParser::SerializationMapping
{
	public:
		Serializable() {}
		~Serializable() {}

	public:
		DLLEXPORT bool fromString();
		DLLEXPORT bool fromString(const std::shared_ptr<JsonString> &str);

		DLLEXPORT virtual JsonString toString() const;
		DLLEXPORT virtual JsonString toStringArray() const;

	protected:
		DLLEXPORT void addMember(const JsonString &name,
			__int64 &memberVariable,
			bool optional = false);
		DLLEXPORT void addMember(const JsonString &name,
			JsonString &memberVariable,
			bool optional = false);
		DLLEXPORT void addMember(const JsonString &name,
			SerializationMapping &memberVariable,
			bool optional = false);
		DLLEXPORT void addMember(const JsonString &name,
			bool &memberVariable,
			bool optional = false);

		DLLEXPORT void addMember(const JsonString &name,
			JsonParser::Vector<JsonString> &memberVariable,
			bool optional = false);
		DLLEXPORT void addMember(const JsonString &name,
			JsonParser::Vector<JsonParser::Number> &memberVariable,
			bool optional = false);
		DLLEXPORT void addMember(const JsonString &name,
			JsonParser::Vector<bool> &memberVariable, bool optional = false);
		DLLEXPORT void addMember(const JsonString &name,
			JsonParser::VectorBase &memberVariable, bool optional = false);

		template<typename T>
		DLLEXPORT T DataContract(JsonString name, T &value, const T &defaultValue)
		{
			addMember(name, value);
			return T(defaultValue);
		}
};
}  // namespace simple_json

#endif  // SRC_SERIALIZABLE_H_