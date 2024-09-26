#pragma once
class SendBuffer sealed : public enable_shared_from_this<SendBuffer>
{
public:
	SendBuffer(uint32 initSize = 512);
	~SendBuffer();

	BYTE* Buffer() { return _buffer.data(); }
	BYTE* WritePos() { return &_buffer[_writePos]; }
	uint32 RemainSize() { return _size - _writePos; }
	uint32 DataSize() { return _writePos; }

	void CopyData(void* src, int32 len);

	void SetWritePos(uint32 pos) { _writePos = pos; }

private:
	vector<BYTE> _buffer;
	uint32 _size = 0;
	uint32 _writePos = 0;
};

