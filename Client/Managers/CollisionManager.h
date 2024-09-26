#pragma once

class Collider;

class CollisionManager
{
	DECLARE_SINGLE(CollisionManager);

public:
	void Init();
	void Update();
	void Render(HDC hdc);

	void AddCollider(Collider* collider);
	void RemoveCollider(Collider* collider);

private:

	vector<Collider*> _colliders;
};

