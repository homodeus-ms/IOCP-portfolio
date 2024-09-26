#pragma once

class Game
{
public:
	Game();
	~Game();

	void Init(HWND hwnd);
	void Update();
	void Render();

	void HandleChatInput(UINT message, WPARAM wParam, LPARAM lParam);

private:
	HWND _hwnd = {};
	HDC _hdc = {};

	// for Double Buffering
	RECT _rect = {};
	HDC _hdcBack = {};
	HBITMAP _bmpBack = {};

};
