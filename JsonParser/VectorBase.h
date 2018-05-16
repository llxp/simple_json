#pragma once

#include <memory>
#include <vector>

namespace JsonParser {
	class VectorBase
	{
		public:
			virtual ~VectorBase() {}
		private:
			friend class SerializationMapping;
			//std::vector<SerializationMapping<StringType> *> m_values;

		public:
			virtual void clear() {};
			virtual void addNew() {};
			std::shared_ptr<void> lastAddedElement() const
			{
				return this->m_lastAddedElement;
			}
			virtual std::vector<std::shared_ptr<void>> getElements() const = 0;

		protected:
			std::shared_ptr<void> m_lastAddedElement{ nullptr };
	};
}