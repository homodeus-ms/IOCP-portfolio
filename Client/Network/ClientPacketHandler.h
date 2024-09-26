#pragma once
// Temp g_ChatBox 사용하기 위해서..
#include "Game.h";

using PacketHandlerFunc = std::function<bool(PacketSessionRef&, BYTE*, int32)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];

enum
{
	S_EnterGame = 2,

	S_MyPlayer = 4,
	S_AddObject = 5,

	C_ChangeState = 7,
	S_ChangeState = 8,

	C_TryMove = 11,
	S_Move = 12,

	C_TryAttack = 15,
	S_Attack = 16,

	C_TryRemoveObject = 20,
	S_RemoveObject = 21,

	C_Chat = 30,
	S_Chat = 31,
};

// Handler 함수
bool Handle_Invalid(PacketSessionRef& session, BYTE* buffer, int32 len);
//bool Handle_S_EnterGame(PacketSessionRef& session, BYTE* buffer, int32 len);
bool Handle_S_MyPlayer(PacketSessionRef& session, Protocol::S_MyPlayer& pkt);
bool Handle_S_AddObject(PacketSessionRef& session, Protocol::S_AddObject& pkt);
bool Handle_S_RemoveObject(PacketSessionRef& session, Protocol::S_RemoveObject& pkt);
bool Handle_S_Move(PacketSessionRef& session, Protocol::S_Move& pkt);
bool Handle_S_Attack(PacketSessionRef& session, Protocol::S_Attack& pkt);
bool Handle_S_ChangeState(PacketSessionRef& session, Protocol::S_ChangeState& pkt);
bool Handle_S_Chat(PacketSessionRef& session, Protocol::S_Chat& pkt);

class ClientPacketHandler
{
public:

	static void Init();

	static bool HandlePacket(PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->packetId](session, buffer, len);
	}

	static std::wstring BytesToWstring(const std::string& byteData);

	// 보내는 패킷을 만드는 함수들
	static SendBufferRef MakeSendBuffer(Protocol::C_TryMove& pkt) { return MakeSendBuffer(pkt, C_TryMove); }
	static SendBufferRef MakeSendBuffer(Protocol::C_ChangeState& pkt) { return MakeSendBuffer(pkt, C_ChangeState); }
	static SendBufferRef MakeSendBuffer(Protocol::C_TryRemoveObject& pkt) { return MakeSendBuffer(pkt, C_TryRemoveObject); }
	static SendBufferRef MakeSendBuffer(Protocol::C_TryAttack& pkt) { return MakeSendBuffer(pkt, C_TryAttack); }
	static SendBufferRef MakeSendBuffer(Protocol::C_Chat& pkt) { return MakeSendBuffer(pkt, C_Chat); }



private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketType pkt;
		if (pkt.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)) == false)
			return false;
		return func(session, pkt);
	}

	template<typename T>
	static SendBufferRef MakeSendBuffer(T& pkt, uint16 pktId)
	{
		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
		const uint16 packetSize = dataSize + sizeof(PacketHeader);

		SendBufferRef sendBuffer = MakeShared<SendBuffer>(packetSize);
		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->packetSize = packetSize;
		header->packetId = pktId;
		assert(pkt.SerializeToArray(&header[1], dataSize));
		sendBuffer->SetWritePos(packetSize);

		return sendBuffer;
	}
};

