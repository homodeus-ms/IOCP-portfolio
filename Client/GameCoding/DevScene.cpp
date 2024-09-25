#include "pch.h"
#include "DevScene.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "CollisionManager.h"
#include "InputManager.h"
#include "Texture.h"
#include "Sprite.h"
#include "Flipbook.h"
#include "FlipbookActor.h"
#include "SpriteActor.h"
#include "Player.h"
#include "Collider.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "SoundManager.h"
#include "Sound.h"
#include "UI.h"
#include "Button.h"
#include "Panel.h"
#include "TestPanel.h"
#include "Tilemap.h"
#include "TilemapActor.h"
#include "GameObject.h"
#include "Creature.h"
#include "Snake.h"
#include "Effect.h"
#include "Arrow.h"
#include "ChatBox.h"
#include "UiManager.h"

// temp : Chat을 WinProc을 통해서 구현하기 위해
#include "Game.h"
#include "GameCoding.h"


int32 GMapSizeX = GWinSizeX;
int32 GMapSizeY = GWinSizeY;

DevScene::DevScene(HWND hwnd)
	: Scene(SceneType::DevScene)
	, _hwnd(hwnd)
{
	_resourceManager = GET_SINGLE(ResourceManager);
	ASSERT_CRASH(_resourceManager != nullptr);

	_players.reserve(100);
}

DevScene::~DevScene()
{	
	
}

void DevScene::Init()
{
	LoadMap();
	LoadTilemap();
	LoadPlayer();
	LoadMonster();
	LoadProjectile();
	LoadEffects();
	LoadUI();
	LoadSound(true);

	// ObjectIdMap

	ASSERT_CRASH(_mapSize.x != 0 && _mapSize.y != 0);

	vector<vector<Tile>> tiles = _tilemap->GetTilemap();

	_objectIdMap.resize(_mapSize.y);

	for (int32 y = 0; y < _mapSize.y; ++y)
	{
		_objectIdMap.resize(_mapSize.y);

		for (int32 x = 0; x < _mapSize.x; ++x)
		{
			_objectIdMap[y].push_back(static_cast<uint64>(tiles[y][x].value));
		}
	}

	Super::Init();

}

void DevScene::Update()
{
	Super::Update();

	
	// TileMap 수정하는 부분
	{
		// Stage01 의 타일맵
#if 0
		if (GET_SINGLE(InputManager)->IsKeyDown(Q))
		{
			GET_SINGLE(ResourceManager)->SaveTilemap(L"tilemap01", L"Tilemap\\Tilemap_01.txt");
		}
		if (GET_SINGLE(InputManager)->IsKeyDown(E))
		{
			GET_SINGLE(ResourceManager)->LoadTilemap(L"tilemap01", L"Tilemap\\Tilemap_01.txt");
		}
		if (GET_SINGLE(InputManager)->IsKeyDown(V))
		{
			_tilemapActor->ToggleShowForDebug();
		}
#endif
		// CityMap의 타일맵
#if 0
		if (GET_SINGLE(InputManager)->IsKeyDown(Q))
		{
			GET_SINGLE(ResourceManager)->SaveTilemap(L"CityMap_01", L"Tilemap\\TileMap_City.txt");
		}
		if (GET_SINGLE(InputManager)->IsKeyDown(E))
		{
			GET_SINGLE(ResourceManager)->LoadTilemap(L"CityMap_01", L"Tilemap\\TileMap_City.txt");
		}
		if (GET_SINGLE(InputManager)->IsKeyDown(V))
		{
			_tilemapActor->ToggleShowForDebug();
		}
#endif
	}
}

void DevScene::Render(HDC hdc)
{
	Super::Render(hdc);
}

