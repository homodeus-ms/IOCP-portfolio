#pragma once

extern thread_local uint32 LThreadId;
extern thread_local class JobQueue* LCurrentJobQueue;
extern thread_local uint64 LWorkEndTickCount;