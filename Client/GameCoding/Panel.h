#pragma once
#include "UI.h"

class Sprite;
class Button;

class Panel : public UI
{
	using Super = UI;
public:
	Panel();
	virtual ~Panel();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

	void AddUI(UI* ui);
	void RemoveUI(UI* ui);

private:
	vector<UI*> _uis = {};

};