void DevScene::LoadMap()
{
	// BackGround
	_resourceManager->LoadTexture(L"City_Back1", L"Sprite\\Map\\City_Back.bmp");
	_resourceManager->LoadTexture(L"Stage01", L"Sprite\\Map\\Stage01.bmp");

	_resourceManager->CreateSprite(L"Stage01", _resourceManager->GetTexture(L"Stage01"));
	_resourceManager->CreateSprite(L"City_Back1", _resourceManager->GetTexture(L"City_Back1"));

	// ForeGround
	_resourceManager->LoadTexture(L"City_Fore1", L"Sprite\\Map\\City_Fore.bmp", RGB(0, 0, 0));
	_resourceManager->CreateSprite(L"City_Fore1", _resourceManager->GetTexture(L"City_Fore1"));

#if 0
	{
		Sprite* sprite = _resourceManager->GetSprite(L"Stage01");
		SpriteActor* backGround = new SpriteActor(LAYER_BACKGROUND);
		backGround->SetSprite(sprite);
		VectorInt size = sprite->GetSize();
		backGround->SetPos(Vector(size.x / 2.f, size.y / 2.f));

		GMapSizeX = sprite->GetSize().x;
		GMapSizeY = sprite->GetSize().y;

		AddActor(backGround);
	}
#endif

#if 1
	// City stage - Back
	{
		Sprite* sprite = _resourceManager->GetSprite(L"City_Back1");
		SpriteActor* backGround = new SpriteActor(LAYER_BACKGROUND);
		backGround->SetSprite(sprite);
		VectorInt size = sprite->GetSize();
		backGround->SetPos(Pos(size.x / 2.f, size.y / 2.f));

		GMapSizeX = sprite->GetSize().x;
		GMapSizeY = sprite->GetSize().y;

		AddActor(backGround);
	}
	// City stage - Fore
	{
		Sprite* sprite = _resourceManager->GetSprite(L"City_Fore1");
		SpriteActor* foreGround = new SpriteActor(LAYER_FOREGROUND);
		foreGround->SetSprite(sprite);
		VectorInt size = sprite->GetSize();
		foreGround->SetPos(Pos(size.x / 2.f, size.y / 2.f));

		AddActor(foreGround);
	}
#endif
}

