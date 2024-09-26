#include "pch.h"
#include "Panel.h"

Panel::Panel()
{
}

Panel::~Panel()
{
	for (UI* ui : _uis)
		SAFE_DELETE(ui);

	_uis.clear();
}

void Panel::BeginPlay()
{
	Super::BeginPlay();
	for (UI* ui : _uis)
		ui->BeginPlay();
}

void Panel::Tick()
{
	Super::Tick();
	for (UI* ui : _uis)
		ui->Tick();
}

void Panel::Render(HDC hdc)
{
	Super::Render(hdc);
	for (UI* ui : _uis)
		ui->Render(hdc);
}

void Panel::AddUI(UI* ui)
{
	if (std::find(_uis.begin(), _uis.end(), ui) != _uis.end())
		return;
	_uis.push_back(ui);
}

void Panel::RemoveUI(UI* ui)
{
	auto it = std::remove(_uis.begin(), _uis.end(), ui);
	_uis.erase(it, _uis.end());
}
