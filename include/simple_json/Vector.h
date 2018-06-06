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

#include "VectorBase.h"

namespace JsonParser {

	template<class T2>
	class Vector : public std::vector<T2>, public VectorBase
	{
	public:
		Vector() { std::vector<T2>::reserve(30); }
		~Vector()
		{
		}

		/*Vector<T2>&operator=(Vector<T2>&& other)
		{
			other.swap(*this);
			return *this;
		}*/

		/*T2 &operator[](size_t index)
		{
			return *std::vector<std::unique_ptr<T2>>::operator[](index);
		}

		T2 &at(size_t index) const
		{
			return *std::vector<std::unique_ptr<T2>>::at(index);
		}

		T2 &at(size_t index)
		{
			return *std::vector<std::unique_ptr<T2>>::at(index);
		}*/

		/*void push_back(const T2 &element)
		{
			auto newElement = std::make_unique<T2>(element);
			if (newElement != nullptr) {
				size_t size = std::vector<std::unique_ptr<T2>>::size();
				std::vector<std::unique_ptr<T2>>::resize(size + 1);
				//std::vector<std::shared_ptr<T2>>::push_back(std::move(newElement));
				std::vector<std::unique_ptr<T2>>::operator[](size) = std::move(newElement);
			}
		}*/

		/*void assign(const std::vector<T2> *other)
		{
			this->clear();
			if (other == nullptr) {
				return;
			}
			auto endPos = other->end();
			std::vector<std::unique_ptr<T2>>::resize(other->size());
			for (auto it = other->begin(); it != endPos; it++) {
				std::vector<std::unique_ptr<T2>>::push_back(std::move(std::make_unique<T2>(*it)));
			}
		}*/

		// Inherited via VectorBase
		void clear() override
		{
			std::vector<T2>::clear();
		}

		// Inherited via VectorBase
		void *addNew() override
		{
			//auto newElement = std::make_unique<T2>();
			//if (newElement != nullptr) {
				//void *rawPtr = newElement.get();
				std::vector<T2>::push_back(std::move(T2()));
				return &this->back();
			//}
			//return nullptr;  // addNew() failed.
		}

		// Inherited via VectorBase
		std::vector<const void *> getElements() const override
		{
			std::vector<const void *> elements;
			auto endPos = this->end();
			for (auto it = this->begin(); it != endPos; it++) {
				const T2 element = *it;
				elements.push_back(static_cast<const void *>(&element));
			}
			return elements;
		}

		// Inherited via VectorBase
		virtual bool isEmpty() override
		{
			if (std::vector<T2>::size() == 0) {
				return true;
			}
			return false;
		}
	};
}  // namespace JsonParser

#endif  // SRC_VECTOR_H_