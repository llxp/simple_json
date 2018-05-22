#include "String.h"

#include <stdio.h>
#include <mmintrin.h>
#include <emmintrin.h>
//#include <dvec.h>
#include <memory>

JsonParser::String::String()
{
}

JsonParser::String::~String()
{
}

JsonParser::String & JsonParser::String::operator=(const String & other)
{
	if (this != &other) {
		if (other.m_length == 0) {
			tidy();
			return *this;
		} else {
			size_t int128Loops = other.m_length / sizeof(__m128i) + 1;
			if (other.m_length < CHUNK_SIZE) {

				copyToChunk(int128Loops, other);

				this->m_value = this->m_chunk;
				this->m_value[other.m_length] = '\0';
				this->m_length = other.m_length;
			} else {
			this->m_value = (char *)malloc(sizeof(char) *other.m_length + 1);

			size_t loops = int128Loops / CHUNK_SIZE;
			size_t i2 = 0;
			while (i2 < loops) {
				copyToChunk(CHUNK_SIZE, other);
				++i2;
			}
			if (loops * CHUNK_SIZE < other.m_length) {
				copyToChunk(other.m_length - (loops * CHUNK_SIZE), other);
			}

			this->m_value = this->m_chunk;
			this->m_value[other.m_length] = '\0';
			this->m_length = other.m_length;
			}
		}
	}
	this->m_length = other.m_length;
	return *this;
}

bool JsonParser::String::operator&(const String & other) const
{
	return false;
}

bool JsonParser::String::operator==(const String & other) const
{
	return false;
}

void JsonParser::String::tidy()
{
}

void JsonParser::String::copyToChunk(const size_t &int128Loops, const String &other)
{
	size_t i = 0;
	while (i < int128Loops) {
		__m128i otherValue = _mm_loadu_si128((const __m128i*)&other.m_value[i * sizeof(__m128i)]);
		_mm_storeu_si128(
			reinterpret_cast<__m128i*>(&this->m_chunk[i * sizeof(__m128i)]),
			otherValue);
		++i;
	}

	this->m_value = this->m_chunk;
}
