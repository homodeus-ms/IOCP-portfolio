#pragma once
class BufferWriter sealed
{
public:
	BufferWriter(BYTE* buffer, uint32 size, uint32 pos = 0) : _buffer(buffer), _size(size), _pos(pos) {}
	~BufferWriter();

	BYTE* WritePos() { return &_buffer[_pos]; }
	uint32 Size() const { return _size; }
	uint32 WriteSize() const { return _pos; }
	uint32 RemainSize() const { return _size - _pos; }

	template<typename T>
	BufferWriter& operator<<(T&& src);

	BufferWriter& operator<<(const char* str)
	{
		const int32 size = strlen(str);
		ASSERT_CRASH(RemainSize() > size);

		::memcpy(&_buffer[_pos], str, size);
		_pos += size;
		return *this;
	}

	template<typename T>
	T* Reserve();

	template<typename T>
	bool Write(T* src) { return Write(src, sizeof(T)); }
	bool Write(void* src, uint32 len);


private:
	BYTE* _buffer;
	uint32 _size = 0;
	uint32 _pos = 0;
};

template<typename T>
inline BufferWriter& BufferWriter::operator<<(T&& src)
{
	using DataType = std::remove_reference_t<T>;
	*reinterpret_cast<DataType*>(&_buffer[_pos]) = std::forward<DataType>(src);
	_pos += sizeof(T);
	return *this;
}

template<typename T>
inline T* BufferWriter::Reserve()
{
	if (RemainSize() < sizeof(T))
		return nullptr;
	T* ret = reinterpret_cast<T*>(&_buffer[_pos]);
	_pos += sizeof(T);

	return ret;
}