void DevScene::LoadPlayer()
{
	// 사이즈 100%
#if 1

	// 크기 100% 상태

	// Load Textures
	{
		_resourceManager->LoadTexture(L"PlayerDown", L"Sprite\\Player\\PlayerDown.bmp", RGB(128, 128, 128));
		_resourceManager->LoadTexture(L"PlayerUp", L"Sprite\\Player\\PlayerUp.bmp", RGB(128, 128, 128));
		_resourceManager->LoadTexture(L"PlayerLeft", L"Sprite\\Player\\PlayerLeft.bmp", RGB(128, 128, 128));
		_resourceManager->LoadTexture(L"PlayerRight", L"Sprite\\Player\\PlayerRight.bmp", RGB(128, 128, 128));
	}
	// Load Flipbooks
	{
		// IDLE
		{
			Texture* texture = _resourceManager->GetTexture(L"PlayerUp");
			Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_IdleUp");
			fb->SetFlipbookInfo({ texture, L"FB_MoveUp", {200, 200}, 0, 9, 0, 0.5f });
		}
		{
			Texture* texture = _resourceManager->GetTexture(L"PlayerDown");
			Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_IdleDown");
			fb->SetFlipbookInfo({ texture, L"FB_MoveDown", {200, 200}, 0, 9, 0, 0.5f });
		}
		{
			Texture* texture = _resourceManager->GetTexture(L"PlayerLeft");
			Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_IdleLeft");
			fb->SetFlipbookInfo({ texture, L"FB_MoveLeft", {200, 200}, 0, 9, 0, 0.5f });
		}
		{
			Texture* texture = _resourceManager->GetTexture(L"PlayerRight");
			Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_IdleRight");
			fb->SetFlipbookInfo({ texture, L"FB_MoveRight", {200, 200}, 0, 9, 0, 0.5f });
		}
		// MOVE
		{
			Texture* texture = _resourceManager->GetTexture(L"PlayerUp");
			Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_MoveUp");
			fb->SetFlipbookInfo({ texture, L"FB_MoveUp", {200, 200}, 0, 9, 1, 0.5f });
		}
		{
			Texture* texture = _resourceManager->GetTexture(L"PlayerDown");
			Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_MoveDown");
			fb->SetFlipbookInfo({ texture, L"FB_MoveDown", {200, 200}, 0, 9, 1, 0.5f });
		}
		{
			Texture* texture = _resourceManager->GetTexture(L"PlayerLeft");
			Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_MoveLeft");
			fb->SetFlipbookInfo({ texture, L"FB_MoveLeft", {200, 200}, 0, 9, 1, 0.5f });
		}
		{
			Texture* texture = _resourceManager->GetTexture(L"PlayerRight");
			Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_MoveRight");
			fb->SetFlipbookInfo({ texture, L"FB_MoveRight", {200, 200}, 0, 9, 1, 0.5f });
		}
		// ATTACK
		{
			Texture* texture = _resourceManager->GetTexture(L"PlayerUp");
			Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_AttackUp");
			fb->SetFlipbookInfo({ texture, L"FB_MoveUp", {200, 200}, 0, 7, 3, 0.4f, false });
		}
		{
			Texture* texture = _resourceManager->GetTexture(L"PlayerDown");
			Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_AttackDown");
			fb->SetFlipbookInfo({ texture, L"FB_MoveDown", {200, 200}, 0, 7, 3, 0.4f, false });
		}
		{
			Texture* texture = _resourceManager->GetTexture(L"PlayerLeft");
			Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_AttackLeft");
			fb->SetFlipbookInfo({ texture, L"FB_MoveLeft", {200, 200}, 0, 7, 3, 0.4f, false });
		}
		{
			Texture* texture = _resourceManager->GetTexture(L"PlayerRight");
			Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_AttackRight");
			fb->SetFlipbookInfo({ texture, L"FB_MoveRight", {200, 200}, 0, 7, 3, 0.4f, false });
		}

		// Arrow
		{
			Texture* texture = _resourceManager->GetTexture(L"PlayerUp");
			Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_BowUp");
			fb->SetFlipbookInfo({ texture, L"FB_MoveUp", {200, 200}, 0, 7, 5, 0.5f, false });
		}
		{
			Texture* texture = _resourceManager->GetTexture(L"PlayerDown");
			Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_BowDown");
			fb->SetFlipbookInfo({ texture, L"FB_MoveDown", {200, 200}, 0, 7, 5, 0.5f, false });
		}
		{
			Texture* texture = _resourceManager->GetTexture(L"PlayerLeft");
			Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_BowLeft");
			fb->SetFlipbookInfo({ texture, L"FB_MoveLeft", {200, 200}, 0, 7, 5, 0.5f, false });
		}
		{
			Texture* texture = _resourceManager->GetTexture(L"PlayerRight");
			Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_BowRight");
			fb->SetFlipbookInfo({ texture, L"FB_MoveRight", {200, 200}, 0, 7, 5, 0.5f, false });
		}
	}

#endif


	// 사이즈 90%
#if 0
	// 90% 축소

	// Load Textures
	{
		_resourceManager->LoadTexture(L"PlayerDown", L"Sprite\\Player\\PlayerDown90Crop.bmp", RGB(128, 128, 128));
		_resourceManager->LoadTexture(L"PlayerUp", L"Sprite\\Player\\PlayerUp90Crop.bmp", RGB(128, 128, 128));
		_resourceManager->LoadTexture(L"PlayerLeft", L"Sprite\\Player\\PlayerLeft90Crop.bmp", RGB(128, 128, 128));
		_resourceManager->LoadTexture(L"PlayerRight", L"Sprite\\Player\\PlayerRight90Crop.bmp", RGB(128, 128, 128));
	}
	// Load Flipbooks
	{
		// IDLE
		{
			Texture* texture = _resourceManager->GetTexture(L"PlayerUp");
			Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_IdleUp");
			fb->SetFlipbookInfo({ texture, L"FB_MoveUp", {180, 180}, 0, 9, 0, 0.5f });
		}
		{
			Texture* texture = _resourceManager->GetTexture(L"PlayerDown");
			Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_IdleDown");
			fb->SetFlipbookInfo({ texture, L"FB_MoveDown", {180, 180}, 0, 9, 0, 0.5f });
		}
		{
			Texture* texture = _resourceManager->GetTexture(L"PlayerLeft");
			Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_IdleLeft");
			fb->SetFlipbookInfo({ texture, L"FB_MoveLeft", {180, 180}, 0, 9, 0, 0.5f });
		}
		{
			Texture* texture = _resourceManager->GetTexture(L"PlayerRight");
			Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_IdleRight");
			fb->SetFlipbookInfo({ texture, L"FB_MoveRight", {180, 180}, 0, 9, 0, 0.5f });
		}
		// MOVE
		{
			Texture* texture = _resourceManager->GetTexture(L"PlayerUp");
			Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_MoveUp");
			fb->SetFlipbookInfo({ texture, L"FB_MoveUp", {180, 180}, 0, 9, 1, 0.5f });
		}
		{
			Texture* texture = _resourceManager->GetTexture(L"PlayerDown");
			Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_MoveDown");
			fb->SetFlipbookInfo({ texture, L"FB_MoveDown", {180, 180}, 0, 9, 1, 0.5f });
		}
		{
			Texture* texture = _resourceManager->GetTexture(L"PlayerLeft");
			Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_MoveLeft");
			fb->SetFlipbookInfo({ texture, L"FB_MoveLeft", {180, 180}, 0, 9, 1, 0.5f });
		}
		{
			Texture* texture = _resourceManager->GetTexture(L"PlayerRight");
			Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_MoveRight");
			fb->SetFlipbookInfo({ texture, L"FB_MoveRight", {180, 180}, 0, 9, 1, 0.5f });
		}
		// ATTACK
		{
			Texture* texture = _resourceManager->GetTexture(L"PlayerUp");
			Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_AttackUp");
			fb->SetFlipbookInfo({ texture, L"FB_MoveUp", {180, 180}, 0, 7, 3, 0.4f, false });
		}
		{
			Texture* texture = _resourceManager->GetTexture(L"PlayerDown");
			Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_AttackDown");
			fb->SetFlipbookInfo({ texture, L"FB_MoveDown", {180, 180}, 0, 7, 3, 0.4f, false });
		}
		{
			Texture* texture = _resourceManager->GetTexture(L"PlayerLeft");
			Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_AttackLeft");
			fb->SetFlipbookInfo({ texture, L"FB_MoveLeft", {180, 180}, 0, 7, 3, 0.4f, false });
		}
		{
			Texture* texture = _resourceManager->GetTexture(L"PlayerRight");
			Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_AttackRight");
			fb->SetFlipbookInfo({ texture, L"FB_MoveRight", {180, 180}, 0, 7, 3, 0.4f, false });
		}

		// Arrow
		{
			Texture* texture = _resourceManager->GetTexture(L"PlayerUp");
			Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_BowUp");
			fb->SetFlipbookInfo({ texture, L"FB_MoveUp", {180, 180}, 0, 7, 5, 0.5f, false });
		}
		{
			Texture* texture = _resourceManager->GetTexture(L"PlayerDown");
			Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_BowDown");
			fb->SetFlipbookInfo({ texture, L"FB_MoveDown", {180, 180}, 0, 7, 5, 0.5f, false });
		}
		{
			Texture* texture = _resourceManager->GetTexture(L"PlayerLeft");
			Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_BowLeft");
			fb->SetFlipbookInfo({ texture, L"FB_MoveLeft", {180, 180}, 0, 7, 5, 0.5f, false });
		}
		{
			Texture* texture = _resourceManager->GetTexture(L"PlayerRight");
			Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_BowRight");
			fb->SetFlipbookInfo({ texture, L"FB_MoveRight", {180, 180}, 0, 7, 5, 0.5f, false });
		}
	}
#endif

}

