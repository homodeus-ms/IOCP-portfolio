#pragma once
#include "GameObject.h"

class Effect : public GameObject
{
	using Super = GameObject;
public:
	Effect();
	virtual ~Effect();

	virtual void BeginPlay();
	virtual void Tick();
	virtual void Render(HDC hdc);

	void TurnOn() { _isOn = true; }

private:
	bool _isOn = false;
};

