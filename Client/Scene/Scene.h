#pragma once

class Actor;
class UI;
class Creature;
class Player;

class Scene
{
public:
	Scene(SceneType type);
	virtual ~Scene();
	virtual void Init();
	virtual void Update();
	virtual void Render(HDC hdc);

	virtual void AddActor(Actor* actor);
	virtual void RemoveActor(Actor* actor);
	
	//virtual Creature* GetCreatureAtOrNull(VectorInt cellPos) { return nullptr; }

	void ClearTrash();
	void AddTrash(Actor* actor) { _trash.push_back(actor); }
	SceneType GetType() const { return _type; }

	void SetWriteReady(bool ready) { _writeReady = ready; }
	bool IsWriteReady() { return _writeReady; }

protected:
	SceneType _type = SceneType::None;
	vector<Actor*> _actors[LAYER_MAXCOUNT];
	vector<Actor*> _trash;
	vector<UI*> _uis;

	bool _writeReady = false;

};

