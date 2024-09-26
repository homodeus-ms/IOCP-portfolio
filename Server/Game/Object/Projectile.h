#pragma once
#include "GameObject.h"

class Player;

class Projectile : public GameObject
{
public:
	Projectile(PlayerRef owner);
	virtual ~Projectile();

	virtual void Update() override;

	virtual void Attack(GameObjectRef target) override;
	uint64 GetOwnerID();

private:
	virtual void UpdateIdle();
	virtual void UpdateMove();
	virtual void UpdateSkill();

private:
	enum 
	{ 
		ARROW_SPEED = 400,
		ARROW_MAX_DIST = 10,
	};
	weak_ptr<Player> _owner;
	uint64 _startWait = 0;
	uint64 _waitUntil = 0;
	VectorInt _originPos;
	bool _isAlive = true;
};

