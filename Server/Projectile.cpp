#include "pch.h"
#include "Projectile.h"
#include "Player.h"
#include "ServerPacketHandler.h"
#include "GameRoom.h"

Projectile::Projectile(PlayerRef player)
	: _owner(player)
	, _originPos(player->GetCellPos())
{
	_info.set_name("Arrow");

	MoveStat* movestat = GetInfo().mutable_movestat();
	movestat->set_dir(player->GetDir());
	movestat->set_x(_originPos.x);
	movestat->set_y(_originPos.y);
	movestat->set_state(IDLE);
	
	Stat* stat = _info.mutable_stat();
	stat->set_attack(BOW_POWER);
	stat->set_hp(1);
	stat->set_speed(ARROW_SPEED);

	_info.set_ownerid(player->GetObjectID());
	_startWait = GetTickCount64() + 300;
}

Projectile::~Projectile()
{
	cout << "ȭ�� �Ҹ���" << endl;
}

void Projectile::Update()
{
	ObjectState state = GetMoveStat()->state();

	switch (state)
	{
	case IDLE:
		UpdateIdle();
		break;
	case MOVE:
		UpdateMove();
		break;
	case SKILL:
		UpdateSkill();
		break;
	}
}

void Projectile::Attack(GameObjectRef target)
{
	if (target == nullptr || target->GetObjectType() != ObjectType::OBJECT_TYPE_MONSTER)
		return;

	ObjectType targetType = target->GetObjectType();

	Protocol::Stat* stat = target->GetStat();
	int32 attack = BOW_POWER;
	int32 damage = attack - stat->defence();
	stat->set_hp(max(0, stat->hp() - damage));

	Protocol::S_Attack pkt;
	pkt.set_attackerid(GetObjectID());
	pkt.set_targetid(target->GetObjectID());
	pkt.set_targethp(target->GetStat()->hp());
	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);

	_room->Broadcast(sendBuffer);

	if (target && target->GetStat()->hp() <= 0)
		_room->OnDeath(target);

	_isAlive = false;
	_room->RemoveObject(GetObjectID());
	
}

uint64 Projectile::GetOwnerID()
{
	ASSERT_CRASH(!_owner.expired());
	
	return _owner.lock()->GetObjectID();
}

void Projectile::UpdateIdle()
{
	if (!_isAlive)
		return;

	uint64 now = GetTickCount64();
	if (_startWait > now)
		return;

	Dir dir = GetDir();
	VectorInt nextPos = GetCellPos() + DELTA_XY[dir];

	if (_room == nullptr)
		return;

	if (_originPos.ManhattanDist(nextPos) >= ARROW_MAX_DIST)
	{
		_isAlive = false;
		_room->RemoveObject(GetObjectID());
		
		return;
	}

	
	// ȭ���� ��Ȥ ���͸� ����ع����� ������ �߻��߾��� (������ ������ ����)
	const VectorInt& currPos = GetCellPos();
	{
		GameObjectRef target = _room->GetObjectInMap(currPos.x, currPos.y);
		if (currPos != _originPos && target && target->GetObjectType() == ObjectType::OBJECT_TYPE_MONSTER)
		{
			Attack(target);
			return;
		}
	}

	if (_room->CanGo(nextPos, GetOwnerID()))
	{
		SetCellPos(nextPos);
		_waitUntil = GetTickCount64() + 100;
		SetState(MOVE, true);
		return;
	}
	
	GameObjectRef target = _room->GetObjectInMap(nextPos.x, nextPos.y);
	
	if (target && target->GetObjectType() == ObjectType::OBJECT_TYPE_MONSTER)
		Attack(target);
	else
	{
		_isAlive = false;
		_room->RemoveObject(GetObjectID());
	}
		
}

void Projectile::UpdateMove()
{
	uint64 now = GetTickCount64();
	if (_waitUntil > now)
		return;

	SetState(IDLE);
}

void Projectile::UpdateSkill()
{
}

