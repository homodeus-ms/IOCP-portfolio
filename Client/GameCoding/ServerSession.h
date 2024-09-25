#pragma once
#include "ClientPacketHandler.h"


class ServerSession : public PacketSession
{
public:
	~ServerSession()
	{
		cout << "ServerSession Disconnect!" << endl;
	}

	virtual void OnConnect() override
	{
		//cout << "Ŭ���̾�Ʈ���� ����" << endl;
		
	}
	virtual void OnRecvPacket(BYTE* buffer, int32 len) override
	{
		PacketSessionRef session = GetPacketSessionRef();
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		// PacketId ��ȿ üũ

		ClientPacketHandler::HandlePacket(session, buffer, len);
	}
	virtual void OnSend(int32 len) override
	{
		
	}
	virtual void OnDisconnect() override
	{

	}
};

