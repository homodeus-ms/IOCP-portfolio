#include "pch.h"
#include "Actor.h"
#include "Component.h"
#include "SceneManager.h"


Actor::~Actor()
{
	for (Component* component : _components)
		SAFE_DELETE(component);

	::DeleteObject(redBrush);
}

void Actor::BeginPlay()
{
	for (Component* component : _components)
		component->BeginPlay();
}

void Actor::Tick()
{
	for (Component* component : _components)
		component->TickComponent();
}

void Actor::Render(HDC hdc)
{
	for (Component* component : _components)
		component->Render(hdc);
	
	/*if (_isOverlapped)
	{
		Vector cameraPos = GET_SINGLE(SceneManager)->GetCameraPos();
		Vector pos = _pos;
		pos.x -= (cameraPos.x - GWinSizeX / 2.f);
		pos.y -= (cameraPos.y - GWinSizeY / 2.f);
		pos.y -= 60;

		HBRUSH oldone = (HBRUSH)::SelectObject(hdc, redBrush);
		::SetDCBrushColor(hdc, (RGB(255, 0, 0)));
		Utils::DrawCircle(hdc, pos, 8);
	}*/
}

void Actor::AddComponent(Component* component)
{
	if (component == nullptr)
		return;
	component->SetOwner(this);
	_components.push_back(component);
}

void Actor::RemoveComponent(Component* component)
{
	auto found = std::find(_components.begin(), _components.end(), component);
	if (found == _components.end())
		return;
	_components.erase(found);
}

void Actor::BeginCollision(Collider* src, Collider* dst)
{
	_isOverlapped = true;
}

void Actor::EndCollision(Collider* src, Collider* dst)
{
	_isOverlapped = false;
}


