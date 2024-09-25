#include "pch.h"
#include "SphereCollider.h"
#include "Actor.h"
#include "SceneManager.h"
#include "BoxCollider.h"

SphereCollider::~SphereCollider()
{
}

void SphereCollider::BeginPlay()
{
	Super::BeginPlay();
}

void SphereCollider::TickComponent()
{
	Super::TickComponent();
}

void SphereCollider::Render(HDC hdc)
{
	Super::Render(hdc);
	if (!_showForDebug)
		return;

	Pos pos = _owner->GetPos();
	Pos cameraPos = GET_SINGLE(SceneManager)->GetCameraPos();

	pos.x -= (cameraPos.x - GWinSizeX / 2.f);
	pos.y -= (cameraPos.y - GWinSizeY / 2.f);

	HBRUSH oldOne = (HBRUSH)::SelectObject(hdc, myBrush);
	Utils::DrawCircle(hdc, pos, _radius);
	::SelectObject(hdc, oldOne);

}

bool SphereCollider::CheckCollision(Collider* other)
{
	ColliderType othersType = other->GetColliderType();

	switch (othersType)
	{
	case ColliderType::Sphere:
		return CheckSphereToSphereCollision(this, static_cast<SphereCollider*>(other));
	case ColliderType::Box:
		return CheckSphereToBoxCollision(this, static_cast<BoxCollider*>(other));
	default:
		CRASH("No Such Type");

	}

	return false;
}
