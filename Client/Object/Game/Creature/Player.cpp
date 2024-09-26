#include "pch.h"
#include "Player.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "CameraComponent.h"
#include "DevScene.h"
#include "Effect.h"
#include "Projectile.h"
#include "Arrow.h"
#include "NetworkManager.h"

Player::Player() 
	: Super()
{
	_flipbookIdle[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_IdleUp");
	_flipbookIdle[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_IdleDown");
	_flipbookIdle[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_IdleLeft");
	_flipbookIdle[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_IdleRight");

	_flipbookMove[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoveUp");
	_flipbookMove[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoveDown");
	_flipbookMove[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoveLeft");
	_flipbookMove[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_MoveRight");

	_flipbookAttack[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_AttackUp");
	_flipbookAttack[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_AttackDown");
	_flipbookAttack[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_AttackLeft");
	_flipbookAttack[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_AttackRight");

	_flipbookArrow[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_BowUp");
	_flipbookArrow[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_BowDown");
	_flipbookArrow[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_BowLeft");
	_flipbookArrow[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_BowRight");

	_fb_AttackEffect = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_Hit_Sword");


	// temp
	
}

Player::~Player()
{
}

void Player::BeginPlay()
{
	Super::BeginPlay();

	SetFlipbook(_flipbookIdle[DIR_DOWN]);
	DevScene* devScene = static_cast<DevScene*>(GET_SINGLE(SceneManager)->GetCurrScene());
	_pos = devScene->CellPosToOriginPos(GetCellPos());
	_dest = _pos;
}

void Player::Tick()
{
	Super::Tick();

}

void Player::Render(HDC hdc)
{
	Super::Render(hdc);
}

void Player::ChangeWeapon(WeaponType type)
{
	/*if (GetWeaponType() == type)
		return;*/

	_info.set_weapontype(type);

	Protocol::C_ChangeState pkt;
	*pkt.mutable_info() = _info;
	SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
	GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);

}

void Player::TickIdle()
{
	
}

void Player::TickMove()
{
	if (_dest == _pos)
	{
		if (_info.movestat().state() == MOVE)
			_info.mutable_movestat()->set_state(IDLE);
		return;
	}

	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();
	DevScene* scene = static_cast<DevScene*>(GET_SINGLE(SceneManager)->GetCurrScene());

	float distSquared = (_dest - _pos).LengthSquared();

	if (distSquared < 100.f)
	{
		_pos = _dest;

		scene->MoveObjectInMap(GetObjectId(), GetCellPos(), _destCell);

		_info.mutable_movestat()->set_x(_destCell.x);
		_info.mutable_movestat()->set_y(_destCell.y);
		
		//_transported = true;

		SetState(IDLE);
		
		return;
	}
	

	// 속도보정
	float speed = GetStat().speed();
	
	{
		switch (_info.movestat().dir())
		{
		case DIR_UP:
			_pos.y -= speed * deltaTime;
			break;
		case DIR_DOWN:
			_pos.y += speed * deltaTime;
			break;
		case DIR_LEFT:
			_pos.x -= speed * deltaTime;
			break;
		case DIR_RIGHT:
			_pos.x += speed * deltaTime;
			break;
		default:
			ASSERT_CRASH("WRONG DIR");
			break;
		}
	}
}

void Player::TickAttack()
{
	if (_flipbook == nullptr)
		return;

	if (IsAnimationEnd())
	{
		
		SetState(IDLE);
	}
}

void Player::UpdateAnimation()
{
	switch (_info.movestat().state())
	{
	case IDLE:
		SetFlipbook(_flipbookIdle[GetDir()]);
		break;
	case MOVE:
		SetFlipbook(_flipbookMove[GetDir()]);
		break;
	case SKILL:
		switch (GetWeaponType())
		{
		case WeaponType::WEAPON_TYPE_SWORD:
			SetFlipbook(_flipbookAttack[GetDir()]);
			break;
		case WeaponType::WEAPON_TYPE_BOW:
			SetFlipbook(_flipbookArrow[GetDir()]);
			break;
		}
		break;
	case CHAT:
		break;
	default:
		ASSERT_CRASH("Wrong PlayerState");
		break;
	}
}

void Player::BeginCollision(Collider* src, Collider* dst)
{
	_isOverlapped = true;
}

void Player::EndCollision(Collider* src, Collider* dst)
{
	_isOverlapped = false;
}




