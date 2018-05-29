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

#include "VectorBase.h"

namespace JsonParser {

	template<class T2>
	class Vector : public std::vector<std::unique_ptr<T2>>, public VectorBase
	{
		public:
			Vector(){}
			~Vector()
			{
			}
			T2 &operator[](size_t index)
			{
				return *this->at(index);
			}
			void assign(const std::vector<T2> *other)
			{
				this->clear();
				if (other == nullptr) {
					return;
				}
				for (auto it = other->begin(); it != other->end(); it++) {
					this->push_back(std::make_unique<T2>(*it));
				}
			}
			void clear() override
			{
				std::vector<std::unique_ptr<T2>>::clear();
			}
			void *addNew() override
			{
				auto newElement = std::make_unique<T2>();
				if (newElement != nullptr) {
					void *rawPtr = newElement.get();
					this->push_back(std::move(newElement));
					return rawPtr;
				}
				return nullptr;
			}
			std::vector<void *> getElements() const override
			{
				std::vector<void *> elements;
				for (auto it = this->begin(); it != this->end(); it++) {
					elements.push_back(static_cast<void *>((*it).get()));
				}
				return elements;
			}
	};
}  // namespace JsonParser

#endif  // SRC_VECTOR_H_