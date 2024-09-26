#include "pch.h"
#include "RecvBuffer.h"


RecvBuffer::RecvBuffer(uint32 initSize)
	: _bufferSize(initSize)
{
	_capacity = _bufferSize * DEFAULT_BUFFER_COUNT;
	_buffer.resize(_capacity);
}

RecvBuffer::~RecvBuffer()
{
}

bool RecvBuffer::UpdateReadPos(uint32 pos)
{
	uint32 remainSize = RemainSize();
	if (remainSize < pos)
		return false;
	_readPos += pos;
	return true;
}

bool RecvBuffer::UpdateWritePos(uint32 pos)
{
	if (RemainSize() < pos)
		return false;
	_writePos += pos;
	return true;
}

void RecvBuffer::CleanUpPos()
{
	if (_readPos == _writePos)
	{
		_readPos = 0;
		_writePos = 0;
	}
	else if (RemainSize() < _bufferSize)
	{
		const int dataSize = DataSize();
		::memcpy(_buffer.data(), ReadPos(), dataSize);
		_readPos = 0;
		_writePos = dataSize;
	}
}
