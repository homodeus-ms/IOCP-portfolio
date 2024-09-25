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
		//cout << "클라이언트쪽의 세션" << endl;
		
	}
	virtual void OnRecvPacket(BYTE* buffer, int32 len) override
	{
		PacketSessionRef session = GetPacketSessionRef();
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		// PacketId 유효 체크

		ClientPacketHandler::HandlePacket(session, buffer, len);
	}
	virtual void OnSend(int32 len) override
	{
		
	}
	virtual void OnDisconnect() override
	{

	}
};

