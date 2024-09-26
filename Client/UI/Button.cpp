#include "pch.h"
#include "Button.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Sprite.h"

Button::Button()
{
}

Button::~Button()
{
	for (Sprite* sprite : _sprites)
		SAFE_DELETE(sprite);
}

void Button::BeginPlay()
{
	Super::BeginPlay();
	if (_sprites[BS_DEFAULT])
		_currSprite = _sprites[BS_DEFAULT];

}

void Button::Tick()
{
	Super::Tick();
	
	POINT mousePos = GET_SINGLE(InputManager)->GetMousePos();
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	if (_state == BS_CLICKED)
	{
		_sumTime += deltaTime;

		if (_sumTime >= 0.2f)
		{
			_sumTime = 0.f;
			SetState(BS_DEFAULT);
		}
	}

	if (IsMouseInRect())
	{
		if (GET_SINGLE(InputManager)->IsKeyDown(LClick))
		{
			SetState(BS_PRESSED);
			// todo when pressed
		}
		else
		{
			if (_state == BS_PRESSED)
			{
				SetState(BS_CLICKED);
				if (_onClick)
					_onClick();
			}
		}
	}
	else
	{
		SetState(BS_DEFAULT);
	}
}

void Button::Render(HDC hdc)
{
	if (_currSprite)
	{
		::TransparentBlt(hdc,
			(int32)_pos.x - _size.x / 2,
			(int32)_pos.y - _size.y / 2,
			_size.x,
			_size.y,
			_currSprite->GetDc(),
			_currSprite->GetPos().x,
			_currSprite->GetPos().y,
			_currSprite->GetSize().x,
			_currSprite->GetSize().y,
			_currSprite->GetTransparent());
	}
	else
	{
		Utils::DrawRect(hdc, _pos, _size.x, _size.y);
	}
}


void Button::SetState(ButtonState state)
{
	_state = state;
	if (_sprites != NULL)
		_currSprite = _sprites[_state];
}


