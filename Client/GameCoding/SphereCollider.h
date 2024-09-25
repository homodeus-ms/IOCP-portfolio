#pragma once
#include "Collider.h"

class SphereCollider : public Collider
{
	using Super = Collider;
public:
	SphereCollider(uint32 radius) : Super(ColliderType::Sphere), _radius(radius) {}
	virtual ~SphereCollider();

	virtual void BeginPlay() override;
	virtual void TickComponent() override;
	virtual void Render(HDC hdc) override;

	virtual bool CheckCollision(Collider* other) override;
	
	void SetRadius(uint32 radius) { _radius = radius; }
	uint32 GetRadius() const { return _radius; }

private:
	uint32 _radius;
};

