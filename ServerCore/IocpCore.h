#pragma once

class IocpObject : public enable_shared_from_this<IocpObject>
{
public:
	virtual HANDLE GetHandle() abstract;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) abstract;
};

class IocpCore sealed
{
public:
	IocpCore();
	~IocpCore();

	HANDLE GetHandle() const { return _iocpCore; }
	bool Register(IocpObjectRef iocpObject);
	bool Dispatch(uint32 timeLimit = INFINITE);

private:
	HANDLE _iocpCore = INVALID_HANDLE_VALUE;

};

