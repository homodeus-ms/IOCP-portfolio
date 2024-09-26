#include "pch.h"
#include "Arrow.h"
#include "ResourceManager.h"
#include "Player.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "DevScene.h"
#include "Effect.h"

Arrow::Arrow()
	: Super()
{
	SetLayer(LayerType::LAYER_OBJECT);

	_flipbooks[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_ArrowUp");
	_flipbooks[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_ArrowDown");
	_flipbooks[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_ArrowLeft");
	_flipbooks[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_ArrowRight");

	
}

Arrow::~Arrow()
{
}

void Arrow::BeginPlay()
{
	Super::BeginPlay();
	_startPos = GetCellPos();
}

void Arrow::Tick()
{
	Super::Tick();
}

void Arrow::Render(HDC hdc)
{
	if (GetCellPos() == _startPos)
		return;
	Super::Render(hdc);
}

void Arrow::UpdateAnimation()
{
	SetFlipbook(_flipbooks[GetDir()]);
}

void Arrow::SetOwner(Player* player)
{
	_owner = player;
	_pos = _owner->GetPos();
	_cellPos = _owner->GetCellPos();
}

void Arrow::TickIdle()
{
	
}

void Arrow::TickMove()
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
