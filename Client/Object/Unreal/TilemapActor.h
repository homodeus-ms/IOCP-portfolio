#pragma once
#include "Actor.h"

class Tilemap;



class TilemapActor : public Actor
{
	using Super = Actor;
public:
	TilemapActor(Tilemap* tilemap) : Super(LAYER_TILEMAP), _tilemap(tilemap) {}
	virtual ~TilemapActor();

	virtual void BeginPlay();
	virtual void Tick();
	virtual void Render(HDC hdc);

	
	void ChangeTileAt(VectorInt idx);
	void SetTilemap(Tilemap* tilemap) { _tilemap = tilemap; }
	Tilemap* GetTilemap() const { return _tilemap; }
	void SetShowForDebug(bool b) { _showForDebug = b; }
	void ToggleShowForDebug() { _showForDebug = !_showForDebug; }

private:
	bool _showForDebug = true;
	Tilemap* _tilemap = nullptr;

	VectorInt _prevIdx = { -1, -1 };
};

