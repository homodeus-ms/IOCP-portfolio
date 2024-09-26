#include "pch.h"
#include "BufferReader.h"

BufferReader::~BufferReader()
{
}

bool BufferReader::Peek(void* dest, uint32 len)
{
	if (RemainSize() < len)
		return false;

	::memcpy(dest, &_buffer[_pos], len);
	
	return true;
}

bool BufferReader::Read(void* dest, uint32 len)
{
	if (Peek(dest, len) == false)
		return false;
	_pos += len;
	return true;
}
