#include "pch.h"
#include "CollisionManager.h"
#include "Collider.h"
#include "Actor.h"

void CollisionManager::Init()
{
}

void CollisionManager::Update()
{
	uint32 size = static_cast<uint32>(_colliders.size());

	for (uint32 i = 0; i < size - 1; ++i)
	{
		for (uint32 j = i + 1; j < size; ++j)
		{
			Collider* src = _colliders[i];
			Collider* dst = _colliders[j];

			if (src->CheckCollision(dst))
			{
				if (!src->IsColliderInSet(dst))
				{
					src->AddColliderToSet(dst);
					dst->AddColliderToSet(src);

					src->GetOwner()->BeginCollision(src, dst);
					dst->GetOwner()->BeginCollision(dst, src);
				}
			}
			else
			{
				if (src->IsColliderInSet(dst))
				{
					src->RemoveColliderFromSet(dst);
					dst->RemoveColliderFromSet(src);
					src->GetOwner()->EndCollision(src, dst);
					dst->GetOwner()->EndCollision(dst, src);
				}
			}
		}
	}
}

void CollisionManager::Render(HDC hdc)
{
}

void CollisionManager::AddCollider(Collider* collider)
{
	if (collider == nullptr)
		return;
	_colliders.push_back(collider);
}

void CollisionManager::RemoveCollider(Collider* collider)
{
	if (collider == nullptr)
		return;
	auto it = std::remove(_colliders.begin(), _colliders.end(), collider);
	_colliders.erase(it, _colliders.end());
}
