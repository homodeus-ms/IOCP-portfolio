#include "pch.h"
#include "Player.h"
#include "GameRoom.h"
#include "ServerPacketHandler.h"
#include "Projectile.h"


Player::Player()
{
	Protocol::MoveStat* moveStat = _info.mutable_movestat();
	moveStat->set_dir(DIR_DOWN);
	moveStat->set_state(IDLE);

	VectorInt pos = GetPlayerSpawnPos();

	moveStat->set_x(pos.x);
	moveStat->set_y(pos.y);
	
	_info.set_name("AA");
	_info.set_weapontype(SWORD);

	Stat* stat = _info.mutable_stat();
	stat->set_hp(100);
	stat->set_maxhp(100);
	stat->set_attack(SWORD_POWER);
	stat->set_defence(10);
	stat->set_speed(180);
}

void Player::SetWeapon(WeaponType type)
{
	if (GetWeaponType() == type)
		return;
	_info.set_weapontype(type);
}

VectorInt Player::GetPlayerSpawnPos()
{
	VectorInt pos(5, 5);
	if (GRoom->GetObjectInMap(pos.x, pos.y) == nullptr)
		return pos;

	const int32 depth = 10;
	VectorInt result = pos;

	for (int i = 0; i < depth; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			result = pos + DELTA_XY[j];
			if (GRoom->GetObjectInMap(result.x, result.y) == nullptr)
			{
				if (GRoom->CanGo(result))
					return result;
			}
		}
		
		pos.y += 1;
	}

	// ������ġ ������ �÷��̾�� �� ���ִ� ��Ȳ.
	// Todo..
	ASSERT_CRASH(false);
	const VectorInt tempSpawnPos(0, 6);

	return tempSpawnPos;
}

void Player::Attack(GameObjectRef target)
{
	ASSERT_CRASH(_room != nullptr);
	
	// todo.. �� �Լ� �и��� �� �ؾ���

	if (GetWeaponType() == WeaponType::WEAPON_TYPE_BOW)
	{
		ProjectileRef arrow = CreateProjectile(static_pointer_cast<Player>(shared_from_this()));
		_room->AddObject(static_pointer_cast<GameObject>(arrow));
		
		Protocol::S_Attack pkt;
		pkt.set_attackerid(GetObjectID());
		pkt.set_targetid(0);
		pkt.set_targethp(0);
		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		_room->Broadcast(sendBuffer);

		return;
	}

	if (target == nullptr || target->GetObjectType() == ObjectType::OBJECT_TYPE_PLAYER)
	{
		Protocol::S_Attack pkt;
		pkt.set_attackerid(GetObjectID());
		pkt.set_targetid(0);
		pkt.set_targethp(0);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		_room->Broadcast(sendBuffer);
		return;
	}

	// ���� ���Ⱑ sword�� ���

	ObjectType targetType = target->GetObjectType();
	ASSERT_CRASH(targetType == ObjectType::OBJECT_TYPE_MONSTER);

	if (GetWeaponType() != WeaponType::WEAPON_TYPE_SWORD)
	{
		cout << "Log : WeaponType SWORD BUG!" << endl;
		_info.set_weapontype(WeaponType::WEAPON_TYPE_SWORD);

		
	}
	

	Protocol::Stat* stat = target->GetStat();
	int32 attack = SWORD_POWER;
	int32 damage = max(0, attack - stat->defence());
	stat->set_hp(max(0, stat->hp() - damage));

	Protocol::S_Attack pkt;
	pkt.set_attackerid(GetObjectID());
	pkt.set_targetid(target->GetObjectID());
	pkt.set_targethp(target->GetStat()->hp());

	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	_room->Broadcast(sendBuffer);

	if (target && target->GetStat()->hp() <= 0)
		_room->OnDeath(target);
}

