#pragma once
#include <string>
#include <map>
#include <vector>

template<class StringType> class SerializationMapping;

class NotImplemented : public std::logic_error
{
public:
	NotImplemented() : std::logic_error("Function not yet implemented") { };
};

constexpr size_t constLength(const char* str)
{
	return (*str == 0) ? 0 : constLength(str + 1) + 1;
}

template<typename T>
class Serializable
{
	friend class SerializationMapping<T>;
public:
	Serializable(Serializable<T> * parent = nullptr) : m_parent(parent) {}
	/*T getValue(const T &name) const;
	void setValue(const T &name, const T &value);*/

	virtual T toString() const;
	size_t fromString(size_t pos);
	virtual bool fromString();
	bool fromString(const T &str);

private:
	size_t fromStringArray(size_t pos);
	size_t parseStringArray(size_t pos);
	bool checkString();
	bool matchChar(char ch1, char ch2);
	bool checkEscape(size_t pos);
	size_t addKVPair(size_t pos);
	size_t addName(size_t pos, T &name);
	size_t addValue(size_t pos, const T &name);
	bool isNumber(size_t pos);
	bool isBool(size_t pos);
	static size_t toNumber(const T &numberStr);
	size_t strLen() const;
	char getChar(size_t pos) const;

private:
	size_t addStringValue(size_t pos, const T &name);
	size_t addObjectValue(size_t pos, const T &name);
	size_t addArrayValue(size_t pos, const T &name);
	size_t addIntegerValue(size_t pos, const T &name);
	size_t addBoolValue(size_t pos, const T &name);

private:
	size_t addArray(size_t pos);
	size_t addString(size_t pos);
	size_t addObject(size_t pos);
	size_t addInteger(size_t pos);
	size_t addBool(size_t pos);

protected:
	std::map<T, size_t> m_kvPairNumbers;
	std::map<T, bool> m_kvPairBools;
	std::map<T, T> m_kvPairStrings;
	std::map<T, Serializable<T> *> m_kvPairObjects;
	std::map<T, Serializable<T> *> m_kvPairArrays;

	std::vector<Serializable<T> *> m_arrayObjects;
	std::vector<Serializable<T> *> m_arrayArrays;
	std::vector<size_t> m_arrayNumbers;
	std::vector<bool> m_arrayBools;
	std::vector<T> m_arrayStrings;

	Serializable<T> * m_parent { nullptr };
	T *m_fullString{ nullptr };
};