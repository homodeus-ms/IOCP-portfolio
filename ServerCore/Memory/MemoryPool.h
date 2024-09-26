#pragma once



enum
{
	SLIST_ALIGN = 16,
};

DECLSPEC_ALIGN(SLIST_ALIGN)
struct MemoryHeader : public SLIST_ENTRY
{
	MemoryHeader(int32 size) : allocSize(size) {}

	static MemoryHeader* AttachHeader(void* ptr)
	{
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr);
		return --header;
	}
	static void* DetachHeader(MemoryHeader* header, int32 allocSize)
	{
		new(header)MemoryHeader(allocSize);
		return reinterpret_cast<void*>(++header);
	}

	int32 allocSize = 0;
};

DECLSPEC_ALIGN(SLIST_ALIGN)
class MemoryPool
{
public:
	MemoryPool(int32 size);
	~MemoryPool();

	friend class Memory;

	template<typename T>
	friend class ObjectPool;

private:
	MemoryHeader* Pop();
	void Push(MemoryHeader* header);

private:
	SLIST_HEADER _slistHeader;
	int32 _allocSize = 0;
	atomic<uint32> _useCount = 0;
	atomic<uint32> _reserveCount = 0;
};

