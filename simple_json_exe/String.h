#ifndef JSONPARSER_STRING_H_
#define JSONPARSER_STRING_H_

#ifdef __linux__ 
#include <stddef.h>
#endif

namespace JsonParser {
	class String
	{
	public:
		String();
		~String();
		String& operator=(const String& other);
		bool operator&(const String& other) const;
		bool operator==(const String& other) const;

	private:
		void tidy();
		void copyToChunk(const size_t &int128Loops, const String &other);
	private:
		enum { CHUNK_SIZE = 128 };
		char m_chunk[CHUNK_SIZE]; // used for strings which are short than 128 chars
		int m_length;             // contains the length of the string
		char *m_value;            // points to either the chunk for small strings or to a heap buffer for longer strings
		int m_bufferSize;         // contains the size of a heap allocated buffer

	};

}

#endif // JSONPARSER_STRING_H_