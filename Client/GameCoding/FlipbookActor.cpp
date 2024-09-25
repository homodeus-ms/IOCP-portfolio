#include "pch.h"
#include "FlipbookActor.h"
#include "Texture.h"
#include "TimeManager.h"
#include "SceneManager.h"



FlipbookActor::~FlipbookActor()
{
}

void FlipbookActor::BeginPlay()
{
	Super::BeginPlay();
}

void FlipbookActor::Tick()
{
	Super::Tick();

	if (_flipbook == nullptr)
		return;

	const FlipbookInfo& info = _flipbook->GetFlipbookInfo();

	if (info.loop == false && info.end == _idx)
	{
		return;
	}

	// 1. deltaTime을 구해온다   2. _sumTime에 deltaTime들을 더한다
	// 3. frameDuration을 구한다 (= wholeDuration / frameCount)
	// 4. sumTime이 frameDuration이상이되면 idx++
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();
	_sumTime += deltaTime;

	uint32 frameCount = info.end - info.start + 1;
	float frameDuration = info.duration / frameCount;

	if (_sumTime >= frameDuration)
	{
		_sumTime = 0;

		_idx = (_idx + 1) % frameCount;
	}
}

void FlipbookActor::Render(HDC hdc)
{
	Super::Render(hdc);
	
	if (!_flipbook)
		return;

	const FlipbookInfo& info = _flipbook->GetFlipbookInfo();
	VectorInt size = info.size;

	Pos cameraPos = GET_SINGLE(SceneManager)->GetCameraPos();
	
	::TransparentBlt(hdc,
		/*_pos.x - size.x / 2,
		_pos.y - size.y / 2,*/
		(int32)_pos.x - size.x / 2 - ((int32)cameraPos.x - GWinSizeX / 2),
		(int32)_pos.y - size.y / 2 - ((int32)cameraPos.y - GWinSizeY / 2),
		info.size.x, info.size.y,
		info.texture->GetDc(),
		info.size.x * (info.start + _idx), info.size.y * info.line,
		info.size.x, info.size.y,
		info.texture->GetTransparent());
}

void FlipbookActor::SetFlipbook(Flipbook* fb)
{
	ASSERT_CRASH(fb != nullptr);
	if (_flipbook == fb)
		return;
	_flipbook = fb;
	Reset();
}

bool FlipbookActor::IsAnimationEnd()
{
	if (_flipbook == nullptr)
		return true;

	const FlipbookInfo& info = _flipbook->GetFlipbookInfo();
	if (info.loop == false && _idx == info.end)
		return true;

	return false;
}



void FlipbookActor::Reset()
{
	_sumTime = 0.f;
	_idx = 0;
}
