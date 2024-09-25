#include "pch.h"
#include "BufferWriter.h"

BufferWriter::~BufferWriter()
{
}



bool BufferWriter::Write(void* src, uint32 len)
{
    if (RemainSize() < len)
        return false;

    ::memcpy(&_buffer[_pos], src, len);
    _pos += len;

    return true;;
}