void DevScene::LoadMonster()
{
	_resourceManager->LoadTexture(L"Snake", L"Sprite\\Monster\\Snake.bmp", RGB(128, 128, 128));

	{
		Texture* texture = _resourceManager->GetTexture(L"Snake");
		Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_SnakeDown");
		fb->SetFlipbookInfo({ texture, L"FB_SnakeDown", {100, 100}, 0, 3, 0, 0.5f });
	}
	{
		Texture* texture = _resourceManager->GetTexture(L"Snake");
		Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_SnakeRight");
		fb->SetFlipbookInfo({ texture, L"FB_SnakeRight", {100, 100}, 0, 3, 1, 0.5f });
	}
	{
		Texture* texture = _resourceManager->GetTexture(L"Snake");
		Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_SnakeLeft");
		fb->SetFlipbookInfo({ texture, L"FB_SnakeLeft", {100, 100}, 0, 3, 2, 0.5f });
	}
	{
		Texture* texture = _resourceManager->GetTexture(L"Snake");
		Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_SnakeUp");
		fb->SetFlipbookInfo({ texture, L"FB_SnakeUp", {100, 100}, 0, 3, 3, 0.5f });
	}
}

void DevScene::LoadProjectile()
{
	_resourceManager->LoadTexture(L"Arrow", L"Sprite\\Item\\Arrow.bmp", RGB(128, 128, 128));

	{
		Texture* texture = _resourceManager->GetTexture(L"Arrow");
		Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_ArrowDown");
		fb->SetFlipbookInfo({ texture, L"FB_ArrowDown", {100, 100}, 0, 0, 0, 0.5f });
	}
	{
		Texture* texture = _resourceManager->GetTexture(L"Arrow");
		Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_ArrowLeft");
		fb->SetFlipbookInfo({ texture, L"FB_ArrowLeft", {100, 100}, 0, 0, 1, 0.5f });
	}
	{
		Texture* texture = _resourceManager->GetTexture(L"Arrow");
		Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_ArrowRight");
		fb->SetFlipbookInfo({ texture, L"FB_ArrowRight", {100, 100}, 0, 0, 2, 0.5f });
	}
	{
		Texture* texture = _resourceManager->GetTexture(L"Arrow");
		Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_ArrowUp");
		fb->SetFlipbookInfo({ texture, L"FB_ArrowUp", {100, 100}, 0, 0, 3, 0.5f });
	}
}

