#include "pch.h"
#include "ChatBox.h"
#include "UiManager.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "NetworkManager.h"
#include "MyPlayer.h"


ChatBox::ChatBox(HWND hwnd, HDC hdcBack)
{
	_hwnd = hwnd;
	_hdc = hdcBack;

	_pos.x = 400;
	_pos.y = 550;
	_size.x = 700;
	_size.y = 30;

	_cursorX = _pos.x - (_size.x / 2.0f) + 1;
	_cursorStartX = _cursorX;
	_cursorY = _pos.y + (_size.y / 2.0f) - 2;

	_renderPos.x = 50;
	_renderPos.y = 550;
	_renderStartY = 510;
	

	_inputManager = GET_SINGLE(InputManager)->GetInstance();

	// Font
	_font = CreateFont(
		14,                        // 글자 높이
		0,                         // 글자 너비 (0이면 기본값)
		0,                         // 문자 각도
		0,                         // 기준선 기준 각도
		500,                   // 글자 두께
		FALSE,                     // 기울임
		FALSE,                     // 밑줄
		FALSE,                     // 취소선
		DEFAULT_CHARSET,           // 문자셋
		OUT_DEFAULT_PRECIS,        // 출력 정밀도
		CLIP_DEFAULT_PRECIS,       // 클리핑 정밀도
		DEFAULT_QUALITY,           // 출력 품질
		DEFAULT_PITCH | FF_SWISS,  // 글꼴 가족과 피치
		L"나눔고딕");                 // 글꼴 이름

	::SelectObject(_hdc, _font);

}

ChatBox::~ChatBox()
{
	::DeleteObject(_font);
}

void ChatBox::BeginPlay()
{
	Super::BeginPlay();
	
	ASSERT_CRASH(_inputManager != nullptr);
}

void ChatBox::Tick()
{
	Super::Tick();

}

void ChatBox::Render(HDC hdc)
{

	Super::Render(hdc);

	if (!_myPlayer)
		return;

	::SetBkMode(hdc, TRANSPARENT);
	HFONT oldFont = (HFONT)::SelectObject(hdc, _font);
	
	if (_myPlayer->GetState() == CHAT)
	{
		Utils::DrawTransparentRect(hdc, _pos, _size.x, _size.y, RGB(255, 255, 255), 200);
		
		Utils::WriteText(hdc, Pos(_cursorStartX, _cursorY - 20), _text);
		
		Utils::WriteText(hdc, Pos(_cursorX, _cursorY - 20), _compText);
	}

	ShowOldChatsToScreen(hdc);

	::SelectObject(hdc, oldFont);

}


float ChatBox::GetTextWidth()
{
	SIZE size;
	size_t length = _text.length();
	::GetTextExtentPoint32(_hdc, _text.c_str(), _text.length(), &size);
	return static_cast<float>(size.cx);
}

float ChatBox::GetHangulWidth(WCHAR ch)
{
	SIZE size;
	GetTextExtentPoint32(_hdc, &ch, 1, &size);
	return static_cast<float>(size.cx);
}

void ChatBox::SetWriteReadyState(bool ready)
{
	_showChatBox = ready;
	
	GET_SINGLE(SceneManager)->SetWriteReadyState(ready);
}

void ChatBox::ShowOldChatsToScreen(HDC hdc)
{
	
	int32 size = static_cast<int32>(_chats.size());
	int32 offset = size;

	// Old Chat중에서 Maximum 10개까지만 화면에 보여줄 것임
	while (size > MAX_COUNT_TO_SCREEN)
	{
		_chats.pop_front();
		--size;
	}

	if (size != 0)
	{
		_renderBoxPos.y = (_renderStartY - (size * 10));
		Utils::DrawTransparentRect(hdc, _renderBoxPos, 500, (size * 20) + 10, RGB(255, 255, 255), 80);
	}
	

	for (wstring& str : _chats)
	{
		_renderPos.y = _renderStartY - (offset-- * 20);
		Utils::WriteText(hdc, _renderPos, str);
	}
}

void ChatBox::ClearText()
{
	_text.clear();
	_compText.clear();
}

void ChatBox::SendChat()
{
	SetWriteReadyState(false);

	_cursorX = _pos.x - (_size.x / 2.0) + 1;
	
	Protocol::C_Chat pkt;
	string str = WstringToBytes(_text);
	pkt.set_message(str);

	SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
	GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);


	ClearText();
}

bool ChatBox::IsWriteReady()
{
	if (!_myPlayer)
		return false;
	return _myPlayer->GetState() == ObjectState::OBJECT_STATE_TYPE_CHAT;
}

void ChatBox::HandleAlphabet(WCHAR c)
{
	if (c == VK_RETURN)
	{
		_myPlayer->SetState(IDLE, false);
		return;
	}

	if (c == VK_BACK && !_text.empty()) 
	{
		_text.pop_back();
	}
	else 
	{
		_text += c;
	}
	SIZE outSize;
	::GetTextExtentPoint32W(_hdc, _text.c_str(), _text.length(), &outSize);
	_cursorX = _cursorStartX + outSize.cx;
}
void ChatBox::HandleHangul(LPARAM lParam, WCHAR c) 
{
	if (lParam & GCS_COMPSTR) {
		HIMC hIMC = ImmGetContext(_hwnd);
		if (hIMC)
		{
			_compText.resize(COMPTEXT_MAX_LENGTH);
			LONG size = ImmGetCompositionString(hIMC, GCS_COMPSTR, &_compText[0], COMPTEXT_MAX_LENGTH);
			ASSERT_CRASH(size < COMPTEXT_MAX_LENGTH);
			ImmReleaseContext(_hwnd, hIMC);
		}
	}

	else if (lParam & GCS_RESULTSTR) {
		HIMC hIMC = ImmGetContext(_hwnd);
		if (hIMC) {
			LONG size = ImmGetCompositionString(hIMC, GCS_RESULTSTR, _buffer, sizeof(_buffer));
			_buffer[size / sizeof(WCHAR)] = 0;
			_text += _buffer;
			
			SIZE outSize;
			::GetTextExtentPoint32W(_hdc, _text.c_str(), _text.length(), &outSize);
			_cursorX = _cursorStartX + outSize.cx;

			ImmReleaseContext(_hwnd, hIMC);

			_compText.clear();
		}
	}
}

bool ChatBox::IsHangulMode() 
{
	HIMC hIMC = ImmGetContext(_hwnd);
	if (!hIMC) return false;

	DWORD conversion, sentence;
	bool isHangulMode = ImmGetConversionStatus(hIMC, &conversion, &sentence) && (conversion & IME_CMODE_HANGUL);

	ImmReleaseContext(_hwnd, hIMC);
	return isHangulMode;
}

void ChatBox::AddStr(const wstring& str)
{
	_chats.push_back(str);
	_showChatBox = true;
}



std::string ChatBox::WstringToBytes(const std::wstring& wstr) 
{
	const wchar_t* data = wstr.data();
	size_t dataSize = wstr.size() * sizeof(wchar_t);
	return std::string(reinterpret_cast<const char*>(data), dataSize);
}
