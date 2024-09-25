#include "pch.h"
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "GameRoom.h"
#include "GameSessionManager.h"
#include "GameSession.h"
#include "Player.h"


PacketHandlerFunc GPacketHandler[UINT16_MAX];


void ServerPacketHandler::Init()
{
    for (int32 i = 0; i < UINT16_MAX; ++i)
        GPacketHandler[i] = Handle_Invalid;

    GPacketHandler[C_ChangeState] = [](PacketSessionRef& session, BYTE* buffer, int32 len)
    {
        return HandlePacket<Protocol::C_ChangeState>(Handle_C_ChangeState, session, buffer, len);
    };
    GPacketHandler[C_TryMove] = [](PacketSessionRef& session, BYTE* buffer, int32 len)
    {
        return HandlePacket<Protocol::C_TryMove>(Handle_C_TryMove, session, buffer, len);
    };
    GPacketHandler[C_TryAttack] = [](PacketSessionRef& session, BYTE* buffer, int32 len)
    {
        return HandlePacket<Protocol::C_TryAttack>(Handle_C_TryAttack, session, buffer, len);
    };
    GPacketHandler[C_TryRemoveObject] = [](PacketSessionRef& session, BYTE* buffer, int32 len)
    {
        return HandlePacket<Protocol::C_TryRemoveObject>(Handle_C_TryRemoveObject, session, buffer, len);
    };
    GPacketHandler[C_Chat] = [](PacketSessionRef& session, BYTE* buffer, int32 len)
    {
        return HandlePacket<Protocol::C_Chat>(Handle_C_Chat, session, buffer, len);
    };

}

bool Handle_Invalid(PacketSessionRef& session, BYTE* buffer, int32 len)
{
    PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
    // todo.. 
    return false;
}



bool Handle_C_ChangeState(PacketSessionRef& session, Protocol::C_ChangeState& pkt)
{
    GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

    GRoom->DoAsync(&GameRoom::ChangeState, pkt.info());

    return true;
}

bool Handle_C_TryMove(PacketSessionRef& session, Protocol::C_TryMove& pkt)
{
    /*GameObjectRef object = GRoom->FindObject(pkt.id());
    GameObjectRef mapObj = GRoom->GetObjectInMap(pkt.fromx(), pkt.fromy());

    if (object != mapObj)
    {
        VectorInt whereIs = GRoom->GetObjectPosFromMap(pkt.id());
        cout << "Move Log" << endl;
        cout << whereIs.x << " , " << whereIs.y << endl;
        return false;
    }
    

    cout << "Move Log" << endl;
    cout << "From : " << pkt.fromx() << " , " << pkt.fromy() << " -> " << pkt.destx()
        << " , " << pkt.desty() << endl;*/

    GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

    GRoom->DoAsync(&GameRoom::TryMove, pkt);

    

    return true;
}

bool Handle_C_TryAttack(PacketSessionRef& session, Protocol::C_TryAttack& pkt)
{
    GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

    GRoom->DoAsync(&GameRoom::TryAttack, pkt.attackerid(), pkt.destx(), pkt.desty());

    return true;
}

bool Handle_C_TryRemoveObject(PacketSessionRef& session, Protocol::C_TryRemoveObject& pkt)
{
    GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

    GRoom->DoAsync(&GameRoom::RemoveObject, pkt.id());

    return true;
}

bool Handle_C_Chat(PacketSessionRef& session, Protocol::C_Chat& pkt)
{
    GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

    GRoom->DoAsync(&GameRoom::Chat, pkt);

    return true;
}