void DevScene::LoadEffects()
{
	_resourceManager->LoadTexture(L"HitEffect", L"Sprite\\Effect\\Hit.bmp", RGB(0, 0, 0));

	{
		Texture* texture = _resourceManager->GetTexture(L"HitEffect");
		Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_Hit_Sword");
		fb->SetFlipbookInfo({ texture, L"FB_Hit_Sword", {50, 47}, 0, 5, 0, 0.5f, false });
	}

	{
		Texture* texture = _resourceManager->GetTexture(L"Snake");
		Flipbook* fb = _resourceManager->CreateFlipbook(L"FB_Hit_Snake");
		fb->SetFlipbookInfo({ texture, L"FB_Hit_Snake", {100, 100}, 0, 7, 4, 0.4f, false });
	}
}

void DevScene::LoadTilemap()
{
	_resourceManager->LoadTexture(L"Tile", L"Sprite\\Map\\Tile.bmp", RGB(128, 128, 128));
	

	_resourceManager->CreateSprite(L"TileO", _resourceManager->GetTexture(L"Tile"), 0, 0, 48, 48);
	_resourceManager->CreateSprite(L"TileX", _resourceManager->GetTexture(L"Tile"), 48, 0, 48, 48);

#if 0
	// TilemapActor
	{
		Tilemap* tilemap = _resourceManager->CreateTilemap(L"tilemap01", { STAGE01_TILEX, STAGE01_TILEY }, STAGE01_TILE_SIZEX);
		TilemapActor* actor = new TilemapActor(tilemap);
		_tilemapActor = actor;
		_tilemap = tilemap;
		_mapSize = _tilemap->GetMapSize();

		_resourceManager->LoadTilemap(L"tilemap01", L"Tilemap\\Tilemap_01.txt");
		
		actor->SetShowForDebug(false);

		AddActor(actor);
	}
#endif

#if 1
	// CityTileMap
	{

		Tilemap* tilemap = _resourceManager->CreateTilemap(L"CityMap_01", { CITYSTAGE_TILEX, CITYSTAGE_TILEY }, CITYSTAGE_TILE_SIZE);
		TilemapActor* actor = new TilemapActor(tilemap);
		_tilemapActor = actor;
		_tilemap = tilemap;
		_mapSize = _tilemap->GetMapSize();

		_resourceManager->LoadTilemap(L"CityMap_01", L"Tilemap\\Tilemap_City.txt");
		actor->SetShowForDebug(false);

		AddActor(actor);

	}
#endif

	
}

