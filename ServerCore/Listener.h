#pragma once
#include "IocpCore.h"

class AcceptEvent;
class ServerService;

class Listener : public IocpObject
{
public:
	Listener();
	~Listener();

	// 외부에서 사용
	bool StartAccept(ServerServiceRef service);

	// 인터페이스 구현
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

	// 수신 관련
	void RegisterAccept(AcceptEvent* acceptEvent);
	void ProcessAccept(AcceptEvent* acceptEvent);

private:
	SOCKET _socket = INVALID_SOCKET;
	vector<AcceptEvent*> _acceptEvents;
	ServerServiceRef _service;
};

