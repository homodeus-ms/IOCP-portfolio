#pragma once
class BufferReader sealed
{
public:
	BufferReader(BYTE* buffer, uint32 size, uint32 pos = 0) : _buffer(buffer), _size(size), _pos(pos) {}
	~BufferReader();

	uint32 RemainSize() const { return _size - _pos; }
	uint32 Size() const { return _size; }
	BYTE* ReadPos() { return &_buffer[_pos]; }
	uint32 ReadSize() const { return _pos; }

	template<typename T>
	BufferReader& operator>>(T& dest);

	template<typename T>
	bool Peek(T* dest) { return Peek(dest, sizeof(T)); }
	bool Peek(void* dest, uint32 len);

	template<typename T>
	bool Read(T* dest) { return Read(dest, sizeof(T)); }
	bool Read(void* dest, uint32 len);

private:
	BYTE* _buffer;
	uint32 _size;
	uint32 _pos;
};

template<typename T>
inline BufferReader& BufferReader::operator>>(T& dest)
{
	dest = *(reinterpret_cast<T*>(&_buffer[_pos]));
	_pos += sizeof(T);
	return *this;
}
