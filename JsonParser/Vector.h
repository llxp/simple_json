#pragma once
#ifndef JSONPARSER_VECTOR_H_
#define JSONPARSER_VECTOR_H_

//#include "Serializable.h"
#include "Serializable.h"

namespace JsonParser {

	class VectorBase
	{
		public:
			virtual ~VectorBase() {}
		private:
			friend class SerializationMapping;
			//std::vector<SerializationMapping<StringType> *> m_values;

		protected:
			virtual void clear() {};
			virtual void addNew() {};
			void *lastAddedElement() const
			{
				return this->m_lastAddedElement;
			}
			virtual std::vector<void *>getElements() const = 0;

		protected:
			void *m_lastAddedElement{ nullptr };
	};

	template <typename B, typename D>
	const B* is_unambiguous_public_base_of(const D* obj) {
		try {
			throw obj;
		}
		catch (const B* pb) {
			return pb;
		}
		catch (...) {
		}
		return nullptr;
	}

	template<class T2>
	class Vector : public std::vector<T2 *>, public VectorBase
	{
		public:
			explicit Vector(Vector *parent = nullptr) : m_parent(parent) {}
			~Vector()
			{
				this->clearPointers();
			}
			T2 &operator[](size_t index)
			{
				T2 *ptr = this->at(index);
				return *ptr;
			}
			void assign(const std::vector<T2> &other)
			{
				this->clear();
				for (auto it = other.begin(); it != other.end(); it++) {
					this->push_back(new T2(*it));
				}
				//std::vector<T2 *>::operator=(other);
			}
			void clear() override
			{
				this->clearPointers();
				std::vector<T2 *>::clear();
			}
			void addNew() override
			{
				T2 *newElement = new T2();
				this->push_back(newElement);
				m_lastAddedElement = newElement;
			}
			std::vector<void *> getElements() const
			{
				std::vector<void *> elements;
				for (auto it = this->begin(); it != this->end(); it++) {
					elements.push_back((void *)*it);
				}
				return elements;
			}

		private:
			void clearPointers()
			{
				if (this->size() <= 0) {
					return;
				}
				for (auto it = this->begin(); it != this->end(); it++) {
					T2 *currentElement = *it;
					if (currentElement != nullptr) {
						delete currentElement;
					}
				}
			}

		private:
			Vector * m_parent{ nullptr };
	};
}

#endif  // JSONPARSER_VECTOR_H_