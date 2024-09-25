#include "pch.h"
#include "GameRoom.h"
#include "Player.h"
#include "Monster.h"
#include "Projectile.h"
#include "Session.h"
#include "GameSession.h"
#include "ServerPacketHandler.h"

GameRoomRef GRoom = make_shared<GameRoom>();

GameRoom::GameRoom()
{
	
}

GameRoom::~GameRoom()
{
	// GameRoom 소멸자에 왔다는건 프로그램 종료라 안해줘도 되긴하지만..
	_monsters.clear();
	_projectiles.clear();
	_players.clear();
	_trashIds.clear();
}

void GameRoom::Init()
{
	//_tilemap.LoadFile(L"C:\\Users\\seekc\\Desktop\\Server\\Client\\Resources\\Tilemap\\Tilemap_01.txt");
	_tilemap.LoadFile(L"C:\\kiwi\\GameCoding\\Server\\Client\\Resources\\Tilemap\\Tilemap_City.txt");

	int32 row = _tilemap.GetMapSize().y;
	int32 col = _tilemap.GetMapSize().x;

	_objectMap.resize(row);
	for (int32 y = 0; y < row; ++y)
	{
		_objectMap[y].resize(col);
	}

	//TestSpawnMonsters();

	SpawnMonsters();
	

	_trashIds.reserve(100);
}

void GameRoom::Update()
{
	for (auto& item : _players)
	{
		item.second->Update();
	}
	for (auto& item : _monsters)
	{
		item.second->Update();
	}
	for (auto& item : _projectiles)
	{
		item.second->Update();
	}

	RemoveTrash();
	
}

void GameRoom::EnterRoom(GameSessionRef session)
{
	PlayerRef player = GameObject::CreatePlayer();

	session->_gameRoom = GetRoomRef();
	session->_player = player;
	player->SetSession(session);

	// TEMP 위치 지정

	// 입장한 클라에게 그 클라의 플레이어정보를(여기서 만들어서) 보내준다
	// "너의 현재 PlayerInfo는 이런상태야"
	{
		Protocol::S_MyPlayer pkt;
		*pkt.mutable_info() = player->GetInfo();
		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		session->Send(sendBuffer);
	}
	// 입장한 클라에게 현재 룸에 존재하는 모든 오브젝트의 정보를 전송해준다
	{
		Protocol::S_AddObject pkt;

		for (auto& item : _players)
		{
			Protocol::ObjectInfo* info = pkt.add_objects();
			*info = item.second->GetInfo();
		}

		for (auto& item : _monsters)
		{
			Protocol::ObjectInfo* info = pkt.add_objects();
			*info = item.second->GetInfo();
		}


		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		session->Send(sendBuffer);
	}

	AddObject(player);
}

void GameRoom::LeaveRoom(GameSessionRef session)
{
	if (session == nullptr)
		return;
	if (session->GetPlayerRef() == nullptr)
		return;
	uint64 id = session->GetPlayerRef()->GetObjectID();
	RemoveObject(id);
}

GameObjectRef GameRoom::FindObject(uint64 id)
{
	int32 flag = id >> 32;
	ObjectType type = static_cast<ObjectType>(flag);

	switch (type)
	{
	case ObjectType::OBJECT_TYPE_PLAYER:
	{
		auto findIt = _players.find(id);
		if (findIt != _players.end())
			return findIt->second;
	}
	break;
	case ObjectType::OBJECT_TYPE_MONSTER:
	{
		auto findIt = _monsters.find(id);
		if (findIt != _monsters.end())
			return findIt->second;
	}
	break;
	case ObjectType::OBJECT_TYPE_PROJECTILE:
	{
		auto findIt = _projectiles.find(id);
		if (findIt != _projectiles.end())
			return findIt->second;
	}
	break;
	}


	return nullptr;
}

void GameRoom::ChangeState(Protocol::ObjectInfo info)
{
	uint64 id = info.objectid();
	PlayerRef player = _players[id];
	ASSERT_CRASH(player != nullptr);


	ObjectInfo& playerInfo = player->GetInfo();
	playerInfo.mutable_movestat()->set_dir(info.movestat().dir());
	playerInfo.mutable_movestat()->set_state(info.movestat().state());
	playerInfo.set_weapontype(info.weapontype());

	 

	Protocol::S_ChangeState pkt;
	*pkt.mutable_info() = info;

	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	
	Broadcast(sendBuffer);
}

