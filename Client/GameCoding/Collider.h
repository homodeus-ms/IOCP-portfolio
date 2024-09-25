#pragma once
#include "Component.h"

class SphereCollider;
class BoxCollider;

class Collider : public Component
{
	using Super = Component;
public:
	Collider(ColliderType type) : _colliderType(type) {}
	virtual ~Collider();

	virtual void BeginPlay() override;
	virtual void TickComponent() override;
	virtual void Render(HDC hdc) override;

	virtual bool CheckCollision(Collider* other) abstract;
	
	void SetShowForDebug(bool b) { _showForDebug = b; }
	ColliderType GetColliderType() const { return _colliderType; }

	void AddColliderToSet(Collider* collider);
	void RemoveColliderFromSet(Collider* collider);
	bool IsColliderInSet(Collider* collider);

protected:
	bool CheckBoxToBoxCollision(BoxCollider* c1, BoxCollider* c2);
	bool CheckSphereToSphereCollision(SphereCollider* c1, SphereCollider* c2);
	bool CheckSphereToBoxCollision(SphereCollider* c1, BoxCollider* c2);
	
private:
	bool pointToBoxCollision(Pos point, Pos leftTop, Pos rightBottom);
	bool pointToCircleCollision(Pos point, Pos center, uint32 radius);

protected:
	ColliderType _colliderType;
	bool _showForDebug = true;
	unordered_set<Collider*> _onColliderSet;

};

