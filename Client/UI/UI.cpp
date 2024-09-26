#include "pch.h"
#include "UI.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "TimeManager.h"

UI::UI()
{
}

UI::~UI()
{
}

void UI::BeginPlay()
{
}

void UI::Tick()
{
}

RECT UI::GetRect()
{
    RECT rect = {
        (LONG)_pos.x - _size.x / 2,
        (LONG)_pos.y - _size.y / 2,
        (LONG)_pos.x + _size.x / 2,
        (LONG)_pos.y + _size.y / 2,
    };

    return rect;
}

bool UI::IsMouseInRect()
{
    RECT rect = GetRect();
    POINT mousePos = GET_SINGLE(InputManager)->GetMousePos();

    if (mousePos.x >= rect.left && mousePos.x <= rect.right && mousePos.y >= rect.top && mousePos.y <= rect.bottom)
        return true;

    return false;
}

void UI::Render(HDC hdc)
{
}
