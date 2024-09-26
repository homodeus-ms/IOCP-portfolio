#pragma once
#include "GameObject.h"


class Projectile : public GameObject
{
	using Super = GameObject;
public:
	Projectile();
	virtual ~Projectile();

	virtual void BeginPlay();
	virtual void Tick();
	virtual void Render(HDC hdc);

	

protected:
	VectorInt _cellPos;
};

