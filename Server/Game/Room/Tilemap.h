#pragma once

struct Tile
{
	int32 value = 0;
};

class Tilemap
{
public:
	Tilemap();
	virtual ~Tilemap();

	virtual void LoadFile(const wstring& path);

	VectorInt GetMapSize() { return _mapSize; }
	int32 GetTileSize() const { return _tileSize; }
	Tile* GetTileAt(VectorInt pos);
	vector<vector<Tile>>& GetTiles() { return _tiles; }

	void SetMapSize(VectorInt size);
	void SetTileSize(int32 size) { _tileSize = size; }

private:
	VectorInt _mapSize = {};
	int32 _tileSize = 0;
	vector<vector<Tile>> _tiles;
};

