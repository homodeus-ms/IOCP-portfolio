#pragma once
#include "UI.h"
#include <imm.h>
#pragma comment(lib, "imm32.lib")

enum ChatState
{
	CHAT_STATE_IDLE,
	CHAT_STATE_READ
};



class ChatBox : public UI
{
	using Super = UI;
	friend class UiManager;
	friend class InputManager;

private:
	ChatBox(HWND hwnd, HDC hdcBack);
	

public:

	virtual ~ChatBox();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

	void SetMyPlayer(class MyPlayer* player) { _myPlayer = player; }
	bool IsWriteReady();

	void HandleAlphabet(WCHAR c);
	void HandleHangul(LPARAM lParam, WCHAR c);
	void AddStr(const wstring& str);
	void HideChatBox() { _showChatBox = false; }

	
	
	void SetWriteReadyState(bool ready);

	void SendChat();

private:
	
	void ShowOldChatsToScreen(HDC hdc);
	void ClearText();
	float GetTextWidth();
	float GetHangulWidth(WCHAR ch);
	

	bool IsHangulMode();
	

	std::string WstringToBytes(const wstring& wstr);

private:
	
	enum 
	{ 
		MAX_COUNT_TO_SCREEN = 10,
		COMPTEXT_MAX_LENGTH = 10,    // 한글은 이걸로 충분, 다른 문자면 다를지도?
	};

	HWND _hwnd = {};
	HDC _hdc = {};

	class InputManager* _inputManager = nullptr;

	WCHAR _buffer[COMPTEXT_MAX_LENGTH];
	wstring _text;
	wstring _compText;

	float _sumTime = 0;
	float _cursorX = 0;
	float _cursorY = 0;
	float _cursorStartX = 0;

	// chats renderPos
	Pos _renderBoxPos = { 300, 550 };
	Pos _renderPos = {};
	float _renderStartY = 0;
	HFONT _font = nullptr;
	bool _showChatBox = false;

	list<wstring> _chats;

	class MyPlayer* _myPlayer = nullptr;
};

