#include "pch.h"
#include "Collider.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "Actor.h"
#include "SceneManager.h"

Collider::~Collider()
{
}

void Collider::BeginPlay()
{
	
}

void Collider::TickComponent()
{
}

void Collider::Render(HDC hdc)
{
	
}


void Collider::AddColliderToSet(Collider* collider)
{
	if (collider == nullptr)
		return;
	_onColliderSet.insert(collider);
}

void Collider::RemoveColliderFromSet(Collider* collider)
{
	if (collider == nullptr)
		return;
	_onColliderSet.erase(collider);
}

bool Collider::IsColliderInSet(Collider* collider)
{
	if (collider == nullptr)
		return false;
	return _onColliderSet.contains(collider);;
}

bool Collider::pointToBoxCollision(Pos point, Pos leftTop, Pos rightBottom)
{
	float px = point.x;
	float py = point.y;
	
	if (px >= leftTop.x && px <= rightBottom.x && py >= leftTop.y && py <= rightBottom.y)
		return true;

	return false;
}

bool Collider::pointToCircleCollision(Pos point, Pos center, uint32 radius)
{
	Pos dir = { center.x - point.x, center.y - point.y };
	float dist = dir.Length();

	return dist <= radius;
}

bool Collider::CheckBoxToBoxCollision(BoxCollider* c1, BoxCollider* c2)
{
	Pos pos1 = c1->GetOwner()->GetPos();
	Pos pos2 = c2->GetOwner()->GetPos();
	Pos size1 = c1->GetBoxSize();
	Pos size2 = c2->GetBoxSize();

	float left1 = pos1.x - size1.x / 2.f;
	float right1 = pos1.x + size1.x / 2.f;
	float up1 = pos1.y - size1.y / 2.f;
	float down1 = pos1.y + size1.y / 2.f;

	float left2 = pos2.x - size2.x / 2.f;
	float right2 = pos2.x + size2.x / 2.f;
	float up2 = pos2.y - size2.y / 2.f;
	float down2 = pos2.y + size2.y / 2.f;

	if (left1 > right2)
		return false;
	if (right1 < left2)
		return false;
	if (up1 > down2)
		return false;
	if (down1 < up2)
		return false;

	return true;
}

bool Collider::CheckSphereToSphereCollision(SphereCollider* c1, SphereCollider* c2)
{
	Pos dir = c1->GetOwner()->GetPos();
	Pos pos2 = c2->GetOwner()->GetPos();
	dir -= pos2;
	float length = dir.Length();

	uint32 radiusSum = c1->GetRadius() + c2->GetRadius();

	return length <= radiusSum;
}

bool Collider::CheckSphereToBoxCollision(SphereCollider* c1, BoxCollider* c2)
{
	Pos boxPos = c2->GetOwner()->GetPos();
	Pos boxSize = c2->GetBoxSize();
	uint32 radius = c1->GetRadius();
	Pos center = c1->GetOwner()->GetPos();
	Pos leftTop = { boxPos.x - boxSize.x / 2.f, boxPos.y - boxSize.y / 2.f };
	Pos rightBottom = { boxPos.x + boxSize.x / 2.f, boxPos.y + boxSize.y / 2.f };

	if (center.x >= leftTop.x && center.x <= rightBottom.x || center.y >= leftTop.y && center.y <= rightBottom.y)
	{
		float radiusToFloat = static_cast<float>(radius);
		leftTop -= {radiusToFloat, radiusToFloat};
		rightBottom += {radiusToFloat, radiusToFloat};

		if (pointToBoxCollision(center, leftTop, rightBottom))
			return true;
	}
	else
	{
		if (pointToCircleCollision(leftTop, center, radius))
			return true;
		if (pointToCircleCollision({ rightBottom.x, leftTop.y }, center, radius))
			return true;
		if (pointToCircleCollision(rightBottom, center, radius))
			return true;
		if (pointToCircleCollision({ leftTop.x, rightBottom.y }, center, radius))
			return true;
	}
	

	return false;
	
	
}