void DevScene::LoadUI()
{
	_resourceManager->LoadTexture(L"Start", L"Sprite\\UI\\Start.bmp");
	_resourceManager->LoadTexture(L"Edit", L"Sprite\\UI\\Edit.bmp");
	_resourceManager->LoadTexture(L"Exit", L"Sprite\\UI\\Exit.bmp");

	_resourceManager->CreateSprite(L"Start_Off", _resourceManager->GetTexture(L"Start"), 0, 0, 150, 150);
	_resourceManager->CreateSprite(L"Start_On", _resourceManager->GetTexture(L"Start"), 150, 0, 150, 150);
	_resourceManager->CreateSprite(L"Edit_Off", _resourceManager->GetTexture(L"Edit"), 0, 0, 150, 150);
	_resourceManager->CreateSprite(L"Edit_On", _resourceManager->GetTexture(L"Edit"), 150, 0, 150, 150);
	_resourceManager->CreateSprite(L"Exit_Off", _resourceManager->GetTexture(L"Exit"), 0, 0, 150, 150);
	_resourceManager->CreateSprite(L"Exit_On", _resourceManager->GetTexture(L"Exit"), 150, 0, 150, 150);

	ASSERT_CRASH(_hwnd != nullptr);

}

void DevScene::LoadSound(bool turnOn)
{
#if 1
	_resourceManager->LoadSound(L"BGM1", L"Sound\\SUM_MR.wav");
	{
		Sound* sound = _resourceManager->GetSound(L"BGM1");
		sound->Play(true, turnOn);
	}
#endif

#if 0
	_resourceManager->LoadSound(L"BGM1", L"Sound\\Exist_MR.wav");
	{
		Sound* sound = _resourceManager->GetSound(L"BGM1");
		sound->Play(turnOn);
	}
#endif
}


Pos DevScene::CellPosToOriginPos(VectorInt cellPos)
{
	int tileSize = _tilemap->GetTileSize();
	float x = tileSize * cellPos.x + (tileSize / 2.f);
	float y = tileSize * cellPos.y + (tileSize / 2.f);
	return Pos(x, y);
}


void DevScene::Handle_S_AddObject(Protocol::S_AddObject& pkt)
{
	uint64 myPlayerId = GET_SINGLE(SceneManager)->GetMyPlayerId();
	
	const int32 size = pkt.objects_size();

	for (int32 i = 0; i < size; ++i)
	{
		const Protocol::ObjectInfo& info = pkt.objects(i);
		if (myPlayerId == info.objectid())
			continue;

		uint64 id = info.objectid();
		ObjectType objectType = static_cast<ObjectType>((id >> 32));

		if (objectType != ObjectType::OBJECT_TYPE_PROJECTILE)
			AddObjectInMap(id, VectorInt(info.movestat().x(), info.movestat().y()));

		if (objectType == ObjectType::OBJECT_TYPE_PLAYER)
		{
			Player* player = SpawnObject<Player>(VectorInt{ info.movestat().x(), info.movestat().y() });
			player->SetDir(info.movestat().dir());
			player->SetState(info.movestat().state());
			player->SetInfo(info);
			_players.push_back(player);
		}
		else if (objectType == ObjectType::OBJECT_TYPE_MONSTER)
		{
			Snake* snake = SpawnObject<Snake>(VectorInt{ info.movestat().x(), info.movestat().y() });
			snake->SetDir(info.movestat().dir());
			snake->SetState(info.movestat().state());
			snake->SetInfo(info);
		}
		else if (objectType == ObjectType::OBJECT_TYPE_PROJECTILE)
		{
			Arrow* arrow = SpawnObject<Arrow>(VectorInt{ info.movestat().x(), info.movestat().y() });
			arrow->SetDir(info.movestat().dir());
			arrow->SetState(info.movestat().state());
			arrow->SetInfo(info);
		}
	}
}

