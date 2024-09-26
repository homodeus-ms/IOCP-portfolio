#pragma once

class MemoryPool;

enum
{
	S_BLOCK_SIZE = 32,
	S_BLOCK_LIMIT = 1024,
	M_BLOCK_SIZE = 128,
	M_BLOCK_LIMIT = 2048,
	L_BLOCK_SIZE = 256,
	L_BLOCK_LIMIT = 4096,
};

enum
{
	POOL_COUNT = S_BLOCK_LIMIT / S_BLOCK_SIZE + M_BLOCK_LIMIT / M_BLOCK_SIZE + L_BLOCK_LIMIT / L_BLOCK_SIZE,
	MAX_ALLOC_SIZE = L_BLOCK_LIMIT,
};

class Memory
{
public:
	Memory();
	~Memory();

	void* Allocate(int32 size);
	void Release(void* ptr);

private:
	std::vector<MemoryPool*> _pools;
	MemoryPool* _poolTable[MAX_ALLOC_SIZE + 1];
};

template<typename Type, typename... Args>
Type* Xnew(Args&&... args)
{
	Type* memory = static_cast<Type*>(Xalloc(sizeof(Type)));
	new(memory)Type(forward<Args>(args)...);
	return memory;
}

template<typename Type>
void Xdelete(Type* obj)
{
	obj->~Type();
	Xrelease(obj);
}

template<typename Type, typename...Args>
shared_ptr<Type> MakeShared(Args&&...args)
{
	return shared_ptr<Type>{ Xnew<Type>(std::forward<Args>(args)...), Xdelete<Type> };
}