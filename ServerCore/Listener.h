#pragma once
#include "IocpCore.h"

class AcceptEvent;
class ServerService;

class Listener : public IocpObject
{
public:
	Listener();
	~Listener();

	// �ܺο��� ���
	bool StartAccept(ServerServiceRef service);

	// �������̽� ����
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

	// ���� ����
	void RegisterAccept(AcceptEvent* acceptEvent);
	void ProcessAccept(AcceptEvent* acceptEvent);

private:
	SOCKET _socket = INVALID_SOCKET;
	vector<AcceptEvent*> _acceptEvents;
	ServerServiceRef _service;
};

