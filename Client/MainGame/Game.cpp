#include "pch.h"
#include "Game.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "CollisionManager.h"
#include "SoundManager.h"
#include "NetworkManager.h"
#include "UiManager.h"



Game::Game()
{
}

Game::~Game()
{
}

void Game::Init(HWND hwnd)
{
	_hwnd = hwnd;
	_hdc = ::GetDC(_hwnd);

	::GetClientRect(_hwnd, &_rect);
	_hdcBack = ::CreateCompatibleDC(_hdc);
	
	_bmpBack = (HBITMAP)::CreateCompatibleBitmap(_hdc, _rect.right, _rect.bottom);
	HBITMAP prev = (HBITMAP)::SelectObject(_hdcBack, _bmpBack);
	::DeleteObject(prev);

	GET_SINGLE(TimeManager)->Init();
	GET_SINGLE(InputManager)->Init(hwnd, _hdcBack);
	GET_SINGLE(ResourceManager)->Init(_hwnd, fs::path(L"C:\\kiwi\\GameCoding\\Server\\Client\\Resources"));
	//GET_SINGLE(ResourceManager)->Init(_hwnd, fs::path(L"./Resources"));

	GET_SINGLE(UiManager)->Init(hwnd);
	GET_SINGLE(CollisionManager)->Init();
	GET_SINGLE(SoundManager)->Init(hwnd);
	GET_SINGLE(SceneManager)->Init(hwnd);
	GET_SINGLE(SceneManager)->ChangeScene(SceneType::DevScene);

	GET_SINGLE(NetworkManager)->Init();
	
	
}

void Game::Update()
{
	GET_SINGLE(TimeManager)->Update();
	GET_SINGLE(InputManager)->Update();
	GET_SINGLE(SceneManager)->Update();
	GET_SINGLE(NetworkManager)->Update();
	
}

void Game::Render()
{
	
	GET_SINGLE(SceneManager)->Render(_hdcBack);
	GET_SINGLE(InputManager)->Render(_hdcBack);    // chat기능을 InputManager쪽에 넣어봤음
	GET_SINGLE(TimeManager)->Render(_hdcBack);
	
	

	// Double Buffering
	::BitBlt(_hdc, 0, 0, GWinSizeX, GWinSizeY, _hdcBack, 0, 0, SRCCOPY);
	::PatBlt(_hdcBack, 0, 0, GWinSizeX, GWinSizeY, WHITENESS);
}

void Game::HandleChatInput(UINT message, WPARAM wParam, LPARAM lParam)
{
	GET_SINGLE(InputManager)->HandleChatInput(message, wParam, lParam);
}
