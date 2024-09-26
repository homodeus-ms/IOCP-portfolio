#include "pch.h"
#include "Tilemap.h"

Tilemap::Tilemap(VectorInt mapSize, uint32 tileSize)
	: _mapSize(mapSize), _tileSize(tileSize)
{
	// mapSize (63, 43) TileSize (48)
	_tiles.reserve(_mapSize.y);

	for (int y = 0; y < _mapSize.y; ++y)
	{
		vector<Tile> innerVector;
		innerVector.reserve(_mapSize.x);
		_tiles.push_back(innerVector);

		for (int x = 0; x < _mapSize.x; ++x)
		{
			_tiles[y].push_back({ 0 });
		}
	}
}

Tilemap::~Tilemap()
{
	
}

void Tilemap::BeginPlay()
{
}

void Tilemap::Tick()
{

}

void Tilemap::Render(HDC hdc)
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

void Tilemap::SaveFile(const wstring& path)
{
	wofstream wfs;
	wfs.open(path);
	if (!wfs.is_open())
	{
		::MessageBox(NULL, L"Open File Failed", L"", MB_OK);
		return;
	}

	wfs << _mapSize.x << " " << _mapSize.y << " " << _tileSize << endl;

	for (int y = 0; y < _mapSize.y; ++y)
	{
		for (int x = 0; x < _mapSize.x; ++x)
		{
			int value = _tiles[y][x].value;
			wfs << value;
		}
		wfs << endl;
	}

	wfs.close();
}

void Tilemap::SetNewTilemap(VectorInt mapSize, uint32 tileSize)
{
	_mapSize = mapSize;
	_tileSize = tileSize;
	_tiles.clear();

	_tiles.reserve(_mapSize.y);
	// mapSize (63, 43) TileSize (48)
	for (int y = 0; y < _mapSize.y; ++y)
	{
		vector<Tile> innerVector;
		innerVector.reserve(_mapSize.x);
		_tiles.push_back(innerVector);

		for (int x = 0; x < _mapSize.x; ++x)
		{
			_tiles[y][x].value = 0;
		}
	}
}