void DevScene::Handle_S_RemoveObject(Protocol::S_RemoveObject& pkt)
{

	Player* myPlayer = reinterpret_cast<Player*>(GET_SINGLE(SceneManager)->GetMyPlayer());

	const int32 size = pkt.ids_size();
	for (int32 i = 0; i < size; ++i)
	{
		uint64 id = pkt.ids(i);
		GameObject* object = GetGameObject(id);
		ObjectType objectType = object->GetObjectType();

		if (objectType != ObjectType::OBJECT_TYPE_PROJECTILE)
			RemoveObjectInMap(id, object->GetCellPos());
		
		if (objectType == ObjectType::OBJECT_TYPE_PLAYER)
		{
			Player* player = static_cast<Player*>(object);

			if (myPlayer == player)
				GET_SINGLE(SceneManager)->SetMyPlayer(nullptr);

			auto it = std::find(_players.begin(), _players.end(), player);
			if (it != _players.end())
				_players.erase(it);
		}

		if (object)
			RemoveActor(object);
	}
}

void DevScene::AddObjectInMap(uint64 id, VectorInt pos)
{
	if (_objectIdMap[pos.y][pos.x] != 0)
		return;
	_objectIdMap[pos.y][pos.x] = id;
}

void DevScene::RemoveObjectInMap(uint64 id, VectorInt pos)
{
	if (_objectIdMap[pos.y][pos.x] != id)
	{
		VectorInt whereIs = FindObjectPosInMap(id);
		_objectIdMap[whereIs.y][whereIs.x] = 0;
		return;
	}
	_objectIdMap[pos.y][pos.x] = 0;
}

void DevScene::MoveObjectInMap(uint64 id, VectorInt from, VectorInt to)
{
	if (_objectIdMap[to.y][to.x] != 0)
	{
		return;
	}
	if (_objectIdMap[from.y][from.x] != id)
	{
		VectorInt whereIs = FindObjectPosInMap(id);
		if (whereIs.x == -1 && whereIs.y == -1)
			ASSERT_CRASH(false);
		from = whereIs;
	}
	

	_objectIdMap[to.y][to.x] = _objectIdMap[from.y][from.x];
	_objectIdMap[from.y][from.x] = 0;
}

uint64 DevScene::GetObjectIdInMap(VectorInt pos)
{
	return _objectIdMap[pos.y][pos.x];
}

VectorInt DevScene::FindObjectPosInMap(uint64 id)
{
	for (int32 y = 0; y < _mapSize.y; ++y)
	{
		for (int32 x = 0; x < _mapSize.x; ++x)
		{
			if (_objectIdMap[y][x] == id)
				return VectorInt(x, y);
		}
	}
	return VectorInt(-1, -1);
}

bool DevScene::CanGo(VectorInt dest)
{
	int32 x = dest.x;
	int32 y = dest.y;

	if (x < 0 || x >= _mapSize.x || y < 0 || y >= _mapSize.y)
		return false;
	if (_objectIdMap[y][x] != 0)
		return false;

	return true;
}


GameObject* DevScene::GetGameObject(uint64 id)
{
	for (Actor* actor : _actors[LAYER_OBJECT])
	{
		GameObject* gameObject = static_cast<GameObject*>(actor);
		if (gameObject && gameObject->GetInfo().objectid() == id)
			return gameObject;
	}
	return nullptr;
}



