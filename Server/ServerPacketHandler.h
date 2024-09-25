#pragma once
#include "Protocol.pb.h"

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

// Handler 함수들
bool Handle_Invalid(PacketSessionRef& session, BYTE* buffer, int32 len);
bool Handle_C_ChangeState(PacketSessionRef& session, Protocol::C_ChangeState& pkt);
bool Handle_C_TryMove(PacketSessionRef& session, Protocol::C_TryMove& pkt);
bool Handle_C_TryAttack(PacketSessionRef& session, Protocol::C_TryAttack& pkt);
bool Handle_C_TryRemoveObject(PacketSessionRef& session, Protocol::C_TryRemoveObject& pkt);
bool Handle_C_Chat(PacketSessionRef& session, Protocol::C_Chat& pkt);


class ServerPacketHandler
{
public:
	static void Init();

	static bool HandlePacket(PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->packetId](session, buffer, len);
	}

	// 이 부분을 자동화 처리 할 수 있겠음
	// 그렇지만 일단은 넘어감. 지금 나의 수준에서는 큰 그림에 집중하는 게 더 좋겠음.
	static SendBufferRef MakeSendBuffer(Protocol::S_EnterGame& pkt) { return MakeSendBuffer(pkt, S_EnterGame); }
	static SendBufferRef MakeSendBuffer(Protocol::S_MyPlayer& pkt) { return MakeSendBuffer(pkt, S_MyPlayer); }
	static SendBufferRef MakeSendBuffer(Protocol::S_AddObject& pkt) { return MakeSendBuffer(pkt, S_AddObject); }
	static SendBufferRef MakeSendBuffer(Protocol::S_RemoveObject& pkt) { return MakeSendBuffer(pkt, S_RemoveObject); }
	static SendBufferRef MakeSendBuffer(Protocol::S_Move& pkt) { return MakeSendBuffer(pkt, S_Move); }
	static SendBufferRef MakeSendBuffer(Protocol::S_Attack& pkt) { return MakeSendBuffer(pkt, S_Attack); }
	static SendBufferRef MakeSendBuffer(Protocol::S_ChangeState& pkt) { return MakeSendBuffer(pkt, S_ChangeState); }
	static SendBufferRef MakeSendBuffer(Protocol::S_Chat& pkt) { return MakeSendBuffer(pkt, S_Chat); }
	
	
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
	static SendBufferRef MakeSendBuffer(T& pkt, uint16 pktId);
};

template<typename T>
SendBufferRef ServerPacketHandler::MakeSendBuffer(T& pkt, uint16 pktId)
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

