#include "pch.h"
#include "Monster.h"
#include "GameRoom.h"
#include "Player.h"
#include "ServerPacketHandler.h"


Monster::Monster(VectorInt pos)
{
	_info.set_name("snake");
	
	Protocol::MoveStat* moveStat = _info.mutable_movestat();
	
	moveStat->set_dir(DIR_DOWN);
	moveStat->set_state(IDLE);
	moveStat->set_x(pos.x);
	moveStat->set_y(pos.y);

	Stat* stat = _info.mutable_stat();
	
	stat->set_hp(50);
	stat->set_maxhp(50);
	stat->set_attack(SNAKE_POWER);
	stat->set_defence(0);
	stat->set_speed(50);
}
Monster::~Monster()
{
	cout << "몬스터 소멸자" << endl;
}

void Monster::Update()
{
	switch (_info.movestat().state())
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

void Monster::UpdateIdle()
{
	if (_room == nullptr)
		return;

	if (!IsAlive())
		return;

	// 여기서 자꾸 오류가 남..
	SyncPosToMap(shared_from_this());
	

	_target = std::move(_room->FindClosestPlayer(GetCellPos()));
	
	if (_target)
	{
		const VectorInt& targetPos = _target->GetCellPos();
		const VectorInt& myPos = GetCellPos();
		int32 dist = abs(targetPos.x - myPos.x) + abs(targetPos.y - myPos.y);

		if (dist == 1)
		{
			// attack;
			SetDir(GetLookAtDir(targetPos));
			SetState(SKILL, true);
			if (_oldTarget != _target)
			{
				_waitUntil = GetTickCount64() + 100;
				_oldTarget = _target;
			}
			else
			{
				_waitUntil = GetTickCount64() + 1000;
			}
		}
		else
		{
			list<VectorInt> path;
			if (_room->FindPath(myPos, targetPos, path, SNAKE_SIGHT))
			{
				if (path.size() > 1)
				{
					// [1]에 있는 위치를 가져온다
					const VectorInt& pos0 = path.front();
					path.pop_front();
					VectorInt nextPos = path.front();
					if (_room->CanGo(nextPos))
					{
						SetDir(GetLookAtDir(nextPos));
						SetCellPos(nextPos);
						_waitUntil = GetTickCount64() + 900;
						
						SetState(MOVE, true);
					}
					else
					{
						SetCellPos(pos0);
					}
				}
			}
		}
	}
}
void Monster::UpdateMove()
{
	uint64 now = GetTickCount64();
	if (_waitUntil > now)
		return;

	SetState(IDLE);
}
void Monster::UpdateSkill()
{
	uint64 now = GetTickCount64();
	if (_waitUntil > now)
		return;
	
	if (_target)
	{
		VectorInt targetPos = _target->GetCellPos();
		
		if (targetPos.ManhattanDist(GetCellPos()) == 1)
		{
			Attack(_target);
		}
	}


	SetState(IDLE);
}

void Monster::Attack(GameObjectRef target)
{
	if (!target || target->GetCellPos().ManhattanDist(GetCellPos()) > 1)
		return;

	ObjectType targetType = target->GetObjectType();
	ASSERT_CRASH(targetType == ObjectType::OBJECT_TYPE_PLAYER);

	Stat* targetStat = target->GetStat();

	int32 attack = _info.mutable_stat()->attack();
	int32 damage = max(0, attack - targetStat->defence());
	targetStat->set_hp(max(0, targetStat->hp() - damage));

	Protocol::S_Attack pkt;
	pkt.set_attackerid(GetObjectID());
	pkt.set_targetid(target->GetObjectID());
	pkt.set_targethp(target->GetStat()->hp());
	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);

	if (_room)
		_room->Broadcast(sendBuffer);

	if (target && target->GetStat()->hp() <= 0)
		_room->OnDeath(target);
}
