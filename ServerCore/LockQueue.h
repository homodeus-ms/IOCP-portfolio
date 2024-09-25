#pragma once

template<typename T>
class LockQueue
{
public:
	void Push(T item)
	{
		WRITE_LOCK;
		_items.push(item);
	}
	T Pop()
	{
		WRITE_LOCK;
		if (_items.empty())
			return T();
		T ret = _items.front();
		_items.pop();
		return ret;
	}
	void PopAll(OUT vector<T>& items)
	{
		WRITE_LOCK;
		int32 currQueueSize = _items.size();
		for (int32 i = 0; i < currQueueSize; ++i)
		{
			items.push_back(_items.front());
			_items.pop();
		}

		// ���� �ڵ忡�� �ؿ�ó�� �Ǿ� �ִµ� ��ó�� �ϴ°� �� ������ �� ����
		/*while (T item = Pop())
			items.push_back(item);*/
	}
	void Clear()
	{
		WRITE_LOCK;
		_items = queue<T>();
	}

private:
	USE_LOCK;
	queue<T> _items;
};

