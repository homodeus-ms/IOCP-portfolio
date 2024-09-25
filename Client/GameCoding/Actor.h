#pragma once
class Component;
class Collider;


class Actor
{
public:
	Actor(LayerType layerType = LAYER_OBJECT) : _layerType(layerType) {}
	virtual ~Actor();

	virtual void BeginPlay();
	virtual void Tick();
	virtual void Render(HDC hdc);

	void SetPos(Pos pos) { _pos = pos; }
	void SetDestPos(Pos dest) { _dest = dest; }
	Pos GetPos() const { return _pos; }
	Pos GetDestPos() const { return _dest; }
	virtual VectorInt GetCellPos() { return { 0, 0 }; }

	void AddComponent(Component* component);
	void RemoveComponent(Component* component);

	LayerType GetLayerType() const { return _layerType; }
	void SetLayer(LayerType type) { _layerType = type; }

	virtual void BeginCollision(Collider* src, Collider* dst);
	virtual void EndCollision(Collider* src, Collider* dst);
	

protected:
	
	Pos _pos = {};
	Pos _dest = {};
	vector<Component*> _components;
	LayerType _layerType = LAYER_OBJECT;

protected:
	// Collision 관련, 사용 안함, Tilemap으로
	bool _isOverlapped = false;
	HBRUSH redBrush = (HBRUSH)::GetStockObject(DC_BRUSH);

};

