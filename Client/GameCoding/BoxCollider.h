#pragma once
#include "Collider.h"

class BoxCollider : public Collider
{
	using Super = Collider;
public:
	BoxCollider(Pos size) : Super(ColliderType::Box), _size(size) {}
	virtual ~BoxCollider();

	virtual void BeginPlay() override;
	virtual void TickComponent() override;
	virtual void Render(HDC hdc) override;

	virtual bool CheckCollision(Collider* other) override;

	void SetBoxSize(Pos size) { _size = size; }
	Pos GetBoxSize() const { return _size; }

private:
	Pos _size;
};

