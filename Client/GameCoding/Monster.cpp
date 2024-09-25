#include "pch.h"
#include "Monster.h"

Monster::Monster()
	: Super()
{
}

Monster::~Monster()
{
	
}


void Monster::BeginPlay()
{
	Super::BeginPlay();
}

void Monster::Tick()
{
	Super::Tick();
}

void Monster::Render(HDC hdc)
{
	Super::Render(hdc);
}


void Monster::UpdateAnimation()
{
}

void Monster::TickIdle()
{
}

void Monster::TickMove()
{
}

void Monster::TickAttack()
{
}
