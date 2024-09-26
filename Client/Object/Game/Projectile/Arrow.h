#pragma once
#include "Projectile.h"

class Player;

class Arrow : public Projectile
{
	using Super = Projectile;
public:
	Arrow();
	virtual ~Arrow();

	virtual void BeginPlay();
	virtual void Tick();
	virtual void Render(HDC hdc);

	virtual void UpdateAnimation() override;

	void SetOwner(Player* player);

private:
	virtual void TickIdle();
	virtual void TickMove();
	

private:
	const int32 ARROW_SPEED = 400;
	const int32 ARROW_ATTACK = 20;
	VectorInt _startPos = {};

	Flipbook* _flipbooks[4];
	Player* _owner = nullptr;
	bool _dirSet = false;
};

