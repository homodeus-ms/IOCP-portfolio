#include "pch.h"
#include "Snake.h"
#include "Flipbook.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "DevScene.h"
#include "TimeManager.h"
#include "Effect.h"

Snake::Snake()
{
	_flipbook[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SnakeUp");
	_flipbook[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SnakeDown");
	_flipbook[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SnakeRight");
	_flipbook[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SnakeLeft");
	_hitEffect = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_Hit_Snake");

	SetState(IDLE);
	SetDir(DIR_DOWN);

}

Snake::~Snake()
{
	
}

void Snake::BeginPlay()
{
	Super::BeginPlay();

	SetFlipbook(_flipbook[DIR_DOWN]);
	DevScene* devScene = static_cast<DevScene*>(GET_SINGLE(SceneManager)->GetCurrScene());
	_pos = devScene->CellPosToOriginPos(GetCellPos());
	_dest = _pos;
	
}

void Snake::Tick()
{
	Super::Tick();

	switch (GetState())
	{
	case IDLE:
		TickIdle();
		break;
	case MOVE:
		TickMove();
		break;
	case SKILL:
		TickAttack();
		break;
	default:
		ASSERT_CRASH("SNAKE STATE ERROR");
		break;
	}
}

void Snake::Render(HDC hdc)
{
	Super::Render(hdc);
}

void Snake::UpdateAnimation()
{
	SetFlipbook(_flipbook[GetDir()]);
}

void Snake::TickIdle()
{
	
}

void Snake::TickMove()
{
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();
	DevScene* devScene = static_cast<DevScene*>(GET_SINGLE(SceneManager)->GetCurrScene());
	
	Pos dist = _dest - _pos;
	if (dist.Length() < 10.f)
	{
		_pos = _dest;
		
		SetState(IDLE);
		
		return;
	}

	bool horizontal = abs(dist.x) > abs(dist.y);
	if (horizontal)
		SetDir(dist.x < 0 ? DIR_LEFT : DIR_RIGHT);
	else
		SetDir(dist.y < 0 ? DIR_UP : DIR_DOWN);

	float speed = GetStat().speed();

	switch (GetDir())
	{
	case DIR_UP:
		_pos.y -= speed * deltaTime;
		break;
	case DIR_DOWN:
		_pos.y += speed * deltaTime;
		break;
	case DIR_LEFT:
		_pos.x -= speed * deltaTime;
		break;
	case DIR_RIGHT:
		_pos.x += speed * deltaTime;
		break;
	default:
		ASSERT_CRASH("DIR ERROR");
		break;
	}
}

void Snake::TickAttack()
{
	if (_flipbook == nullptr)
		return;
}
