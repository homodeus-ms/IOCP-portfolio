#pragma once
//#include "Service.h"
#include "IocpEvent.h"
#include "RecvBuffer.h"
#include "SendBuffer.h"
#include "IocpCore.h"
#include "NetAddress.h"

class Service;

class Session : public IocpObject
{
	friend class Listener;
	friend class IocpCore;
	friend class Service;
public:
	Session();
	virtual ~Session();

	// ���� Event Ref ����
	void InitSession();
	void ClearRefs();

	// �ܺο��� ����� �Լ�
	bool Connect();
	void Disconnect(const WCHAR* cause);
	void Send(SendBufferRef sendBuffer);

	// �������̽� ����
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

	// getter | setter
	SOCKET GetSocket() const { return _socket; }
	void SetService(shared_ptr<Service> service) { _service = service; }
	shared_ptr<Service> GetService() const { return _service.lock(); }
	void SetNetAddress(NetAddress address) { _address = address; }
	NetAddress GetAddress() const { return _address; }
	bool IsConnected() const { return _connected; }
	SessionRef GetSessionRef() { return static_pointer_cast<Session>(shared_from_this()); }

protected:
	// ������ �ڵ忡�� ������
	virtual void OnConnect() {}
	virtual int32 OnRecv(BYTE* buffer, int32 len) { return len; }
	virtual void OnDisconnect() {}
	virtual void OnSend(int32 len) {}

private:
	// �ۼ��� Register, Porcess �Լ�
	bool RegisterConnect();
	bool RegisterDisconnect();
	void RegisterRecv();
	void RegisterSend();

	void ProcessConnect();
	void ProcessDisconnect();
	void ProcessRecv(int32 numOfBytes);
	void ProcessSend(int32 numOfBytes);

	void HandleError(int32 errCode);

private:
	SOCKET _socket = INVALID_SOCKET;
	NetAddress _address = {};
	weak_ptr<Service> _service;
	Atomic<bool> _connected = false;

private:

	USE_LOCK;

	// �ۼ��� ����
	RecvBuffer _recvBuffer;

	queue<SendBufferRef> _sendQueue;
	vector<SendBufferRef> _pendingSendBuffers;
	Atomic<bool> _sendRegistered = false;


private:
	// ���� Event
	ConnectEvent _connectEvent;
	DisconnectEvent _disconnectEvent;
	RecvEvent _recvEvent;
	SendEvent _sendEvent;
};



/*---------------------------
		Packet Session
----------------------------*/

struct PacketHeader
{
	uint16 packetSize;
	uint16 packetId;
};


class PacketSession : public Session
{
public:
	PacketSession();
	virtual ~PacketSession();

	PacketSessionRef GetPacketSessionRef() { return static_pointer_cast<PacketSession>(shared_from_this()); }

protected:
	virtual int32 OnRecv(BYTE* buffer, int32 len) sealed;
	virtual void OnRecvPacket(BYTE* buffer, int32 len) abstract;
};