void GameRoom::TryMove(Protocol::C_TryMove pkt)
{
	uint64 id = pkt.id();
	VectorInt from(pkt.fromx(), pkt.fromy());
	VectorInt dest(pkt.destx(), pkt.desty());

	PlayerRef player = _players[id];
	ASSERT_CRASH(player != nullptr);
	VectorInt currPos = player->GetCellPos();

	if (from != currPos)
	{
		cout << "from != currPos" << endl;
		cout << "From: " << from << " -> " << "Dest: " << dest << endl;
		cout << "Curr" << currPos << endl;
	}

	if (CanGo(dest, id) == false)
	{
		cout << "Log Can't Go" << endl;

		// 클라이언트의 움직임을 취소시켜야함
		Protocol::S_Move pkt;
		Protocol::MoveStat* movestat = pkt.mutable_info()->mutable_movestat();
		pkt.mutable_info()->set_objectid(id);
		movestat->set_state(MOVE);
		movestat->set_x(from.x);
		movestat->set_y(from.y);
		pkt.set_modified(true);
		
		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		player->GetSession()->Send(sendBuffer);

		return;
		//ASSERT_CRASH(false);
	}

	if (GetObjectInMap(currPos.x, currPos.y) != player)
	{
		cout << "ObjectMap Error!" << endl;
		VectorInt whereIs = GetObjectPosFromMap(id);
		cout << "Curr: " << currPos << " , " << "WhereIs: " << whereIs << endl;
		if (whereIs != dest)
			MoveObjectInObjectMap(whereIs, dest);
	}

	ObjectInfo& info = player->GetInfo();

	if (CanGo(dest, id))
	{
		MoveObject(info, dest);

		Protocol::S_Move pkt;
		info.mutable_movestat()->set_dir(CalculateDir(from, dest));
		*pkt.mutable_info() = info;

		//cout << "Move Complete: " << from << " -> " << dest << endl;
		//cout << "Dir: " << info.movestat().dir() << endl;

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		Broadcast(sendBuffer);
	}
	else
	{
		cout << "Can't Go Log" << endl;
		cout << from << " -> " << dest << endl;
		ASSERT_CRASH(false);
		
	}
}

void GameRoom::TryAttack(uint64 attackerId, int32 targetX, int32 targetY)
{
	GameObjectRef attacker = FindObject(attackerId);

	ASSERT_CRASH(attacker != nullptr);

	GameObjectRef target = GetObjectInMap(targetX, targetY);

	if (attacker == target)
	{
		cout << "Attacker == Target" << endl;
		return;
	}
		

	attacker->Attack(target);
}

void GameRoom::AddObject(GameObjectRef object)
{
	uint64 id = object->GetInfo().objectid();
	auto objectType = object->GetObjectType();
	VectorInt pos = object->GetCellPos();

	if (objectType != ObjectType::OBJECT_TYPE_PROJECTILE)
		InsertObjectToObjectMap(object, pos);

	switch (objectType)
	{
	case ObjectType::OBJECT_TYPE_PLAYER:
		_players[id] = static_pointer_cast<Player>(object);
		break;
	case ObjectType::OBJECT_TYPE_MONSTER:
		_monsters[id] = static_pointer_cast<Monster>(object);
		break;
	case ObjectType::OBJECT_TYPE_PROJECTILE:
		_projectiles[id] = static_pointer_cast<Projectile>(object);
		break;
	default:
		return;
	}
	
	object->SetRoom(GetRoomRef());

	// 신규 오브젝트의 정보를 전송
	{
		Protocol::S_AddObject pkt;
		
		Protocol::ObjectInfo* info = pkt.add_objects();
		*info = object->GetInfo();
		
		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		Broadcast(sendBuffer);
	}
}

void GameRoom::RemoveObject(uint64 id)
{
	GameObjectRef gameObject = FindObject(id);
	if (gameObject == nullptr)
		return;
	
	_trashIds.push_back(id);
	if (gameObject->GetObjectType() == ObjectType::OBJECT_TYPE_PLAYER)
		_clearTrashImmediately = true;

	/*cout << "Trash count : " << _trashIds.size() << endl;
	cout << "Trash id : " << id << endl;*/

	//gameObject->SetRoom(nullptr);

	// 오브젝트 Remove사실을 전송
	{
		Protocol::S_RemoveObject pkt;
		pkt.add_ids(id);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		Broadcast(sendBuffer);
	}

	if (gameObject->GetObjectType() != ObjectType::OBJECT_TYPE_PROJECTILE)
		RemoveObjectFromObjectMap(gameObject->GetCellPos());
}

void GameRoom::RemoveTrash()
{
	if (_trashIds.size() < 30 && !_clearTrashImmediately)
		return;

	for (uint64 id : _trashIds)
	{
		GameObjectRef gameObject = FindObject(id);

		if (gameObject == nullptr)
			continue;

		switch (gameObject->GetObjectType())
		{
		case ObjectType::OBJECT_TYPE_PLAYER:
			_players.erase(id);
			break;
		case ObjectType::OBJECT_TYPE_MONSTER:
			_monsters.erase(id);
			break;
		case ObjectType::OBJECT_TYPE_PROJECTILE:
			_projectiles.erase(id);
			break;
		default:
			return;
		}
	}

	_trashIds.clear();
	_clearTrashImmediately = false;
}

