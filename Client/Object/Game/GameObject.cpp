#include "pch.h"
#include "GameObject.h"
#include "DevScene.h"
#include "SceneManager.h"
#include "Effect.h"
#include "NetworkManager.h"
#include "UiManager.h"


void GameObject::BeginPlay()
{
	Super::BeginPlay();
	SetState(MOVE);
	SetState(IDLE);
}

void GameObject::Tick()
{
	//_dirtyFlag = false;

	Super::Tick();

	switch (_info.movestat().state())
	{
	case IDLE:
		TickIdle();
		break;
	case MOVE:
		TickMove();
		break;
	case SKILL:
		TickAttack();
		break;
	}
}

void GameObject::Render(HDC hdc)
{
	Super::Render(hdc);
}



void GameObject::SetCellPos(VectorInt pos)
{
	_info.mutable_movestat()->set_x(pos.x);
	_info.mutable_movestat()->set_y(pos.y);
	DevScene* devScene = static_cast<DevScene*>(GET_SINGLE(SceneManager)->GetCurrScene());
	_pos = devScene->CellPosToOriginPos(pos);
}

void GameObject::SetDestPos(VectorInt dest)
{
	DevScene* devScene = static_cast<DevScene*>(GET_SINGLE(SceneManager)->GetCurrScene());
	_dest = devScene->CellPosToOriginPos(dest);
}


void GameObject::TryRemoveObject(uint64 objectId)
{
	Protocol::C_TryRemoveObject pkt;
	pkt.set_id(objectId);
	SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
	GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
}

void GameObject::TickIdle()
{
}

void GameObject::TickMove()
{
}

void GameObject::TickAttack()
{
}

Protocol::OBJECT_TYPE GameObject::GetObjectType()
{
	uint64 id = _info.objectid();
	id >>= 32;
	return static_cast<ObjectType>(id);
}

VectorInt GameObject::GetFrontCell()
{
	return VectorInt(_info.movestat().x(), _info.movestat().y()) + DIR_OFFSET[_info.movestat().dir()];
}


void GameObject::SetState(ObjectState state, bool sendToServer)
{
	if (_info.movestat().state() == state)
		return;
	_info.mutable_movestat()->set_state(state);
	UpdateAnimation();

	if (sendToServer)
		_dirtyFlag = true;
}

void GameObject::SetDir(Dir dir, bool sendToServer)
{
	//GET_SINGLE(UiManager)->GetChatBox()->HideChatBox();

	if (GetDir() == dir)
		return;

	_info.mutable_movestat()->set_dir(dir);
	UpdateAnimation();

	if (sendToServer)
		_dirtyFlag = true;
}


