#include "pch.h"
#include "TilemapActor.h"
#include "Tilemap.h"
#include "ResourceManager.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Sprite.h"

TilemapActor::~TilemapActor()
{
	// 리소스 매니저에서 생성된 리소스들을 한꺼번에 지우고 있음
	// 여기서 _tilemap을 지우면 나중에 더블프리가 발생함
}

void TilemapActor::BeginPlay()
{
	Super::BeginPlay();
}

void TilemapActor::Tick()
{
	Super::Tick();

	return;

	if (_tilemap == nullptr)
		return;

	

	if (GET_SINGLE(InputManager)->IsKeyPressing(LClick))
	{
		Pos mousePos = GET_SINGLE(InputManager)->GetMousePos();
		Pos cameraPos = GET_SINGLE(SceneManager)->GetCameraPos();
		int32 tileSize = _tilemap->GetTileSize();

		int32 x = static_cast<int32>((mousePos.x + cameraPos.x - GWinHalfX) / tileSize);
		int32 y = static_cast<int32>((mousePos.y + cameraPos.y - GWinHalfY) / tileSize);

		if (_prevIdx.x != x || _prevIdx.y != y)
		{
			ChangeTileAt({ x, y });
			_prevIdx.x = x;
			_prevIdx.y = y;
		}
	}
	if (GET_SINGLE(InputManager)->IsKeyUp(LClick))
	{
		_prevIdx.x = -1;
		_prevIdx.y = -1;
	}
	
}

void TilemapActor::Render(HDC hdc)
{
	Super::Render(hdc);
	
	if (_tilemap == nullptr || !_showForDebug)
		return;

	Sprite* tileO = GET_SINGLE(ResourceManager)->GetSprite(L"TileO");
	Sprite* tileX = GET_SINGLE(ResourceManager)->GetSprite(L"TileX");

	const vector<vector<Tile>>& tiles = _tilemap->GetTilemap();
	const VectorInt mapSize = _tilemap->GetMapSize();
	const uint32 tileSize = _tilemap->GetTileSize();
	const Pos cameraPos = GET_SINGLE(SceneManager)->GetCameraPos();

	// 컬링

	int32 left = (int32)cameraPos.x - GWinHalfX;
	int32 right = (int32)cameraPos.x + GWinHalfX;
	int32 top = (int32)cameraPos.y - GWinHalfY;
	int32 bottom = (int32)cameraPos.y + GWinHalfY;

	int32 startX = left / tileSize;
	int32 endX = right / tileSize;
	int32 startY = top / tileSize;
	int32 endY = bottom / tileSize;

	/*int32 startX = (left - _pos.x) / 48;
	int32 endX = (right - _pos.x) / 48;
	int32 startY = (top - _pos.y) / 48;
	int32 endY = (bottom - _pos.y) / 48;*/

	int32 LimitX = _tilemap->GetMapSize().x;
	int32 LimitY = _tilemap->GetMapSize().y;
	
	for (int32 y = startY; y <= endY; ++y)
	{
		for (int32 x = startX; x <= endX; ++x)
		{
			if (x < 0 || x >= LimitX)
				continue;
			if (y < 0 || y >= LimitY)
				continue;

			int32 value = tiles[y][x].value;

			switch (value)
			{
			case 0:
				TransparentBlt(hdc,
					(x - startX) * tileSize,
					(y - startY) * tileSize,
					tileSize,
					tileSize,
					tileO->GetDc(),
					tileO->GetPos().x,
					tileO->GetPos().y,
					tileO->GetSize().x,
					tileO->GetSize().y,
					tileO->GetTransparent());
				break;
			case 1:
				TransparentBlt(hdc,
					(x - startX) * tileSize,
					(y - startY) * tileSize,
					tileSize,
					tileSize,
					tileX->GetDc(),
					tileX->GetPos().x,
					tileX->GetPos().y,
					tileX->GetSize().x,
					tileX->GetSize().y,
					tileX->GetTransparent());
				break;
			}
		}
	}
}

void TilemapActor::ChangeTileAt(VectorInt idx)
{
	if (_tilemap == nullptr)
		return;
	if (idx.x >= _tilemap->GetMapSize().x || idx.y >= _tilemap->GetMapSize().y)
		return;
	
	int32 value = _tilemap->GetTileAt(idx).value;
	_tilemap->SetTileAt(idx, value == 0 ? 1 : 0);
}
