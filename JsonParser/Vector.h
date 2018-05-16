#pragma once
#ifndef JSONPARSER_VECTOR_H_
#define JSONPARSER_VECTOR_H_

//#include "Serializable.h"
#include <memory>

#include "VectorBase.h"

namespace JsonParser {

	template<class T2>
	class Vector : public std::vector<std::shared_ptr<T2>>, public VectorBase
	{
		public:
			explicit Vector(){}
			~Vector()
			{
			}
			T2 &operator[](size_t index)
			{
				std::shared_ptr<T2> ptr = this->at(index);
				return *ptr;
			}
			void assign(const std::vector<T2> &other)
			{
				this->clear();
				for (auto it = other.begin(); it != other.end(); it++) {
					this->push_back(std::make_shared<T2>(*it));
				}
				//std::vector<T2 *>::operator=(other);
			}
			void clear() override
			{
				std::vector<std::shared_ptr<T2>>::clear();
			}
			void addNew() override
			{
				auto newElement = std::make_shared<T2>();
				this->push_back(newElement);
				m_lastAddedElement = std::static_pointer_cast<void>(newElement);
			}
			std::vector<std::shared_ptr<void>> getElements() const override
			{
				std::vector<std::shared_ptr<void>> elements;
				for (auto it = this->begin(); it != this->end(); it++) {
					elements.push_back(std::static_pointer_cast<void>(*it));
				}
				return elements;
			}
	};
}

#endif  // JSONPARSER_VECTOR_H_