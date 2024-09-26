#include "pch.h"
#include "NetworkManager.h"
#include "Service.h"
#include "ThreadManager.h"
#include "ServerSession.h"
#include "ClientPacketHandler.h"

const wstring MY_IP = L"61.72.192.178";
const wstring LOOP_BACK_IP = L"127.0.0.1";

void NetworkManager::Init()
{
	SocketUtils::Init();

	//Sleep(500);
	ClientPacketHandler::Init();


	_service = make_shared<ClientService>(
		NetAddress(LOOP_BACK_IP, 7777),
		make_shared<IocpCore>(),
		[=]() { return CreateSession(); },
		1);
	
	assert(_service->Start());

	// Ŭ������ �ϴ� ���Ͼ������

}

void NetworkManager::Update()
{
	// ���� ������� �������̹Ƿ� �񵿱��Լ����� �ٷ� ����������
	_service->GetIocpCore()->Dispatch(0);
}

ServerSessionRef NetworkManager::CreateSession()
{
	return _session = make_shared<ServerSession>();
}

void NetworkManager::SendPacket(SendBufferRef sendBuffer)
{
	if (_session)
		_session->Send(sendBuffer);
}
