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

#ifndef SRC_VECTOR_H_
#define SRC_VECTOR_H_

#include <memory>
#include <vector>
#include <iostream>
#include <string>
#include <utility>

#include <simple_json/VectorBase.h>
#include <simple_json/SerializationUtils.h>
#include <simple_json/SerializableBase.h>
#include <simple_json/SerializationData.h>

extern inline void printDebugMessage(const std::string &function, int line);
extern size_t lineCounter;

namespace JsonParser {

	template<class T2>
	struct Node
	{
	public:
		Node<T2> * addNode()
		{
			Node<T2> *node = new Node<T2>();
			this->m_next = node;
			return node;
		}
		Node<T2> * addValue(T2 &&value)
		{
			Node<T2> *node = new Node<T2>();
			this->m_next = node;
			node->m_value = std::move(value);
			return node;
		}
		T2 *value()
		{
			return &m_value;
		}
		void setValue(T2 &&value)
		{
			m_value = std::move(value);
		}
		Node<T2> *next() const
		{
			return m_next;
		}
	private:
		T2 m_value;
		Node<T2> *m_next{ nullptr };
	};

	namespace internal
	{
		static const unsigned int FRONT_SIZE = sizeof("internal::GetTypeNameHelper<") - 1u;
		static const unsigned int BACK_SIZE = sizeof(">::GetTypeName") - 1u;

		template <typename T>
		struct GetTypeNameHelper
		{
			static const char* GetTypeName(void)
			{
				static const size_t size = sizeof(__FUNCTION__) - FRONT_SIZE - BACK_SIZE;
				static char typeName[size] = {};
				memcpy(typeName, __FUNCTION__ + FRONT_SIZE, size - 1u);

				return typeName;
			}
		};
	}


	template <typename T>
	const char* GetTypeName(void)
	{
		return internal::GetTypeNameHelper<T>::GetTypeName();
	}

	template<class T2>
	class Vector : public VectorBase
	{
	private:
		Node<T2> *rootNode{ nullptr };
		Node<T2> *currentNode{ nullptr };
		std::vector<T2 *> pointers;

	private:
		template <class T, typename std::enable_if<std::is_base_of<T, T2>::value, T>::type* = nullptr>
		T * addNode(T *value)
		{
			T2 *casted = static_cast<T2 *>(value);
			if (rootNode == nullptr) {
				rootNode = new Node<T2>();
				rootNode->setValue(std::move(*casted));
				currentNode = rootNode;
			} else {
				currentNode = currentNode->addValue(std::move(*casted));
			}
			pointers.push_back(currentNode->value());
			return currentNode->value();
		}

		template <class T, typename std::enable_if<!std::is_base_of<T, T2>::value, T>::type* = nullptr>
		T * addNode(T *value)
		{
			value;
			return nullptr;
		}

		template <class T, typename std::enable_if<std::is_base_of<T, T2>::value, T>::type* = nullptr>
		T * addNewNode()
		{
			if (rootNode == nullptr) {
				rootNode = new Node<T2>();
				currentNode = rootNode;
			}
			else {
				currentNode = currentNode->addNode();
			}
			pointers.push_back(currentNode->value());
			return currentNode->value();
		}

		template <class T, typename std::enable_if<!std::is_base_of<T, T2>::value, T>::type* = nullptr>
		T * addNewNode()
		{
			std::cout << __FUNCTION__ << GetTypeName<T>() << "...wrong function called..." << std::endl;
			return nullptr;
		}

		public:
			Vector() { }
			~Vector()
			{
				clear();
			}
			T2 &operator[](size_t index)
			{
				return *this->operator[](index);
			}

			size_t size() const
			{
				return pointers.size();
			}

			void push_back(const T2 &element)
			{
				T2 *newElement = new T2(element);
				addNode<T>(newElement);
			}

			template <class T, typename std::enable_if<std::is_base_of<T, T2>::value, T>::type* = nullptr>
			void push_back(T *element)
			{
				addNode<T>(element);
			}

			template <class T, typename std::enable_if<!std::is_base_of<T, T2>::value, T>::type* = nullptr>
			void push_back(T *element)
			{
				std::cout << __FUNCTION__ << GetTypeName<T>() << "...wrong function called..." << std::endl;
				element;
			}

			// Inherited via VectorBase
			void clear() override
			{
				Node<T2> *node = rootNode;
				while (node != nullptr) {
					Node<T2> *nextNode = node->next();
					delete node;
					node = nextNode;
				}
			}

			// Inherited via VectorBase
			virtual bool isEmpty() override
			{
				if (currentNode == nullptr) {
					return false;
				}
				return true;
			}

