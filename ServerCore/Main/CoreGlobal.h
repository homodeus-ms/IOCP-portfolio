#pragma once


class ThreadManager;
class Memory;
class GlobalQueue;

extern std::unique_ptr<ThreadManager> GThreadManager;
extern std::unique_ptr<Memory> GMemory;
extern std::unique_ptr<GlobalQueue> GGlobalQueue;