#pragma once
#include "Scene.h"

class EditScene : public Scene
{
public:
	EditScene() : Scene(SceneType::EditScene) {}
	virtual ~EditScene() {};
};

