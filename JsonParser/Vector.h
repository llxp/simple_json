#pragma once
#ifndef JSONPARSER_VECTOR_H_
#define JSONPARSER_VECTOR_H_

//#include "Serializable.h"
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
			void assign(const std::vector<T2> &other)
			{
				this->clear();
				for (auto it = other.begin(); it != other.end(); it++) {
					this->push_back(std::make_unique<T2>(*it));
				}
				//std::vector<T2 *>::operator=(other);
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
}

#endif  // JSONPARSER_VECTOR_H_