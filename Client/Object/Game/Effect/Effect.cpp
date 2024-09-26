#include "pch.h"
#include "Effect.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Scene.h"

Effect::Effect() 
	: Super(CobjectType::Effect)
{
	SetLayer(LayerType::LAYER_EFFECT);
	_flipbook = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_Hit_Sword");
}

Effect::~Effect()
{
}

void Effect::BeginPlay()
{
	Super::BeginPlay();
}

void Effect::Tick()
{
	Super::Tick();

	if (IsAnimationEnd())
	{
		GET_SINGLE(SceneManager)->GetCurrScene()->RemoveActor(this);
	}
}

void Effect::Render(HDC hdc)
{
	
	Super::Render(hdc);
}
