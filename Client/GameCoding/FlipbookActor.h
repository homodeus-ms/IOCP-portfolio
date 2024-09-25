#pragma once
#include "Actor.h"
#include "Flipbook.h"

class FlipbookActor : public Actor
{
	using Super = Actor;

public:
	FlipbookActor(LayerType layerType = LAYER_OBJECT) : Super(layerType) {}
	virtual ~FlipbookActor();

	virtual void BeginPlay();
	virtual void Tick();
	virtual void Render(HDC hdc);

	void SetFlipbook(Flipbook* fb);
	bool IsAnimationEnd();
	
	void Reset();


protected:
	
	Flipbook* _flipbook = nullptr;
	float _sumTime = 0.f;
	uint32 _idx = 0;
};

