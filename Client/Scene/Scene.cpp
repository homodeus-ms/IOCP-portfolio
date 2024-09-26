#include "pch.h"
#include "UI.h"
#include "Scene.h"
#include "Actor.h"
#include "Player.h"
#include "SceneManager.h"


Scene::Scene(SceneType type)
	: _type(type)
{
	_trash.reserve(50);
	_actors->reserve(LAYER_MAXCOUNT);
	_actors[LAYER_OBJECT].reserve(50);
}

Scene::~Scene()
{
	for (const vector<Actor*>& vector : _actors)
		for (Actor* actor : vector)
			SAFE_DELETE(actor);

	_actors->clear();

	for (UI* ui : _uis)
		SAFE_DELETE(ui);

	_uis.clear();
}

void Scene::Init()
{
	for (vector<Actor*>& vector : _actors)
		for (Actor* actor : vector)
			actor->BeginPlay();

	for (UI* ui : _uis)
		ui->BeginPlay();
}

void Scene::Update()
{
	if (_trash.size() > 30)
	{
		ClearTrash();
	}
	vector<Actor*>& actors = _actors[LAYER_OBJECT];
	std::sort(actors.begin(), actors.end(), [=](Actor* a, Actor* b) {
		return a->GetPos().y < b->GetPos().y;
		});

	for (vector<Actor*> vector : _actors)
		for (Actor* actor : vector)
			actor->Tick();

	for (UI* ui : _uis)
		ui->Tick();
}

void Scene::Render(HDC hdc)
{
	for (vector<Actor*>& vector : _actors)
		for (Actor* actor : vector)
			actor->Render(hdc);

	for (UI* ui : _uis)
		ui->Render(hdc);
}

void Scene::AddActor(Actor* actor)
{
	if (actor == nullptr)
		return;
	_actors[actor->GetLayerType()].push_back(actor);
	
}

void Scene::RemoveActor(Actor* actor)
{
	if (actor == nullptr)
		return;

	vector<Actor*>& actors = _actors[actor->GetLayerType()];
	auto it = std::remove(actors.begin(), actors.end(), actor);
	actors.erase(it, actors.end());

	AddTrash(actor);
}

void Scene::ClearTrash()
{
	for (auto it = _trash.begin(); it != _trash.end(); ++it)
	{
		delete *it;
		*it = nullptr;
	}
	_trash.clear();
}