			// Inherited via VectorBase
			virtual JsonString toString() const override
			{
				JsonString outputStr(JsonArrayOpenStr);
				for (Node<T2> *node = rootNode; node != nullptr; node = node->next()) {
					if (node != nullptr) {
						T2 *value(node->value());
						JsonString &&tempStr = toString(value);
						if (tempStr.length() <= 0) {
							continue;
						}
						outputStr += tempStr;
					}
					if (node->next() != nullptr && node->next()->next() != nullptr) {
						outputStr += JsonEntrySeparator;
					}
				}
				outputStr += JsonArrayClose;
				return outputStr;
			}
			virtual bool fromString(JsonString * string) override
			{
				this->setString(string->data());
				this->setStrLen(string->length());
				size_t pos = 0;
				return this->parseString(pos);
			}
			virtual bool parseString(size_t &currentPos) override
			{
				size_t openingCount(0);
				const size_t tempLen(strLen());
				while (currentPos < tempLen) {
					switch (getChar(currentPos)) {
					case '\n':
						++lineCounter;
						break;
					case JsonArrayOpen:
						++openingCount;
						// Expecting position of JsonArrayClose as result
						if (!addArrayValues(currentPos)) {
							printDebugMessage(__FUNCTION__, __LINE__);
							return false;
						}
						break;
					}
					switch (getChar(currentPos)) {
					case JsonArrayClose:
						--openingCount;
						if (openingCount == 0) {
							return true;
						}
					}
					++currentPos;
				}
				printDebugMessage(__FUNCTION__, __LINE__);
				return false;
			}

		private:
			/*################################################################
			Begin Array parsing related functions
			################################################################*/
			bool addArrayValues(size_t &currentPos)
			{
				bool commaFound = true;
				const size_t tempLen(strLen());
				++currentPos;
				while (currentPos < tempLen) {
					char &&c = getChar(currentPos);
					switch (c) {
					case '\n':
						++lineCounter;
						break;
					case JsonObjectOpen: {
						// Expecting currentPos to be the position of
						// JsonObjectClose after return
						if (!addObjectToArray(currentPos) || !commaFound) {
							printDebugMessage(__FUNCTION__, __LINE__);
							return false;
						}
						commaFound = false;
					}
					break;
					case JsonArrayOpen: {
						// Expecting currentPos to be the position of
						// JsonArrayClose after return
						if (!addArrayToArray(currentPos) || !commaFound) {
							printDebugMessage(__FUNCTION__, __LINE__);
							return false;
						}
						commaFound = false;
					}
					break;
					case JsonStringSeparator: {
						if (!addStringToArray(currentPos) || !commaFound) {
							printDebugMessage(__FUNCTION__, __LINE__);
							return false;
						}
						commaFound = false;
					}
					break;
					case JsonEntrySeparator:
						commaFound = true;
						break;
					case JsonArrayClose:
						if (!commaFound) {
							return true;
						}
						break;
					case 't':
					case 'T':
					case 'f':
					case 'F': {
						if (!addBoolToArray(currentPos) || !commaFound) {
							printDebugMessage(__FUNCTION__, __LINE__);
							return false;
						}
						commaFound = false;
					}
					break;
					default:
						if (isNumber(c)) {
							if (!addNumberToArray(currentPos) || !commaFound) {
								printDebugMessage(__FUNCTION__, __LINE__);
								return false;
							}
							commaFound = false;
						}
					}
					++currentPos;
				}
				printDebugMessage(__FUNCTION__, __LINE__);
				return false;
			}

			bool addObjectToArray(size_t &currentPos)
			{
				auto newElement = this->addNewNode<SerializationData>();
				if (newElement != nullptr) {
					newElement->assign(this);
					return newElement->parseString(currentPos);
				}
				printDebugMessage(__FUNCTION__, __LINE__);
				return false;
			}

			bool addNumberToArray(size_t &currentPos)
			{
				const size_t tempLen(strLen());
				// iterate as long there is a number, otherwise,
				// copy the substring and create a number
				for (size_t i = currentPos; i < tempLen; ++i) {
					if (!isNumber(getChar(i))) {
						this->push_back<JsonParser::Number>(
							new JsonParser::Number(std::move(
								substr(currentPos, i - currentPos))));
						currentPos = i - 1;
						return true;
					}
				}
				printDebugMessage(__FUNCTION__, __LINE__);
				return false;
			}

			bool addStringToArray(size_t &currentPos)
			{
				auto newElement = this->addNewNode<JsonString>();
				if (newElement != nullptr) {
					return this->getString(currentPos, *newElement);
				}
				return true;
			}

			bool addArrayToArray(size_t &currentPos)
			{
				auto newElement = this->addNewNode<JsonParser::VectorBase>();
				if (newElement != nullptr) {
					newElement->assign(this);
					return newElement->parseString(currentPos);
				}
				printDebugMessage(__FUNCTION__, __LINE__);
				return false;
			}

			bool addBoolToArray(size_t &currentPos)
			{
				switch (getChar(currentPos)) {
				case 't':  // adding true to the array
					this->addNode<bool>(new bool(true));
					currentPos += constLength("True") - 1;
					return true;
				case 'f':  // adding false to the array
					this->addNode<bool>(new bool(false));
					currentPos += constLength("False") - 1;
					return true;
				}
				printDebugMessage(__FUNCTION__, __LINE__);
				return false;
			}
			/*################################################################
			End Array parsing related functions
			################################################################*/

			private:
				JsonString toString(VectorBase *array) const
				{
					return array->toString();
				}

				JsonString toString(JsonParser::SerializationData *object) const
				{
					return object->toString();
				}

				JsonString toString(bool *boolean) const
				{
					switch (*boolean) {
					case true:
						return "true";
					case false:
						return "false";
					}
					return "";
				}

				JsonString toString(JsonParser::Number *number) const
				{
					return number->toString();
				}

				JsonString toString(JsonString *string) const
				{
					return JsonParser::SerializationUtils::makeString(*string);
				}
	};
}  // namespace JsonParser

#endif  // SRC_VECTOR_H_