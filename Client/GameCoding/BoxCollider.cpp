#include "pch.h"
#include "BoxCollider.h"
#include "Actor.h"
#include "SceneManager.h"
#include "SphereCollider.h"

BoxCollider::~BoxCollider()
{
}

void BoxCollider::BeginPlay()
{
	Super::BeginPlay();
}

void BoxCollider::TickComponent()
{
	Super::TickComponent();
}

void BoxCollider::Render(HDC hdc)
{
	Super::Render(hdc);
	if (!_showForDebug)
		return;

	Pos pos = _owner->GetPos();
	Pos cameraPos = GET_SINGLE(SceneManager)->GetCameraPos();

	pos.x -= (cameraPos.x - GWinSizeX / 2.f);
	pos.y -= (cameraPos.y - GWinSizeY / 2.f);

	HBRUSH oldOne = (HBRUSH)::SelectObject(hdc, myBrush);
	Utils::DrawRect(hdc, pos, static_cast<int32>(_size.x), static_cast<int32>(_size.y));
	::SelectObject(hdc, oldOne);
}

bool BoxCollider::CheckCollision(Collider* other)
{
	ColliderType othersType = other->GetColliderType();

	switch (othersType)
	{
	case ColliderType::Box:
		return CheckBoxToBoxCollision(this, static_cast<BoxCollider*>(other));
	case ColliderType::Sphere:
		return CheckSphereToBoxCollision(static_cast<SphereCollider*>(other), this);
	default:
		CRASH("No Such Type");

	}

	return false;
}
