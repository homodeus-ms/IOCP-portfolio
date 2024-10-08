#include "pch.h"
#include "TestPanel.h"
#include "ResourceManager.h"
#include "Button.h"
#include "Sprite.h"

TestPanel::TestPanel()
{
	{
		Button* ui = new Button();
		//ui->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Start"), BS_Default);
		ui->SetPos({ 400, 200 });
		ui->SetSize({ 650, 300 });
		AddUI(ui);
	}
	{
		Button* ui = new Button();
		ui->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Start_Off"), BS_DEFAULT);
		ui->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Start_On"), BS_CLICKED);
		ui->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Start_On"), BS_PRESSED);
		ui->SetPos({ 200, 200 });

		//ui->_onClickMemFunc = &TestPanel::OnClickStartButton;
		ui->AddOnClickDelegate(this, &TestPanel::OnClickStartButton);
		AddUI(ui);
	}
	{
		Button* ui = new Button();
		ui->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Edit_Off"), BS_DEFAULT);
		ui->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Edit_On"), BS_CLICKED);
		ui->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Edit_On"), BS_PRESSED);
		ui->SetPos({ 400, 200 });
		ui->AddOnClickDelegate(this, &TestPanel::OnClickEditButton);
		AddUI(ui);
	}
	{
		Button* ui = new Button();
		ui->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_Off"), BS_DEFAULT);
		ui->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_On"), BS_CLICKED);
		ui->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Exit_On"), BS_PRESSED);
		ui->SetPos({ 600, 200 });
		ui->AddOnClickDelegate(this, &TestPanel::OnClickExitButton);
		AddUI(ui);
	}
}

TestPanel::~TestPanel()
{
}

void TestPanel::BeginPlay()
{
	Super::BeginPlay();
}

void TestPanel::Tick()
{
	Super::Tick();
}

void TestPanel::Render(HDC hdc)
{
	Super::Render(hdc);
}

void TestPanel::OnClickStartButton()
{
}

void TestPanel::OnClickEditButton()
{
}

void TestPanel::OnClickExitButton()
{
}
