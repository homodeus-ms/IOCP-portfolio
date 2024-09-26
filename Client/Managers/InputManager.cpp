#include "pch.h"
#include "InputManager.h"
#include "MyPlayer.h"

InputManager::~InputManager()
{
	SAFE_DELETE(_chatBox);
}

void InputManager::Init(HWND hwnd, HDC hdcBack)
{
	_hwnd = hwnd;
	_hdcBack = hdcBack;
	GetChatBox();
}

void InputManager::Update()
{
	BYTE asciiKeys[MAX_KEY_STATES];
	if (!::GetKeyboardState(asciiKeys))
	{
		// 키상태를 가져오는데 실패시?
		CRASH("GetKeyboardState()");
	}

	for (int i = 0; i < MAX_KEY_STATES; ++i)
	{
		// 상위비트가 1이면 KeyDown상태
		if (asciiKeys[i] & 0x80)
		{
			if (_states[i] == KeyState::Press || _states[i] == KeyState::Down)
			{
				_states[i] = KeyState::Press;
			}
			else
			{
				_states[i] = KeyState::Down;
			}
		}
		else
		{
			if (_states[i] == KeyState::Press || _states[i] == KeyState::Down)
			{
				_states[i] = KeyState::Up;
			}
			else
			{
				_states[i] = KeyState::None;
			}
		}
	}

	::GetCursorPos(&_mousePos);
	::ScreenToClient(_hwnd, &_mousePos);
}

ChatBox* InputManager::GetChatBox()
{
	if (_chatBox == nullptr)
	{
		_chatBox = new ChatBox(_hwnd, _hdcBack);
	}

	return _chatBox;
}

void InputManager::ConnectChatBoxWithPlayer(MyPlayer* player)
{
	if (_chatBox)
	{
		_chatBox->SetMyPlayer(player);
		player->SetChatBox(_chatBox);
	}
}

void InputManager::Render(HDC hdc)
{
	_chatBox->Render(hdc);
}

void InputManager::HandleChatInput(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CHAR:
	{
		if (!_chatBox || _chatBox->IsWriteReady() == false)
			break;

		_chatBox->HandleAlphabet(static_cast<WCHAR>(wParam));
		break;
	}

	case WM_IME_COMPOSITION:
	{
		if (!_chatBox || _chatBox->IsWriteReady() == false)
			break;

		_chatBox->HandleHangul(lParam, static_cast<WCHAR>(wParam));
		break;
	}
	}
}
