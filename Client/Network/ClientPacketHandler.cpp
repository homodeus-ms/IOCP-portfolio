#include "pch.h"
#include "ClientPacketHandler.h"
#include "BufferReader.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "DevScene.h"
#include "Player.h"
#include "MyPlayer.h"
#include "Monster.h"
#include "Snake.h"
#include "GameObject.h"
#include "Effect.h"
#include "UiManager.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];


void ClientPacketHandler::Init()
{

	for (int32 i = 0; i < UINT16_MAX; ++i)
		GPacketHandler[i] = Handle_Invalid;

	GPacketHandler[S_MyPlayer] = [](PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		return HandlePacket<Protocol::S_MyPlayer>(Handle_S_MyPlayer, session, buffer, len);
	};

	GPacketHandler[S_AddObject] = [](PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		return HandlePacket<Protocol::S_AddObject>(Handle_S_AddObject, session, buffer, len);
	};

	GPacketHandler[S_ChangeState] = [](PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		return HandlePacket<Protocol::S_ChangeState>(Handle_S_ChangeState, session, buffer, len);
	};
	GPacketHandler[S_Move] = [](PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		return HandlePacket<Protocol::S_Move>(Handle_S_Move, session, buffer, len);
	};
	GPacketHandler[S_Attack] = [](PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		return HandlePacket<Protocol::S_Attack>(Handle_S_Attack, session, buffer, len);
	};
	GPacketHandler[S_RemoveObject] = [](PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		return HandlePacket<Protocol::S_RemoveObject>(Handle_S_RemoveObject, session, buffer, len);
	};
	GPacketHandler[S_Chat] = [](PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		return HandlePacket<Protocol::S_Chat>(Handle_S_Chat, session, buffer, len);
	};
}

bool Handle_Invalid(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// todo.. 
	return false;
}

bool Handle_S_MyPlayer(PacketSessionRef& session, Protocol::S_MyPlayer& pkt)
{
	const Protocol::ObjectInfo& info = pkt.info();

	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	if (scene)
	{
		scene->AddObjectInMap(info.objectid(), VectorInt(info.movestat().x(), info.movestat().y()));

		MyPlayer* myPlayer = scene->SpawnObject<MyPlayer>(VectorInt{ info.movestat().x(), info.movestat().y() });
		myPlayer->SetInfo(info);
		GET_SINGLE(SceneManager)->SetMyPlayer(myPlayer);
		GET_SINGLE(InputManager)->ConnectChatBoxWithPlayer(myPlayer);
		scene->AddPlayer(myPlayer);
	}

	return true;
}

bool Handle_S_AddObject(PacketSessionRef& session, Protocol::S_AddObject& pkt)
{
	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	if (scene)
		scene->Handle_S_AddObject(pkt);

	return true;
}

bool Handle_S_RemoveObject(PacketSessionRef& session, Protocol::S_RemoveObject& pkt)
{
	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	if (scene)
		scene->Handle_S_RemoveObject(pkt);

	return true;
}

bool Handle_S_Move(PacketSessionRef& session, Protocol::S_Move& pkt)
{
	uint64 id = pkt.info().objectid();
	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	GameObject* gameObject = scene->GetGameObject(id);

	if (pkt.modified() == true)
	{
		Protocol::ObjectInfo& playerInfo = gameObject->GetInfo();
		Protocol::MoveStat* movestat = playerInfo.mutable_movestat();

		const Protocol::ObjectInfo& pktInfo = pkt.info();
		const Protocol::MoveStat& pktMovestat = pktInfo.movestat();

		//movestat->set_state(MOVE);

		movestat->set_x(pktMovestat.x());
		movestat->set_y(pktMovestat.y());

		gameObject->SetDestPos(gameObject->GetCellPos());
		Pos dest = scene->CellPosToOriginPos(gameObject->GetCellPos());
		gameObject->SetPos(dest);
		
		return false;
	}

	if (GET_SINGLE(SceneManager)->GetMyPlayerId() == id || gameObject == nullptr)
		return false;


	ObjectType objectType = gameObject->GetObjectType();
	Protocol::ObjectInfo& info = gameObject->GetInfo();
	const Protocol::ObjectInfo& pktInfo = pkt.info();

	VectorInt from = gameObject->GetCellPos();
	int32 destX = pktInfo.movestat().x();
	int32 destY = pktInfo.movestat().y();

	if (from.x == destX && from.y == destY)
	{
		info = pktInfo;
		gameObject->UpdateAnimation();
		return false;
	}

	if (scene)
	{
		if (objectType == ObjectType::OBJECT_TYPE_PLAYER || objectType == ObjectType::OBJECT_TYPE_MONSTER)
		{
			if (scene->GetObjectIdInMap(from) != id)
			{
				from = scene->FindObjectPosInMap(id);
			}

			scene->MoveObjectInMap(id, from, VectorInt(destX, destY));

		}
		info = pktInfo;
		gameObject->UpdateAnimation();
		gameObject->SetDestPos(VectorInt(destX, destY));
		
	}

	return true;
}

bool Handle_S_Attack(PacketSessionRef& session, Protocol::S_Attack& pkt)
{
	uint64 attackerId = pkt.attackerid();
	uint64 targetId = pkt.targetid();
	int32 targetHp = pkt.targethp();

	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	if (scene)
	{
		GameObject* attacker = scene->GetGameObject(attackerId);
		GameObject* target = scene->GetGameObject(targetId);

		if (attacker == nullptr)
			return false;

		ObjectType attackerType = attacker->GetObjectType();

		
		if (attackerType == ObjectType::OBJECT_TYPE_PLAYER && attackerId != GET_SINGLE(SceneManager)->GetMyPlayerId())
			attacker->GetInfo().mutable_movestat()->set_state(SKILL);

		if (target)
		{
			target->GetInfo().mutable_stat()->set_hp(targetHp);
			scene->SpawnObject<Effect>(target->GetCellPos());
		}
	}
	return true;
}

bool Handle_S_ChangeState(PacketSessionRef& session, Protocol::S_ChangeState& pkt)
{
	uint64 id = pkt.info().objectid();
	MyPlayer* myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();
	Protocol::MoveStat pktStat = pkt.info().movestat();

	if (myPlayer->GetObjectId() == id)
	{
		return false;
	}


	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	if (scene)
	{
		GameObject* object = scene->GetGameObject(id);
		ASSERT_CRASH(object != nullptr);
		
		Protocol::MoveStat* objectStat = object->GetInfo().mutable_movestat();
		
		object->GetInfo().set_weapontype(pkt.info().weapontype());
		objectStat->set_dir(pktStat.dir());
		objectStat->set_state(pktStat.state());
		object->UpdateAnimation();

		if (object->GetState() == IDLE)
		{
			object->SetPos(object->GetDestPos());
		}
		
		
	}

	return true;
}

bool Handle_S_Chat(PacketSessionRef& session, Protocol::S_Chat& pkt)
{
	wstring str = ClientPacketHandler::BytesToWstring(pkt.message());
	GET_SINGLE(InputManager)->GetChatBox()->AddStr(str);

	return true;
}


std::wstring ClientPacketHandler::BytesToWstring(const std::string& byteData)
{
	const wchar_t* data = reinterpret_cast<const wchar_t*>(byteData.data());
	size_t dataSize = byteData.size() / sizeof(wchar_t);
	return std::wstring(data, dataSize);
}
