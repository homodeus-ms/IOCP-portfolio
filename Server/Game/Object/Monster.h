#pragma once
#include "GameObject.h"

class Monster : public GameObject
{
	using Super = GameObject;
public:
	Monster(VectorInt pos);
	virtual ~Monster() override;

	virtual void Update();

	enum
	{
		SNAKE_POWER = 1,
		SNAKE_SIGHT = 12,
	};

private:
	virtual void UpdateIdle();
	virtual void UpdateMove();
	virtual void UpdateSkill();

private:
	virtual void Attack(GameObjectRef target) override;

private:

	uint64 _waitUntil = 0;
	uint64 _attackInterval = 0;
	PlayerRef _target;
	PlayerRef _oldTarget = nullptr;
};

