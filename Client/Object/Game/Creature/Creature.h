#pragma once
#include "GameObject.h"

/*
enum class CreatureType
{
	Player,
	Monster,
};

struct Stat
{
	int32 hp;
	int32 maxHp;
	int32 attack;
	int32 defence;
	float speed;
};
*/

class Creature : public GameObject
{
	using Super = GameObject;
public:
	//Creature() {}
	Creature() : Super(CobjectType::Creature) {};
	virtual ~Creature();

	virtual void BeginPlay();
	virtual void Tick();
	virtual void Render(HDC hdc);

	virtual void TickIdle();
	virtual void TickMove();
	virtual void TickAttack();

	//virtual void OnDamaged(Creature* attacker);
	//virtual void OnDamaged(int32 attack);
	//virtual void ProcessDeath();
	

protected:

	//CreatureType _type;
	//Stat _stat = {};
};

