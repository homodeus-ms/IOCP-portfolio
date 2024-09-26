#include "pch.h"
#include "ThreadManager.h"
#include "Service.h"
#include "NetAddress.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "SendBuffer.h"
#include "BufferWriter.h"
#include "Protocol.pb.h"
#include "ServerPacketHandler.h"
#include "GameRoom.h"

const wstring MY_IP = L"61.72.192.178";
const wstring LOOP_BACK_IP = L"127.0.0.1";

enum
{
    MAX_WORK_TIME = 64,
};

void DoGlobalWork(const ServerServiceRef& service)
{
    while (true)
    {
        LWorkEndTickCount = ::GetTickCount64() + MAX_WORK_TIME;
        service->GetIocpCore()->Dispatch(10);
        ThreadManager::HandleGlobalQueueWork();
    }
}


int main()
{
    SocketUtils::Init();
    
    ServerPacketHandler::Init();
    GRoom->Init();

    ServerServiceRef service = make_shared<ServerService>(
        NetAddress(LOOP_BACK_IP, 7777),
        make_shared<IocpCore>(),
        []() { return make_shared<GameSession>(); },
        100);

    assert(service->Start());

    for (int32 i = 0; i < 5; ++i)
    {
        GThreadManager->Launch([=]()
            {
                while (true)
                {
                    DoGlobalWork(service);
                }
            });
    }

    while (true)
    {
        GRoom->Update();
        //service->GetIocpCore()->Dispatch(0);
        this_thread::sleep_for(20ms);
    }

    GThreadManager->Join();

    SocketUtils::Clear();

    return 0;
}
