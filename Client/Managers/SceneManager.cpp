#include "pch.h"
#include "SceneManager.h"
#include "DevScene.h"
#include "EditScene.h"
#include "MyPlayer.h"
#include "GameObject.h"

SceneManager::~SceneManager()
{
	SAFE_DELETE(_scene);
		
}

void SceneManager::Init(HWND hwnd)
{
	_hwnd = hwnd;
	_scene = new DevScene(_hwnd);
	_sceneType = SceneType::DevScene;
	ASSERT_CRASH(_scene != nullptr);
	_scene->Init();
}

void SceneManager::Update()
{
	_scene->Update();
}

void SceneManager::Render(HDC hdc)
{
	_scene->Render(hdc);
}

void SceneManager::ChangeScene(SceneType type)
{
	if (_scene->GetType() == type)
		return;

	Scene* scene = nullptr;

	switch (type)
	{
	case SceneType::DevScene:
		scene = new DevScene(_hwnd);
		_sceneType = SceneType::DevScene;
		break;
	case SceneType::EditScene:
		scene = new EditScene();
		_sceneType = SceneType::EditScene;
		break;
	default:
		CRASH("SceneType Error");
		return;
	}

	_scene = scene;
}

DevScene* SceneManager::GetDevScene()
{
	if (_sceneType == SceneType::DevScene)
		return static_cast<DevScene*>(_scene);
	return nullptr;
}

uint64 SceneManager::GetMyPlayerId()
{
	return _myPlayer ? _myPlayer->GetObjectId() : 0;
}
