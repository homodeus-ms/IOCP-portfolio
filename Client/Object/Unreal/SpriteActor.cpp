#include "pch.h"
#include "SpriteActor.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "DevScene.h"
#include "Sprite.h"


SpriteActor::~SpriteActor()
{
	
}

void SpriteActor::BeginPlay()
{
	Super::BeginPlay();
}

void SpriteActor::Tick()
{
	Super::Tick();
}

void SpriteActor::Render(HDC hdc)
{
	Super::Render(hdc);

	if (_sprite == nullptr)
		return;

	VectorInt size = _sprite->GetSize();
	Pos cameraPos = GET_SINGLE(SceneManager)->GetCameraPos();


	if (GetLayerType() == LAYER_FOREGROUND)
	{
		::TransparentBlt(hdc,
			(int32)_pos.x - size.x / 2 - ((int32)cameraPos.x - GWinSizeX / 2),
			(int32)_pos.y - size.y / 2 - ((int32)cameraPos.y - GWinSizeY / 2),
			size.x,
			size.y,
			_sprite->GetDc(),
			_sprite->GetPos().x,
			_sprite->GetPos().y,
			_sprite->GetSize().x,
			_sprite->GetSize().y,
			_sprite->GetTransparent());
	}
	else
	{
		::BitBlt(hdc,
			(int32)_pos.x - size.x / 2 - ((int32)cameraPos.x - GWinSizeX / 2),
			(int32)_pos.y - size.y / 2 - ((int32)cameraPos.y - GWinSizeY / 2),
			size.x,
			size.y,
			_sprite->GetDc(),
			_sprite->GetPos().x,
			_sprite->GetPos().y,
			SRCCOPY);
	}
}



