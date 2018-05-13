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
			Vector(Vector *parent = nullptr) : m_parent(parent) {}
			~Vector()
			{
				if (this->size() <= 0) {
					return;
				}
				for (auto it = this->begin(); it != this->end(); it++) {
					T2 *currentElement = *it;
					if (currentElement != nullptr) {
						delete currentElement;
						//currentElement = nullptr;
					}
				}
				//this->clear();
			}
			T2 &operator[](size_t index)
			{
				T2 *ptr = this->at(index);
				return *ptr;
			}
			Vector operator=(const std::vector<T2 *> &other)
			{
				std::vector<T2 *>::operator=(other);
				return *this;
			}
			void clear() override
			{
				std::vector<T2 *>::clear();
			}
			void addNew() override
			{
				//T2 newElement;
				T2 *newElement = new T2();
				this->push_back(newElement);
				m_lastAddedElement = newElement;
				//return newElement;
			}
			std::vector<void *> getElements() const
			{
				std::vector<void *> elements;
				for (auto it = this->begin(); it != this->end(); it++) {
					elements.push_back((void *)*it);
				}
				return elements;
			}
			//T2 *addNew();

		private:
			Vector * m_parent{ nullptr };
	};
}
/*
template<class StringType, class T>
class Vector :
	private Serializable<StringType>
{
public:
	Vector() {}
	~Vector() {}
	size_t size() const { return 0; }

protected:
	void addValue(Serializable<StringType> *value) {}
};*/

//#include "Vector.cpp"

#endif  // JSONPARSER_VECTOR_H_