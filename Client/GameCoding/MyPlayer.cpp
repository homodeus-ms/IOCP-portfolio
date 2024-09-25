#include "pch.h"
#include "MyPlayer.h"
#include "Component.h"
#include "CameraComponent.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "DevScene.h"
#include "NetworkManager.h"
#include "ChatBox.h"
#include "TimeManager.h"

MyPlayer::MyPlayer()
{
	_sendDelay = ::GetTickCount64() + 250;
	_moveDelay = ::GetTickCount64() + 250;
}

MyPlayer::~MyPlayer()
{
}

void MyPlayer::BeginPlay()
{
	Super::BeginPlay();

	CameraComponent* camera = new CameraComponent();
	this->AddComponent(camera);
	camera->SetOwner(this);

	_destCell = GetCellPos();
}

void MyPlayer::Tick()
{
	Super::Tick();

	/*uint64 now = ::GetTickCount64();
	if (_sendDelay > now)
		return;

	_sendDelay = now + 250;*/

	SyncToServer();
}

void MyPlayer::Render(HDC hdc)
{
	Super::Render(hdc);
}

void MyPlayer::TickIdle()
{
	TickInput();
	TryMove();
}

void MyPlayer::TickMove()
{
	Super::TickMove();
}

void MyPlayer::TickAttack()
{
	Super::TickAttack();
	
}

void MyPlayer::SyncToServer()
{
	if (_dirtyFlag == false)
		return;

	Protocol::C_ChangeState pkt;
	*pkt.mutable_info() = _info;

	SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
	ASSERT_CRASH(sendBuffer != nullptr);
	GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);

	_dirtyFlag = false;
}

void MyPlayer::TickInput()
{

	InputManager* inputManager = GET_SINGLE(InputManager);
	

	_keyPressed = true;

	if (inputManager->IsKeyPressing(Up))
	{
		SetDir(DIR_UP);
		_chatBox->HideChatBox();
	}
	else if (inputManager->IsKeyPressing(Down))
	{
		SetDir(DIR_DOWN);
		_chatBox->HideChatBox();
	}
	else if (inputManager->IsKeyPressing(Left))
	{
		SetDir(DIR_LEFT);
		_chatBox->HideChatBox();
	}
	else if (inputManager->IsKeyPressing(Right))
	{
		SetDir(DIR_RIGHT);
		_chatBox->HideChatBox();
	}
	else
	{
		_keyPressed = false;

		SetState(IDLE);
	}


	if (inputManager->IsKeyDown(Key_1))
	{
		ChangeWeapon(WeaponType::WEAPON_TYPE_SWORD);
	}
	else if (inputManager->IsKeyDown(Key_2))
	{
		ChangeWeapon(WeaponType::WEAPON_TYPE_BOW);
	}

	if (inputManager->IsKeyPressing(SpaceBar))
	{
		SetState(SKILL);

		Protocol::C_TryAttack pkt;
		
		VectorInt dest = GetCellPos() + DELTA_XY[GetDir()];
		pkt.set_attackerid(GetObjectId());
		pkt.set_destx(dest.x);
		pkt.set_desty(dest.y);

		SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
	}

	if (inputManager->IsKeyDown(Enter))
	{
		if (GET_SINGLE(SceneManager)->GetCurrScene()->IsWriteReady() == true)
		{
			_chatBox->SendChat();
		}
		else
		{
			SetState(CHAT, false);
			_chatBox->SetWriteReadyState(true);
		}
		
	}
}

void MyPlayer::TryMove()
{
	if (_keyPressed == false)
		return;

	DevScene* scene = GET_SINGLE(SceneManager)->GetDevScene();
	
	VectorInt pos = GetCellPos();
	_destCell = pos + DELTA_XY[GetDir()];

	ASSERT_CRASH(scene->GetObjectIdInMap(pos) == GetObjectId());
 	ASSERT_CRASH(_pos == _dest);
	ASSERT_CRASH(pos != _destCell);
	ASSERT_CRASH(scene->GetObjectIdInMap(pos) == GetObjectId());
	
	if (scene->CanGo(_destCell) == false)
	{
		return;
	}

	SetState(MOVE);
	//_transported = false;
	_dest = scene->CellPosToOriginPos(_destCell);

	Protocol::C_TryMove pkt;
	pkt.set_id(GetObjectId());
	pkt.set_fromx(pos.x);
	pkt.set_fromy(pos.y);
	pkt.set_destx(_destCell.x);
	pkt.set_desty(_destCell.y);

	SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
	GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
}
