#pragma once
#include "Actor.h"

class Sprite;


class SpriteActor : public Actor
{
	using Super = Actor;
public:
	SpriteActor(LayerType layerType) : Super(layerType) {}
	virtual ~SpriteActor();

	void BeginPlay() override;
	void Tick() override;
	void Render(HDC hdc) override;

	void SetSprite(Sprite* sprite) { _sprite = sprite; }
	Sprite* GetSprite() const { return _sprite; }

private:
	

private:
	Sprite* _sprite = nullptr;

};

