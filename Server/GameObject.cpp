#include "pch.h"
#include "GameObject.h"
#include "Player.h"
#include "Monster.h"
#include "Projectile.h"
#include "GameRoom.h"
#include "ServerPacketHandler.h"

atomic<uint64> GameObject::s_id = 1;

GameObject::~GameObject()
{
	_room = nullptr;
}

void GameObject::Update()
{

}

void GameObject::Attack(GameObjectRef target)
{
}

// id의 앞 32비트에 타입, 뒤 32비트가 id
PlayerRef GameObject::CreatePlayer()
{
	PlayerRef player = make_shared<Player>();
	GameObjectRef object = static_pointer_cast<GameObject>(player);
	SetId(object, ObjectType::OBJECT_TYPE_PLAYER);
	
	return player;
}

MonsterRef GameObject::CreateMonster(VectorInt pos)
{
	//MonsterRef monster = make_shared<Monster>(pos);

	MonsterRef monster = ObjectPool<Monster>::MakeShared<Monster>(pos);
	GameObjectRef object = static_pointer_cast<GameObject>(monster);
	SetId(object, ObjectType::OBJECT_TYPE_MONSTER);

	return monster;
}

ProjectileRef GameObject::CreateProjectile(PlayerRef owner)
{
	//ProjectileRef projectile = make_shared<Projectile>(owner);
	ProjectileRef projectile = ObjectPool<Projectile>::MakeShared<Projectile>(owner);
	GameObjectRef object = static_pointer_cast<GameObject>(projectile);
	SetId(object, ObjectType::OBJECT_TYPE_PROJECTILE);

	return projectile;
}

void GameObject::SetId(GameObjectRef& object, ObjectType type)
{
	Protocol::ObjectInfo& info = object->GetInfo();
	int64 id = s_id++;

	int64 flag = static_cast<int64>(type);
	flag <<= 32;
	id |= flag;
	
	info.set_objectid(id);
}

ObjectType GameObject::GetObjectType()
{
	int64 id = _info.objectid();
	id >>= 32;
	return static_cast<ObjectType>(id);
}

void GameObject::SetState(ObjectState state, bool broadcast)
{
	if (_info.movestat().state() == state)
		return;
	_info.mutable_movestat()->set_state(state);
	if (broadcast)
		BroadcastMove();
}
void GameObject::SetDir(Dir dir, bool broadcast)
{
	if (_info.movestat().dir() == dir)
		return;
	_info.mutable_movestat()->set_dir(dir);
	if (broadcast)
		BroadcastMove();
}

bool GameObject::CanGo(VectorInt dest)
{
	if (_room == nullptr)
		return false;
	return _room->CanGo(dest);
}
Dir GameObject::GetLookAtDir(VectorInt dest)
{
	VectorInt dir = dest - GetCellPos();
	if (dir.x > 0)
		return DIR_RIGHT;
	else if (dir.x < 0)
		return DIR_LEFT;
	else if (dir.y > 0)
		return DIR_DOWN;
	else
		return DIR_UP;
}

void GameObject::SetCellPos(VectorInt cellPos, bool broadcast)
{
	Protocol::MoveStat* moveStat = _info.mutable_movestat();
	if (GetObjectType() != ObjectType::OBJECT_TYPE_PROJECTILE)
		_room->MoveObjectInObjectMap(VectorInt{ moveStat->x(), moveStat->y() }, cellPos);
	moveStat->set_x(cellPos.x);
	moveStat->set_y(cellPos.y);

	if (broadcast)
		BroadcastMove();
}

VectorInt GameObject::GetFrontCellPos()
{
	VectorInt pos = GetCellPos();

	switch (_info.movestat().dir())
	{
	case DIR_UP:
		return pos + VectorInt{ 0, -1 };
	case DIR_DOWN:
		return pos + VectorInt{ 0, 1 };
	case DIR_LEFT:
		return pos + VectorInt{ -1, 0 };
	case DIR_RIGHT:
		return pos + VectorInt{ 1, 0 };
	}

	ASSERT_CRASH(FALSE);
	return pos;
}

Protocol::MoveStat* GameObject::GetMoveStat()
{
	return _info.mutable_movestat();;
}

Protocol::Stat* GameObject::GetStat()
{
	return _info.mutable_stat();
}


void GameObject::SyncPosToMap(GameObjectRef object)
{
	VectorInt currPos = object->GetCellPos();
	uint64 objectId = object->GetObjectID();
	GameObjectRef objectInMap = _room->GetObjectInMap(currPos.x, currPos.y);
	if (objectInMap && objectInMap->GetObjectID() == objectId)
		return;

	cout << "==== Log : Pos UnSync  ====" << endl;

	VectorInt whereIs = _room->GetObjectPosFromMap(objectId);
	_room->MoveObjectInObjectMap(whereIs, currPos);
}

void GameObject::BroadcastMove()
{
	if (_room)
	{
		Protocol::S_Move pkt;
		*pkt.mutable_info() = _info;

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		_room->Broadcast(sendBuffer);
	}
}

void GameObject::BroadcastChangeState()
{
	if (_room)
	{
		Protocol::S_ChangeState pkt;
		*pkt.mutable_info() = _info;

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		_room->Broadcast(sendBuffer);
	}
}
