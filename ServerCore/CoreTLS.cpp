#include "pch.h"
#include "CoreTLS.h"
#include "JobQueue.h"

thread_local uint32 LThreadId = 0;
thread_local JobQueue* LCurrentJobQueue = nullptr;
thread_local uint64 LWorkEndTickCount = 0;