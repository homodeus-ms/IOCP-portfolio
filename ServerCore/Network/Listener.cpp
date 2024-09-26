#include "pch.h"
#include "Listener.h"
#include "Service.h"
#include "IocpEvent.h"


Listener::Listener()
{
    _socket = ::SocketUtils::CreateSocket();
    ASSERT_CRASH(_socket != INVALID_SOCKET);
}

Listener::~Listener()
{
    for (AcceptEvent* event : _acceptEvents)
    {
        delete event;
    }

    SocketUtils::Close(_socket);
}

bool Listener::StartAccept(ServerServiceRef service)
{
    _service = service;
    if (_service == nullptr)
        return false;

    if (_socket == INVALID_SOCKET)
        return false;

    if (_service->GetIocpCore()->Register(shared_from_this()) == false)
        return false;

    if (SocketUtils::SetReuseAddress(_socket, true) == false)
        return false;
    if (SocketUtils::SetLinger(_socket, 0, 0) == false)
        return false;
    if (SocketUtils::Bind(_socket, _service->GetAddress()) == false)
        return false;
    if (SocketUtils::Listen(_socket) == false)
        return false;

    const int32 sessionCount = _service->GetMaxSessionCount();
    for (int i = 0; i < sessionCount; ++i)
    {
        AcceptEvent* acceptEvent = new AcceptEvent();
        acceptEvent->_owner = shared_from_this();
        _acceptEvents.push_back(acceptEvent);
        RegisterAccept(acceptEvent);
    }


    return true;
}


HANDLE Listener::GetHandle()
{
    return reinterpret_cast<HANDLE>(_socket);
}

void Listener::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
    ASSERT_CRASH(iocpEvent->_type == EventType::Accept);
    AcceptEvent* acceptEvent = static_cast<AcceptEvent*>(iocpEvent);
    ProcessAccept(acceptEvent);
}

void Listener::RegisterAccept(AcceptEvent* acceptEvent)
{
    SessionRef session = _service->CreateSession();

    acceptEvent->Init();
    acceptEvent->_session = session;

    DWORD bytesRecved = 0;

    // AcceptEx를 걸어준다.
    if (false == SocketUtils::AcceptEx(_socket, session->GetSocket(), session->_recvBuffer.WritePos(),
        0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &bytesRecved, static_cast<LPOVERLAPPED>(acceptEvent)))
    {
        const int32 errCode = ::WSAGetLastError();
        if (errCode != WSA_IO_PENDING)
        {
            // 일단 다시 Register를 걸어준다
            RegisterAccept(acceptEvent);
        }
    }
}

void Listener::ProcessAccept(AcceptEvent* acceptEvent)
{
    SessionRef session = acceptEvent->_session;

    if (SocketUtils::SetUpdateAcceptSocket(session->_socket, _socket) == false)
    {
        RegisterAccept(acceptEvent);
        return;
    }

    SOCKADDR_IN sockAddr = { 0, };
    int addrLen = sizeof(sockAddr);

    if (SOCKET_ERROR == ::getpeername(session->_socket, reinterpret_cast<SOCKADDR*>(&sockAddr), &addrLen))
    {
        RegisterAccept(acceptEvent);
        return;
    }

    cout << "Client Connected..." << endl;
    NetAddress address(sockAddr);
    session->SetNetAddress(address);

    wstring str = address.GetIpAddress();

    wcout << L"Ip: " << str << L" 접속" << endl;
    

    session->ProcessConnect();
    RegisterAccept(acceptEvent);

}
