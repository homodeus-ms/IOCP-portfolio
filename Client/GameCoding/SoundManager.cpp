#include "pch.h"
#include "SoundManager.h"
#include "ResourceManager.h"
#include "Sound.h"

SoundManager::~SoundManager()
{
	if (_soundDevice)
		_soundDevice->Release();
}

void SoundManager::Init(HWND hwnd)
{
	// 사운드 디바이스를 생성함
	if (FAILED(::DirectSoundCreate(NULL, &_soundDevice, NULL)))
	{
		::MessageBox(NULL, L"Sound Device Failed", L"SYSTEM ERROR", MB_OK);
		return;
	}
	// 사운드 디바이스 협조레벨(?) 설정?
	if (FAILED(_soundDevice->SetCooperativeLevel(hwnd, DSSCL_PRIORITY)))
	{
		::MessageBox(NULL, L"SetCooperativeLevel Failed", L"SYSTEM ERROR", MB_OK);
		return;
	}
}

void SoundManager::Play(const wstring& key, bool loop)
{
	Sound* sound = GET_SINGLE(ResourceManager)->GetSound(key);
	if (sound == nullptr)
	{
		::MessageBox(NULL, L"sound* == nullptr", L"", MB_OK);
		return;
	}

	sound->Play(loop);
}
