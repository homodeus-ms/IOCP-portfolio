#pragma once
#include "UI.h"

class Sprite;

enum ButtonState
{
	BS_DEFAULT,
	BS_PRESSED,
	BS_CLICKED,
	BS_MAX_COUNT,
};

class Button : public UI
{
	using Super = UI;
public:
	Button();
	virtual ~Button();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

	void SetSprite(Sprite* sprite, ButtonState state) { _sprites[state] = sprite; }
	void SetState(ButtonState state);

	ButtonState GetState() const { return _state; }
	Sprite* GetCurrSprite() const { return _currSprite; }

public:
	std::function<void(void)> _onClick = nullptr;

	template<typename T>
	void AddOnClickDelegate(T* owner, void(T::*func)())
	{
		_onClick = [owner, func]()
		{
			(owner->*func)();
		};
	}

protected:
	Sprite* _currSprite = nullptr;
	ButtonState _state = BS_DEFAULT;
	Sprite* _sprites[BS_MAX_COUNT] = {};
	float _sumTime = 0.f;
};

