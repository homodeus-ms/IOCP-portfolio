#include "pch.h"
#include "IocpCore.h"
#include "IocpEvent.h"

IocpCore::IocpCore()
{
	_iocpCore = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	ASSERT_CRASH(_iocpCore != INVALID_HANDLE_VALUE);
}

IocpCore::~IocpCore()
{
	::CloseHandle(_iocpCore);
}

bool IocpCore::Register(IocpObjectRef iocpObject)
{
	// Key를 따로 보낼 필요는 없음
	return ::CreateIoCompletionPort(iocpObject->GetHandle(), _iocpCore, 0, 0);
}

bool IocpCore::Dispatch(uint32 timeLimit)
{
	DWORD numOfBytes = 0;
	ULONG_PTR key = 0;
	IocpEvent* iocpEvent = nullptr;

	if (::GetQueuedCompletionStatus(_iocpCore, OUT &numOfBytes, OUT &key, 
		OUT reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), timeLimit))
	{
		IocpObjectRef iocpObject = iocpEvent->_owner;
		iocpObject->Dispatch(iocpEvent, numOfBytes);
	}
	else
	{
		const int32 errCode = ::WSAGetLastError();
		switch (errCode)
		{
		case WAIT_TIMEOUT:
			return false;
		default:
			// TODO.. 로그 찍고, 일단은 Dispatch로 간다
			IocpObjectRef iocpObject = iocpEvent->_owner;
			iocpObject->Dispatch(iocpEvent, numOfBytes);
			break;
		}
	}

	return true;
}


