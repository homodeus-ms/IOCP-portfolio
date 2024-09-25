#pragma once
#include <cmath>
//#include <windows.h>

//using int8		= __int8;
//using int16		= __int16;
//using int32		= __int32;
//using int64		= __int64;
//using uint8		= unsigned __int8;
//using uint16	= unsigned __int16;
//using uint32	= unsigned __int32;
//using uint64	= unsigned __int64;

//using Pos = struct Vector;


struct Pos
{
	
	Pos() {}
	Pos(float x, float y) : x(x), y(y) {}
	Pos(POINT p) : x(static_cast<float>(p.x)), y(static_cast<float>(p.y)) {}

	Pos operator+(const Pos& rhs)
	{
		Pos ret;
		ret.x = x + rhs.x;
		ret.y = y + rhs.y;
		return ret;
	}
	Pos operator-(const Pos& rhs)
	{
		Pos ret;
		ret.x = x - rhs.x;
		ret.y = y - rhs.y;
		return ret;
	}
	void operator+=(const Pos& rhs)
	{
		x += rhs.x;
		y += rhs.y;
	}
	void operator-=(const Pos& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
	}
	Pos operator*(const Pos& rhs)
	{
		Pos ret;
		ret.x = x * rhs.x;
		ret.y = y * rhs.y;
		return ret;
	}
	void operator*=(const Pos& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
	}
	bool operator==(const Pos& rhs)
	{
		if ((*this - rhs).LengthSquared() < 0.0001)
			return true;

		return false;
	}
	float LengthSquared()
	{
		return x * x + y * y;
	}
	float Length()
	{
		return ::sqrt(LengthSquared());
	}
	void Normalize()
	{
		float length = Length();
		if (length < 0.00000000001f)
			return;
		x /= length;
		y /= length;
	}
	float Dot(Pos other)
	{
		return x * other.x + y * other.y;
	}
	float Cross(Pos other)
	{
		return x * other.y - y * other.x;
	}

	float x = 0;
	float y = 0;
};

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


using ServerSessionRef = shared_ptr<class ServerSession>;

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


using ObjectType = Protocol::OBJECT_TYPE;
using ObjectState = Protocol::OBJECT_STATE_TYPE;
using Stat = Protocol::Stat;
using WeaponType = Protocol::WeaponType;

