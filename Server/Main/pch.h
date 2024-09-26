#pragma once

#include "CorePch.h"
#include "Enum.pb.h"
#include "Protocol.pb.h"
#include "Struct.pb.h"

#ifdef _DEBUG
#pragma comment(lib, "ServerCore\\Debug\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "ServerCore\\Release\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib")
#endif

struct VectorInt
{
	VectorInt() {}
	VectorInt(int x, int y) : x(x), y(y) {}
	VectorInt(POINT p) : x(p.x), y(p.y) {}

	VectorInt operator+(const VectorInt& rhs)
	{
		VectorInt ret;
		ret.x = x + rhs.x;
		ret.y = y + rhs.y;
		return ret;
	}
	VectorInt operator-(const VectorInt& rhs)
	{
		VectorInt ret;
		ret.x = x - rhs.x;
		ret.y = y - rhs.y;
		return ret;
	}
	void operator+=(const VectorInt& rhs)
	{
		x += rhs.x;
		y += rhs.y;
	}
	void operator-=(const VectorInt& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
	}
	VectorInt operator*(const VectorInt& rhs)
	{
		VectorInt ret;
		ret.x = x * rhs.x;
		ret.y = y * rhs.y;
		return ret;
	}
	void operator*=(const VectorInt& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
	}

	bool operator==(const VectorInt& rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}
	bool operator!=(const VectorInt& rhs) const
	{
		return x != rhs.x || y != rhs.y;
	}
	bool operator<(const VectorInt& rhs) const
	{
		if (x == rhs.x)
		{
			return y < rhs.y;
		}
		return x < rhs.x;
	}

	int32 LengthSquared()
	{
		return x * x + y * y;
	}
	float Length()
	{
		return static_cast<float>(::sqrt(LengthSquared()));
	}
	int32 ManhattanDist(const VectorInt& other)
	{
		return abs(other.x - x) + abs(other.y - y);
	}

	int32 Dot(VectorInt other)
	{
		return x * other.x + y * other.y;
	}
	int32 Cross(VectorInt other)
	{
		return x * other.y - y * other.x;
	}

	friend ostream& operator<<(ostream& os, const VectorInt& v);

	int32 x = 0;
	int32 y = 0;
};



namespace std
{
	template<>
	struct hash<VectorInt>
	{
		size_t operator()(const VectorInt& v) const
		{
			return hash<int32>()(v.x << 16 | v.y);
		}
	};
}

using GameSessionRef = shared_ptr<class GameSession>;
using GameObjectRef = shared_ptr<class GameObject>;
using CreatureRef = shared_ptr<class Creature>;
using PlayerRef = shared_ptr<class Player>;
using ProjectileRef = shared_ptr<class Projectile>;
using MonsterRef = shared_ptr<class Monster>;
using GameRoomRef = shared_ptr<class GameRoom>;


#include "Enum.pb.h"
#include "Struct.pb.h"

using Dir = Protocol::DIR_TYPE;

#define DIR_DOWN	Protocol::DIR_TYPE_DOWN
#define DIR_UP		Protocol::DIR_TYPE_UP
#define DIR_LEFT	Protocol::DIR_TYPE_LEFT
#define DIR_RIGHT	Protocol::DIR_TYPE_RIGHT

#define IDLE (Protocol::OBJECT_STATE_TYPE_IDLE)
#define MOVE (Protocol::OBJECT_STATE_TYPE_MOVE)
#define SKILL (Protocol::OBJECT_STATE_TYPE_SKILL)
#define CHAT (Protocol::OBJECT_STATE_TYPE_CHAT)

#define PLAYER (Protocol::OBJECT_TYPE_PLAYER)
#define MONSTER (Protocol::OBJECT_TYPE_MONSTER)

#define SWORD (Protocol::WEAPON_TYPE_SWORD)
#define BOW (Protocol::WEAPON_TYPE_BOW)

const VectorInt DELTA_XY[4] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

using ObjectInfo = Protocol::ObjectInfo;
using ObjectType = Protocol::OBJECT_TYPE;
using ObjectState = Protocol::OBJECT_STATE_TYPE;
using Stat = Protocol::Stat;
using WeaponType = Protocol::WeaponType;
using MoveStat = Protocol::MoveStat;


