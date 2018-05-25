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
			virtual void clear() = 0;
			virtual void *addNew() = 0;
			virtual std::vector<void *> getElements() const = 0;
	};
}