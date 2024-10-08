#pragma once
#include "Job.h"
#include "LockQueue.h"

class JobQueue : public enable_shared_from_this<JobQueue>
{
	friend class ThreadManager;

public:

	void DoAsync(CallbackType&& callback)
	{
		Push(MakeShared<Job>(std::move(callback)));
	}

	template<typename T, typename Ret, typename... Args>
	void DoAsync(Ret(T::* memFunc)(Args...), Args... args)
	{
		shared_ptr<T> owner = static_pointer_cast<T>(shared_from_this());
		Push(MakeShared<Job>(owner, memFunc, std::forward<Args>(args)...));
	}
	void ClearJobs() { _jobs.Clear(); }

private:
	void Push(JobRef&& job);
	void Execute();

protected:
	LockQueue<JobRef> _jobs;
	Atomic<int32> _jobCount = 0;

};

