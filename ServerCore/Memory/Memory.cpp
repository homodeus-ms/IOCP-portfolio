#include "pch.h"
#include "Memory.h"
#include "MemoryPool.h"

Memory::Memory()
{
    int32 tableIdx = 0;
    int32 size;

    // 32의 배수로 블록이 커짐, 1024바이트까지
    for (size = S_BLOCK_SIZE; size <= S_BLOCK_LIMIT; size += S_BLOCK_SIZE)
    {
        MemoryPool* pool = new MemoryPool(size);
        _pools.push_back(pool);
        while (tableIdx <= size)
        {
            _poolTable[tableIdx++] = pool;
        }
    }

    // 128의 배수로 블록이 커짐, 2048바이트까지
    for (size = S_BLOCK_LIMIT; size <= M_BLOCK_LIMIT; size += M_BLOCK_SIZE)
    {
        MemoryPool* pool = new MemoryPool(size);
        _pools.push_back(pool);
        while (tableIdx <= size)
        {
            _poolTable[tableIdx++] = pool;
        }
    }

    // 256의 배수로 블록이 커짐, 4096바이트까지
    for (size = M_BLOCK_LIMIT; size <= L_BLOCK_LIMIT; size += L_BLOCK_SIZE)
    {
        MemoryPool* pool = new MemoryPool(size);
        _pools.push_back(pool);
        while (tableIdx <= size)
        {
            _poolTable[tableIdx++] = pool;
        }
    }
}

Memory::~Memory()
{
    for (MemoryPool* p : _pools)
        delete p;
    _pools.clear();
}

void* Memory::Allocate(int32 size)
{
    MemoryHeader* header = nullptr;
    const int allocSize = size + sizeof(MemoryHeader);

    if (allocSize > MAX_ALLOC_SIZE)
    {
        header = reinterpret_cast<MemoryHeader*>(::_aligned_malloc(allocSize, SLIST_ALIGN));
    }
    else
    {
        header = _poolTable[allocSize]->Pop();
    }

    return MemoryHeader::DetachHeader(header, allocSize);
}

void Memory::Release(void* ptr)
{
    MemoryHeader* header = MemoryHeader::AttachHeader(ptr);
    const int allocSize = header->allocSize;

    if (allocSize > MAX_ALLOC_SIZE)
    {
        ::_aligned_free(reinterpret_cast<void*>(header));
    }
    else
    {
        _poolTable[allocSize]->Push(header);
    }
}
