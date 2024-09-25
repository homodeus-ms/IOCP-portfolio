#pragma once
#include "pch.h"
#include "Scene.h"

class Scene;
class CameraComponent;
class MyPlayer;

class SceneManager
{
	DECLARE_SINGLE(SceneManager);

public:

	~SceneManager();

	void Init(HWND hwnd);
	void Update();
	void Render(HDC hdc);

	void ChangeScene(SceneType type);
	Scene* GetCurrScene() const { return _scene; }

	void SetCameraPos(Pos pos) { _cameraPos = pos; }
	Pos GetCameraPos() const { return _cameraPos; }

	class DevScene* GetDevScene();

	MyPlayer* GetMyPlayer() const { return _myPlayer; }
	void SetMyPlayer(MyPlayer* player) { _myPlayer = player; }
	uint64 GetMyPlayerId();
	void SetWriteReadyState(bool ready) { _scene->SetWriteReady(ready); }

private:
	HWND _hwnd = {};
	Scene* _scene = nullptr;
	SceneType _sceneType = SceneType::None;
	Pos _cameraPos = { 400, 300 };
	MyPlayer* _myPlayer = nullptr;
};