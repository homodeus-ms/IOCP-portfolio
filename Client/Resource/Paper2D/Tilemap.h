#pragma once
#include "ResourceBase.h"

class Sprite;

struct Tile
{
	int32 value = 0;
};

//enum TILE_SIZE
//{
//	TILE_WIDTH = 63,
//	TILE_HEIGHT = 43,
//	TILE_SIZEX = 48,
//	TILE_SIZEY = 48,
//};

class Tilemap : public ResourceBase
{
public:
	
	Tilemap(VectorInt mapSize, uint32 tileSize);
	virtual ~Tilemap();

	void BeginPlay();
	void Tick();
	void Render(HDC hdc);

	void LoadFile(const wstring& path);
	void SaveFile(const wstring& path);

	void SetNewTilemap(VectorInt mapSize, uint32 tileSize);
	vector<vector<Tile>>& GetTilemap() { return _tiles; }
	Tile GetTileAt(VectorInt idx) { return _tiles[idx.y][idx.x]; }
	void SetTileAt(VectorInt idx, int32 value) { _tiles[idx.y][idx.x].value = value; }

	VectorInt GetMapSize() const { return _mapSize; }
	void SetMapSize(VectorInt size) { _mapSize = size; }

	uint32 GetTileSize() const { return _tileSize; }
	void SetTileSize(uint32 size) { _tileSize = size; }

private:
	VectorInt _mapSize = {};
	uint32 _tileSize = 0;
	vector<vector<Tile>> _tiles;
};

