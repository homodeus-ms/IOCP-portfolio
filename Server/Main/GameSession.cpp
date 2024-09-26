#include "pch.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "ServerPacketHandler.h"
#include "GameRoom.h"

void GameSession::OnConnect()
{
	GSessionManager.AddSession(static_pointer_cast<GameSession>(shared_from_this()));

	//Send(ServerPacketHandler::Make_S_EnterGame());

	GRoom->EnterRoom(GetGameSessionRef());
}

void GameSession::OnDisconnect()
{
	GSessionManager.RemoveSession(static_pointer_cast<GameSession>(shared_from_this()));

	GRoom->LeaveRoom(GetGameSessionRef());
}

void GameSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	PacketSessionRef session = GetPacketSessionRef();
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

	// PacketID 가 유효한 지 여기서 체크

	ServerPacketHandler::HandlePacket(session, buffer, len);
}

void GameSession::OnSend(int32 len)
{
	//cout << "GameSession(Sever) Send!" << endl;
}
