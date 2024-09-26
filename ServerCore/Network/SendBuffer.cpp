#include "pch.h"
#include "SendBuffer.h"

SendBuffer::SendBuffer(uint32 initSize)
	: _size(initSize)
{
	_buffer.resize(_size);
}

SendBuffer::~SendBuffer()
{
}

void SendBuffer::CopyData(void* src, int32 len)
{
	if (RemainSize() < len)
		return;
	::memcpy(&_buffer[_writePos], src, len);
	_writePos += len;
}
