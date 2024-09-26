#pragma once
class RecvBuffer sealed
{
	enum
	{
		DEFAULT_BUFFER_COUNT = 10
	};

public:
	RecvBuffer(uint32 initSize = 512);
	~RecvBuffer();

	BYTE* WritePos() { return &_buffer[_writePos]; }
	BYTE* ReadPos()  { return &_buffer[_readPos]; }
	uint32 Capacity() const { return _capacity; }
	uint32 RemainSize() const { return _capacity - _writePos; }
	uint32 DataSize() const { return _writePos - _readPos; }

	bool UpdateReadPos(uint32 pos);
	bool UpdateWritePos(uint32 pos);
	void CleanUpPos();


private:
	vector<BYTE> _buffer;
	uint32 _readPos = 0;
	uint32 _writePos = 0;
	uint32 _capacity;
	uint32 _bufferSize;
};

