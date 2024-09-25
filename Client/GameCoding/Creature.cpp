#include "pch.h"
#include "Creature.h"
#include "SceneManager.h"
#include "Scene.h"
#include "DevScene.h"

Creature::~Creature()
{
}

void Creature::BeginPlay()
{
	Super::BeginPlay();
}

void Creature::Tick()
{
	Super::Tick();
}

void Creature::Render(HDC hdc)
{
	Super::Render(hdc);
}

void Creature::TickIdle()
{
}

void Creature::TickMove()
{
}

void Creature::TickAttack()
{
}

