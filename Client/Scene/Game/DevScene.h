#pragma once

#include "Scene.h"
#include "Creature.h"
#include "GameObject.h"
#include <type_traits>
#include <gdiplus.h>


class SpriteActor;
class Texture;
class Actor;
class UI;
class TilemapActor;
class Tilemap;
class ResourceManager;
class GameObject;


class DevScene : public Scene
{
	using Super = Scene;
public:
	DevScene(HWND hwnd);
	~DevScene();

	void Init() override;
	void Update() override;
	void Render(HDC hdc) override;

	void LoadMap();
	void LoadPlayer();
	void LoadMonster();
	void LoadProjectile();
	void LoadEffects();
	void LoadTilemap();
	void LoadUI();
	void LoadSound(bool turnOn);

	Pos CellPosToOriginPos(VectorInt cellPos);
	
	template<typename T>
	T* SpawnObject(VectorInt cellPos);

	// Handle Packet ÇÔ¼öµé
	void Handle_S_AddObject(Protocol::S_AddObject& pkt);
	void Handle_S_RemoveObject(Protocol::S_RemoveObject& pkt);

	void AddObjectInMap(uint64 id, VectorInt pos);
	void RemoveObjectInMap(uint64 id, VectorInt pos);
	void MoveObjectInMap(uint64 id, VectorInt from, VectorInt to);
	uint64 GetObjectIdInMap(VectorInt pos);
	VectorInt FindObjectPosInMap(uint64 id);
	bool CanGo(VectorInt dest);
	

	GameObject* GetGameObject(uint64 id);
	void AddPlayer(Player* player) { _players.push_back(player); }

public:
	
	unique_ptr<Gdiplus::Image> _foreImage = nullptr;

private:
	enum
	{
		MAP_EMPTY = 0,
		MAP_WALL = 1,
	};
	HWND _hwnd = {};

	TilemapActor* _tilemapActor = nullptr;
	Tilemap* _tilemap = nullptr;
	VectorInt _mapSize = {};
	ResourceManager* _resourceManager = nullptr;
	vector<Player*> _players;
	vector<vector<uint64>> _objectIdMap;
	
};


template<typename T>
inline T* DevScene::SpawnObject(VectorInt cellPos)
{
	auto ret = std::is_convertible_v<T*, GameObject*>;
	ASSERT_CRASH(ret);

	T* obj = new T();
	obj->SetCellPos(cellPos);
	AddActor(obj);
	obj->BeginPlay();

	return obj;
}
