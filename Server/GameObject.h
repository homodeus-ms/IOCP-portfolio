#pragma once

class Player;
class Monster;
class Projectile;

class GameObject : public enable_shared_from_this<GameObject>
{
public:
	GameObject() {}
	virtual ~GameObject();

	virtual void Update();
	virtual void Attack(GameObjectRef target);

	static PlayerRef CreatePlayer();
	static MonsterRef CreateMonster(VectorInt pos);
	static ProjectileRef CreateProjectile(PlayerRef owner);
	static void SetId(GameObjectRef& object, ObjectType type);
	
	ObjectType GetObjectType();

	void SetState(ObjectState state, bool broadcast = false);
	void SetDir(Dir dir, bool broadcast = false);
	Dir GetDir() { return _info.mutable_movestat()->dir(); }
	bool IsAlive() { return _info.stat().hp() > 0; }
	bool CanGo(VectorInt dest);
	Dir GetLookAtDir(VectorInt dest);

	void SetCellPos(VectorInt cellPos, bool broadcast = false);
	VectorInt GetCellPos() { return VectorInt{ _info.movestat().x(), _info.movestat().y() }; }
	VectorInt GetFrontCellPos();

	int64 GetObjectID() { return _info.objectid(); }
	void SetObjectID(int64 id) { _info.set_objectid(id); }
	Protocol::MoveStat* GetMoveStat();
	Protocol::Stat* GetStat();

	Protocol::ObjectInfo& GetInfo() { return _info; }
	void SetRoom(GameRoomRef room) { _room = room; }
	void SyncPosToMap(GameObjectRef object);
	
	void BroadcastMove();
	void BroadcastChangeState();

protected:

	Protocol::ObjectInfo _info;
	GameRoomRef _room;
	static atomic<uint64> s_id;
};

