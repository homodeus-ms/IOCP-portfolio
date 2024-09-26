#include "pch.h"
#include "TimeManager.h"

const static Pos TIME_INFO_POS(GWinSizeX - 200, 10);

void TimeManager::Init()
{
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(& _prevCount));
	::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(& _frequency));
}

void TimeManager::Update()
{
	uint64 now;
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(& now));
	_deltaTime = (now - _prevCount) / static_cast<float>(_frequency);
	_prevCount = now;

	++_frameCount;
	_frameTime += _deltaTime;
	if (_frameTime >= 1.f)
	{
		_fps = static_cast<uint32>(_frameCount / _frameTime);
		_frameCount = 0;
		_frameTime = 0.f;
	}
}

void TimeManager::Render(HDC hdc)
{
	const wstring timeInfo = std::format(L"FPS({0:4}), DT({1:.2f}ms)",
		_fps, _deltaTime * 1000);
	Utils::DrawTransparentRect(hdc, Pos{GWinSizeX - 140, 20}, 140, 20, RGB(250, 250, 250), 100);
	Utils::WriteText(hdc, TIME_INFO_POS, timeInfo);
}
