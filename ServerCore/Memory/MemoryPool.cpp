#include "pch.h"
#include "MemoryPool.h"

MemoryPool::MemoryPool(int32 size)
    : _allocSize(size)
{
    ::InitializeSListHead(&_slistHeader);
}

MemoryPool::~MemoryPool()
{
    while (MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_slistHeader)))
    {
        ::_aligned_free(memory);
    }
}

MemoryHeader* MemoryPool::Pop()
{
    MemoryHeader* header = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_slistHeader));
    if (header == nullptr)
    {
        header = reinterpret_cast<MemoryHeader*>(::_aligned_malloc(_allocSize, SLIST_ALIGN));
    }
    else
    {
        _reserveCount.fetch_sub(1);
    }

    _useCount.fetch_add(1);

    return header;
}

void MemoryPool::Push(MemoryHeader* header)
{
    header->allocSize = 0;
    ::InterlockedPushEntrySList(&_slistHeader, static_cast<PSLIST_ENTRY>(header));
    _useCount.fetch_sub(1);
    _reserveCount.fetch_add(1);
}
