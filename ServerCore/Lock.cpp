#include "pch.h"
#include "Lock.h"
#include "CoreTLS.h"

// [WWWW'WWWW][WWWW'WWWW][RRRR'RRRR][RRRR'RRRR]

void Lock::WriteLock()
{
	const uint32 lockedId = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;
	if (lockedId == LThreadId)
	{
		++_writeCount;
		return;
	}

	const int64 startTick = ::GetTickCount64();
	const uint32 desired = (LThreadId << 16) & WRITE_THREAD_MASK;

	while (true)
	{
		for (uint32 i = 0; i < MAX_SPIN_COUNT; ++i)
		{
			uint32 expected = EMPTY_FLAG;
			if (_lockFlag.compare_exchange_strong(expected, desired))
			{
				++_writeCount;
				return;
			}
		}

		if (::GetTickCount64() - startTick > LIMIT_ACQUIRE_TICK)
			CRASH("LOCK_TIMEOUT");

		this_thread::yield();
	}
}

void Lock::WriteUnlock()
{
	// Read락을 함께 잡고 있으면 Read락부터 풀어야함
	if ((_lockFlag.load() & READ_COUNT_MASK) != 0)
		CRASH("SHOULD READ UNLOCK FIRST");

	if (--_writeCount == 0)
		_lockFlag.store(EMPTY_FLAG);
}

void Lock::ReadLock()
{
	const uint32 lockedId = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;
	if (lockedId == LThreadId)
	{
		_lockFlag.fetch_add(1);
		return;
	}

	const int64 startTick = ::GetTickCount64();

	while (true)
	{
		for (uint32 i = 0; i < MAX_SPIN_COUNT; ++i)
		{
			uint32 expected = (_lockFlag.load() & READ_COUNT_MASK);
			if (_lockFlag.compare_exchange_strong(expected, expected + 1))
			{
				return;
			}
		}

		if (::GetTickCount64() - startTick > LIMIT_ACQUIRE_TICK)
			CRASH("LOCK TIME_OUT");

		this_thread::yield();
	}
}

void Lock::ReadUnlock()
{
	if ((_lockFlag.fetch_sub(1) & READ_COUNT_MASK) == 0)
		CRASH("UNLOCK DUPLICATED")
}
