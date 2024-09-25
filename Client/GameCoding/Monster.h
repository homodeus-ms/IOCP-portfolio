#pragma once
#include "Creature.h"

class Monster : public Creature
{
	using Super = Creature;

public:
	Monster();
	virtual ~Monster();

	void BeginPlay() override;
	void Tick() override;
	void Render(HDC hdc) override;

private:

	void UpdateAnimation() override;
	void TickIdle();
	void TickMove();
	void TickAttack();

private:

};