void GameRoom::Chat(Protocol::C_Chat pkt)
{
	Protocol::S_Chat sendPkt;
	sendPkt.set_message(pkt.message());

	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(sendPkt);
	Broadcast(sendBuffer);
}

void GameRoom::Broadcast(SendBufferRef& sendBuffer)
{
	
	for (auto& item : _players)
	{
		ASSERT_CRASH(item.second->GetSession() != nullptr);
		item.second->GetSession()->Send(sendBuffer);
	}
}

void GameRoom::MoveObjectInObjectMap(VectorInt from, VectorInt to)
{
	GameObjectRef gameObject = _objectMap[from.y][from.x].lock();
	if (gameObject == nullptr)
		return;
	ASSERT_CRASH(gameObject->GetObjectType() != ObjectType::OBJECT_TYPE_PROJECTILE);

	_objectMap[to.y][to.x] = gameObject;
	_objectMap[from.y][from.x].reset();
}

void GameRoom::RemoveObjectFromObjectMap(VectorInt pos)
{
	_objectMap[pos.y][pos.x].reset();
}

void GameRoom::MoveObject(Protocol::ObjectInfo& info, VectorInt dest)
{
	ASSERT_CRASH(CanGo(dest, info.objectid()));

	Protocol::MoveStat* moveStat = info.mutable_movestat();
	moveStat->set_state(MOVE);
	VectorInt from{ moveStat->x(), moveStat->y() };
	
	MoveObjectInObjectMap(from, dest);

	moveStat->set_x(dest.x);
	moveStat->set_y(dest.y);
}

VectorInt GameRoom::GetObjectPosFromMap(uint64 id)
{
	int32 row = _tilemap.GetMapSize().y;
	int32 col = _tilemap.GetMapSize().x;

	for (int32 y = 0; y < row; ++y)
	{
		for (int32 x = 0; x < col; ++x)
		{
			if (_objectMap[y][x].lock() == nullptr)
				continue;

			GameObjectRef object = _objectMap[y][x].lock();
			if (object->GetObjectID() == id)
			{
				return VectorInt(x, y);
			}
		}
	}
	return VectorInt(1, 1);
}

Dir GameRoom::CalculateDir(VectorInt pos, VectorInt dest)
{
	ASSERT_CRASH(pos != dest);

	VectorInt dist = pos - dest;
	Dir ret = Dir::DIR_TYPE_DOWN;
	
	if (dist.x == 0)
	{
		ret = dist.y > 0 ? DIR_UP : DIR_DOWN;
	}
	else
	{
		ret = dist.x > 0 ? DIR_LEFT : DIR_RIGHT;
	}

	return ret;
}

bool GameRoom::CanGo(VectorInt dest, uint64 id)
{
	Tile* tile = _tilemap.GetTileAt(dest);
	if (!tile || tile->value != 0)
		return false;

	GameObjectRef object = _objectMap[dest.y][dest.x].lock();
	if (object == nullptr)
		return true;

	if (object->GetObjectID() == id)
		return true;

	return false;
}

ObjectType GameRoom::GetObjectTypeInMap(int32 x, int32 y)
{
	GameObjectRef object = _objectMap[y][x].lock();
	if (object == nullptr)
		return ObjectType::OBJECT_TYPE_NONE;

	return object->GetObjectType();
}

GameObjectRef GameRoom::GetObjectInMap(int32 x, int32 y)
{
	if (x < 0 || x >= _tilemap.GetMapSize().x || y < 0 || y >= _tilemap.GetMapSize().y)
		return nullptr;

	return _objectMap[y][x].lock();
}

PlayerRef GameRoom::FindClosestPlayer(VectorInt from, int32 limitDist)
{
	if (_players.empty())
		return nullptr;

	int32 minDist = INT_MAX;
	PlayerRef minDistPlayer = nullptr;

	for (auto& pair : _players)
	{
		PlayerRef& player = pair.second;
		const VectorInt& targetPos = player->GetCellPos();
		int32 dist = abs(targetPos.x - from.x) + abs(targetPos.y - from.y);
		
		if (dist >= limitDist)
			continue;

		if (dist < minDist)
		{
			minDist = dist;
			minDistPlayer = player;
		}
	}
	return minDist == INT_MAX ? nullptr : std::move(minDistPlayer);
}

