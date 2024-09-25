#pragma once

#define OUT

// LOCK 관련
#define USE_MANY_LOCKS(count) Lock _locks[count];
#define USE_LOCK              USE_MANY_LOCKS(1)
#define WRITE_LOCK_IDX(idx)   WriteLockGuard writeLockGuard_##idx(_locks[idx]);
#define WRITE_LOCK            WRITE_LOCK_IDX(0)
#define READ_LOCK_IDX(idx)    ReadLockGuard readLockGuard_##idx(_locks[idx]);
#define READ_LOCK             READ_LOCK_IDX(0)


// Memory 관련

#ifdef _DEBUG
#define Xalloc(size)		PoolAllocator::Alloc(size)
#define Xrelease(ptr)		PoolAllocator::Release(ptr)
#else
#define Xalloc(size)		PoolAllocator::Alloc(size)
#define Xrelease(ptr)		PoolAllocator::Release(ptr)
#endif


#define CRASH(cause)                    \
{                                       \
    int* p = nullptr;                   \
    __analysis_assume(p != nullptr);    \
    *p = 0xDEADBEEF;                    \
}

#define ASSERT_CRASH(expr)          \
{                                   \
    if (!(expr))                    \
    {                               \
        CRASH("ASSERT_CRASH");      \
    }                               \
}

