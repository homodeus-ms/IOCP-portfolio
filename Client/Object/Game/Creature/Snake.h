#pragma once
#include "Monster.h"

class Flipbook;

class Snake : public Monster
{
	using Super = Monster;
public:
	Snake();
	virtual ~Snake();

	void BeginPlay() override;
	void Tick() override;
	void Render(HDC hdc) override;


private:

	void UpdateAnimation() override;
	void TickIdle();
	void TickMove();
	void TickAttack();

private:
	
	Flipbook* _flipbook[4];
	Flipbook* _hitEffect = nullptr;
	float _attackInterval = 0.5f;
	
};

