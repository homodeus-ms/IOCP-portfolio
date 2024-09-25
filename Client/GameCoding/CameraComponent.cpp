#include "pch.h"
#include "CameraComponent.h"
#include "SceneManager.h"
#include "Actor.h"

CameraComponent::CameraComponent()
{
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::BeginPlay()
{
}

void CameraComponent::TickComponent()
{
	if (_owner == nullptr)
		return;
	Pos pos = _owner->GetPos();

	pos.x = ::clamp(pos.x, static_cast<float>(GWinHalfX), static_cast<float>(GMapSizeX - GWinHalfX));
	pos.y = ::clamp(pos.y, static_cast<float>(GWinHalfY), static_cast<float>(GMapSizeY - GWinHalfY));

	GET_SINGLE(SceneManager)->SetCameraPos({ pos.x, pos.y });
}

void CameraComponent::Render(HDC hdc)
{
}
