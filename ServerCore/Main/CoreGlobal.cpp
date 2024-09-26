#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "Memory.h"
#include "GlobalQueue.h"

std::unique_ptr<ThreadManager> GThreadManager = make_unique<ThreadManager>();
std::unique_ptr<Memory> GMemory = make_unique<Memory>();
std::unique_ptr<GlobalQueue> GGlobalQueue = make_unique<GlobalQueue>();