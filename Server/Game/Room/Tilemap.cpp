#include "pch.h"
#include "Tilemap.h"
#include <fstream>

Tilemap::Tilemap()
{
	
}

Tilemap::~Tilemap()
{

}


void Tilemap::LoadFile(const wstring& path)
{
	wifstream ifs;
	ifs.open(path);

	if (!ifs.is_open())
	{
		::MessageBox(NULL, L"Open File Failed", L"", MB_OK);
		return;
	}

	ifs >> _mapSize.x >> _mapSize.y >> _tileSize;

	_tiles.clear();

	for (int y = 0; y < _mapSize.y; ++y)
	{
		wstring read;
		ifs >> read;

		_tiles.push_back(vector<Tile>(_mapSize.x));

		for (int x = 0; x < _mapSize.x; ++x)
		{
			_tiles[y][x].value = read.at(x) - L'0';
		}
	}

	ifs.close();
}

Tile* Tilemap::GetTileAt(VectorInt pos)
{
	if (pos.x < 0 || pos.x >= _mapSize.x || pos.y < 0 || pos.y >= _mapSize.y)
		return nullptr;
	return &_tiles[pos.y][pos.x];
}

void Tilemap::SetMapSize(VectorInt size)
{
	_mapSize = size;

	_tiles = vector<vector<Tile>>(size.y, vector<Tile>(size.x));

	for (int32 y = 0; y < size.y; ++y)
	{
		for (int32 x = 0; x < size.x; ++x)
		{
			_tiles[y][x] = Tile{ 0 };
		}
	}
}