bool GameRoom::FindPath(VectorInt src, VectorInt dest, OUT list<VectorInt>& path, int32 limitDist)
{
	if (IsTooFar(src, dest, limitDist))
		return false;

	unordered_map<VectorInt, int32> bests;
	unordered_map<VectorInt, VectorInt> prevs;
	priority_queue<Node, vector<Node>, greater<Node>> pq;

	int32 maxDist = dest.ManhattanDist(src);
	Node node(src, maxDist);
	bests[src] = maxDist;
	prevs[src] = src;
	pq.push(node);
	bool bFound = false;

	const VectorInt deltaXY[4] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

	while (!pq.empty())
	{
		node = pq.top();
		pq.pop();
		VectorInt pos = node._pos;

		int32 minDist = bests[pos];
		int32 dist = node._dist;
		
		if (minDist < dist)
			continue;

		if (pos == dest)
		{
			bFound = true;
			break;
		}

		for (int i = 0; i < 4; ++i)
		{
			VectorInt nextPos = pos + deltaXY[i];

			if (!CanGo(nextPos))
				continue;
			if (IsTooFar(src, nextPos, limitDist))
				continue;

			minDist = bests[nextPos];    // nextPos가 없었으면 pair(nextPos,0) 이 생김
			dist = nextPos.ManhattanDist(dest);

			if (minDist != 0)    // bests에 key:nextPos가 있었으면
			{
				if (minDist <= dist)
					continue;
			}

			bests[nextPos] = dist;
			prevs[nextPos] = pos;
			pq.push(Node(nextPos, dist));
		}
	}

	// 만약 dest까지 가는 경로를 못찾았다면 최대한 가까운쪽으로 한 칸 이동하게끔
	if (!bFound)
	{
		int32 minDist = INT_MAX;

		for (auto& item : bests)
		{
			VectorInt pos = item.first;
			int32 dist = item.second;

			if (minDist == dist)
			{
				int32 dist1 = dest.ManhattanDist(src);
				int32 dist2 = pos.ManhattanDist(src);
				if (dist1 > dist2)
					dest = pos;
			}
			else if (minDist > dist)
			{
				minDist = dist;
				dest = pos;
			}
		}
	}

	path.clear();

	VectorInt pos = dest;

	while (pos != src)
	{
		path.push_front(pos);
		pos = prevs[pos];
	}
	path.push_front(src);

	return true;
}

bool GameRoom::IsTooFar(VectorInt& src, VectorInt& dest, int32 limitDist)
{
	int32 dist = dest.ManhattanDist(src);
	return dist >= limitDist;
}

void GameRoom::SpawnMonsters()
{
	const VectorInt mapSize = _tilemap.GetMapSize();
	//int32 count = 0;
	for (; _currSnakeCount < MAX_SNAKE_COUNT; ++_currSnakeCount)
	{
		VectorInt pos = GetRandomPos(mapSize.x, mapSize.y);
		MonsterRef monster = GameObject::CreateMonster(pos);

		//cout << "Spawn Monster: " << ++count << endl;
		AddObject(monster);
	}
}

void GameRoom::TestSpawnMonsters()
{
	{
		VectorInt pos(4, 4);
		MonsterRef monster = GameObject::CreateMonster(pos);
		AddObject(monster);
	}
	{
		VectorInt pos(8, 4);
		MonsterRef monster = GameObject::CreateMonster(pos);
		AddObject(monster);
	}
	{
		VectorInt pos(5, 8);
		MonsterRef monster = GameObject::CreateMonster(pos);
		AddObject(monster);
	}
}

void GameRoom::AttackTarget(const GameObjectRef& attacker, GameObjectRef& target)
{
	Protocol::Stat* stat = target->GetStat();
	int32 attack = attacker->GetStat()->attack();

	if (attacker->GetObjectType() == ObjectType::OBJECT_TYPE_PLAYER)
	{
		PlayerRef player = static_pointer_cast<Player>(attacker);
		if (player->GetWeaponType() == WeaponType::WEAPON_TYPE_BOW)
		{
			attack = BOW_POWER;
		}
	}
	int32 damage = attack - stat->defence();
	stat->set_hp(max(0, stat->hp() - damage));
}

void GameRoom::OnDeath(GameObjectRef gameObject)
{
	if (gameObject->GetObjectType() == ObjectType::OBJECT_TYPE_MONSTER)
	{
		
		if (--_currSnakeCount <= MAX_SNAKE_COUNT / 3)
		{
			SpawnMonsters();
		}
	}

	
	uint64 objectId = gameObject->GetObjectID();
	
	RemoveObject(objectId);
}

VectorInt GameRoom::GetRandomPos(int32 mapX, int32 mapY)
{
	std::srand(static_cast<unsigned int>(std::time(0)));

	const int32 sx = mapX - 10;
	const int32 sy = mapY - 10;

	for (int32 i = 0; i < 1000; ++i)
	{
		int32 x = 10 + rand() % sx;
		int32 y = 10 + rand() % sy;
		VectorInt pos(x, y);

		if (CanGo(pos))
			return pos;
	}

	ASSERT_CRASH(false);

	return VectorInt(0, 0);
}
