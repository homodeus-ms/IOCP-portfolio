#pragma once
#include "MemoryPool.h"

template<typename Type>
class ObjectPool
{
public:

	template<typename...Args>
	static Type* Pop(Args&&...args)
	{
		Type* memory = reinterpret_cast<Type*>(MemoryHeader::DetachHeader(s_pool.Pop(), sizeof(Type)));
		new(memory)Type(std::forward<Args>(args)...);
		return memory;
	}

	static void Push(Type* obj)
	{
		obj->~Type();
		s_pool.Push(MemoryHeader::AttachHeader(obj));
	}

	template<typename Type, typename...Args>
	static shared_ptr<Type> MakeShared(Args&&... args)
	{
		shared_ptr<Type> ptr = { Pop(std::forward<Args>(args)...), Push };
		return ptr;
	}

private:
	static MemoryPool s_pool;
	static int32 s_allocSize;
};

template<typename Type>
int32 ObjectPool<Type>::s_allocSize = sizeof(Type) + sizeof(MemoryHeader);

template<typename Type>
MemoryPool ObjectPool<Type>::s_pool = { s_allocSize };