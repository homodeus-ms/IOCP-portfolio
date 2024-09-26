#pragma once

#include "ChatBox.h"

enum class KeyState
{
	None,
	Press,
	Down,
	Up,
	End,
};
enum KeyType
{
	LClick = VK_LBUTTON,
	RClick = VK_RBUTTON,

	Up = VK_UP,
	Down = VK_DOWN,
	Right = VK_RIGHT,
	Left = VK_LEFT,
	SpaceBar = VK_SPACE,
	Enter = VK_RETURN,

	W = 'W',
	S = 'S',
	D = 'D',
	A = 'A',
	Q = 'Q',
	E = 'E',
	V = 'V',

	Key_1 = '1',
	Key_2 = '2',
};

enum
{
	MAX_KEY_STATES = 256,
};

class InputManager
{
	DECLARE_SINGLE(InputManager);

public:
	~InputManager();

	void Init(HWND hwnd, HDC hdcBack);
	void Update();
	void Render(HDC hdc);

	bool IsKeyPressing(KeyType key) { return _states[key] == KeyState::Press; }
	bool IsKeyDown(KeyType key) { return _states[key] == KeyState::Down; }
	bool IsKeyUp(KeyType key) { return _states[key] == KeyState::Up; }

	void HandleChatInput(UINT message, WPARAM wParam, LPARAM lParam);

	POINT GetMousePos() const { return _mousePos; }

	void ConnectChatBoxWithPlayer(class MyPlayer* player);
	ChatBox* GetChatBox();

private:
	

private:
	HWND _hwnd;
	HDC _hdcBack = {};
	KeyState _states[MAX_KEY_STATES] = { KeyState::None, };
	POINT _mousePos = {};
	ChatBox* _chatBox = nullptr;
};

