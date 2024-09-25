#pragma once

#include "ChatBox.h"

class UiManager
{
	DECLARE_SINGLE(UiManager);

public:
	void Init(HWND hwnd);
	void Update();
	void Render(HDC hdc);


private:
	HWND _hwnd = {};
};

