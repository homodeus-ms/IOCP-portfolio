#pragma once
#include "Tilemap.h"
#include "JobQueue.h"
#include "Monster.h"

struct Node
{
	Node(VectorInt pos, int32 dist) : _pos(pos), _dist(dist) {}
	bool operator<(const Node& other) const { return _dist < other._dist; }
	bool operator>(const Node& other) const { return _dist > other._dist; }
	VectorInt _pos;
	int32 _dist;
};

class GameRoom : public JobQueue
{
public:
	GameRoom();
	virtual ~GameRoom();

	void Init();
	void Update();

	void EnterRoom(GameSessionRef session);
	void LeaveRoom(GameSessionRef session);
	GameObjectRef FindObject(uint64 id);
	GameRoomRef GetRoomRef() { return static_pointer_cast<GameRoom>(shared_from_this()); }

	// 받은 패킷 처리 함수들
	
	void ChangeState(Protocol::ObjectInfo info);
	void TryMove(Protocol::C_TryMove pkt);
	void TryAttack(uint64 attackerId, int32 targetX, int32 targetY);
	void AddObject(GameObjectRef object);
	void RemoveObject(uint64 id);
	void RemoveTrash();
	void Chat(Protocol::C_Chat pkt);

	void Broadcast(SendBufferRef& sendBuffer);

	template<typename T>
	void InsertObjectToObjectMap(T object, VectorInt pos);
	void MoveObjectInObjectMap(VectorInt from, VectorInt to);
	void RemoveObjectFromObjectMap(VectorInt pos);
	void MoveObject(Protocol::ObjectInfo& info, VectorInt dest);
	
	
	Dir CalculateDir(VectorInt pos, VectorInt dest);
	bool CanGo(VectorInt dest, uint64 id = 0);

	// ObjectMap 관련
	ObjectType GetObjectTypeInMap(int32 x, int32 y);
	GameObjectRef GetObjectInMap(int32 x, int32 y);
	// temp, for debuf
	VectorInt GetObjectPosFromMap(uint64 id);

	// Monster 관련
	PlayerRef FindClosestPlayer(VectorInt from, int32 limitDist = Monster::SNAKE_SIGHT);
	bool FindPath(VectorInt src, VectorInt dest, OUT list<VectorInt>& path, int32 limitDist = Monster::SNAKE_SIGHT);
	bool IsTooFar(VectorInt& src, VectorInt& dest, int32 limitDist);
	void SpawnMonsters();
	void TestSpawnMonsters();

	// Attack 관련
	void AttackTarget(const GameObjectRef& attacker, GameObjectRef& target);
	void OnDeath(GameObjectRef gameObject);

private:
	VectorInt GetRandomPos(int32 mapX, int32 mapY);

private:

	enum 
	{ 
		MAX_SNAKE_COUNT = 50,
	};

	unordered_map<uint64, PlayerRef> _players;
	unordered_map<uint64, MonsterRef> _monsters;
	unordered_map<uint64, ProjectileRef> _projectiles;

	vector<uint64> _trashIds;

	Tilemap _tilemap = {};
	
	vector<vector<weak_ptr<GameObject>>> _objectMap;

	int32 _currSnakeCount = 0;
	bool _clearTrashImmediately = false;

};

extern GameRoomRef GRoom;

template<typename T>
inline void GameRoom::InsertObjectToObjectMap(T object, VectorInt pos)
{
	auto ret = std::is_convertible_v<T, GameObjectRef>;
	ASSERT_CRASH(ret);

	_objectMap[pos.y][pos.x] = object;
}
