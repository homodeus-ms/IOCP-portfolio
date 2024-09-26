#include "pch.h"
#include "Session.h"
#include "Service.h"

Session::Session()
{
    _socket = SocketUtils::CreateSocket();
    ASSERT_CRASH(_socket != INVALID_SOCKET);
}

Session::~Session()
{
    // Event Ref들 확인
    if (_connectEvent._owner != nullptr)
        ClearRefs();

    _pendingSendBuffers.clear();
    SocketUtils::Close(_socket);

    cout << "Session 클래스의 소멸자!" << endl;
}

void Session::InitSession()
{
    _connectEvent.Init();
    _disconnectEvent.Init();
    _recvEvent.Init();
    _sendEvent.Init();

    _connectEvent._owner = shared_from_this();
    _disconnectEvent._owner = shared_from_this();
    _recvEvent._owner = shared_from_this();
    _sendEvent._owner = shared_from_this();
}

void Session::ClearRefs()
{
    _connectEvent._owner = nullptr;
    _disconnectEvent._owner = nullptr;
    _recvEvent._owner = nullptr;
    _sendEvent._owner = nullptr;
}

bool Session::Connect()
{
    if (IsConnected())
        return false;

    if (GetService()->GetServiceType() != ServiceType::Client)
        return false;

    return RegisterConnect();
}

void Session::Disconnect(const WCHAR* cause)
{
    if (_connected.exchange(false) == false)
        return;

    wcout << "Disconnected by " << cause << endl;
    

    RegisterDisconnect();
}

void Session::Send(SendBufferRef sendBuffer)
{
    if (IsConnected() == false)
        return;

    bool goRegister = false;
    {
        WRITE_LOCK;
        _sendQueue.push(sendBuffer);
        if (_sendRegistered.exchange(true) == false)
            goRegister = true;
    }

    if (goRegister)
        RegisterSend();
}

HANDLE Session::GetHandle()
{
    return reinterpret_cast<HANDLE>(_socket);
}

void Session::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
    switch (iocpEvent->_type)
    {
    case EventType::Connect:
        ProcessConnect();
        break;
    case EventType::Disconnect:
        ProcessDisconnect();
        break;
    case EventType::Recv:
        ProcessRecv(numOfBytes);
        break;
    case EventType::Send:
        ProcessSend(numOfBytes);
        break;
    default:
        break;

    }
}

bool Session::RegisterConnect()
{
    if (SocketUtils::SetReuseAddress(_socket, true) == false)
        return false;
    if (SocketUtils::BindAnyAddress(_socket, 0) == false)
        return false;

    SOCKADDR_IN clientAddr = { 0, };
    DWORD numOfBytes = 0;

    clientAddr = GetService()->GetAddress().GetSockAddr();

    if (false == SocketUtils::ConnectEx(_socket,
        reinterpret_cast<SOCKADDR*>(&clientAddr), sizeof(clientAddr), NULL, 0,
        &numOfBytes, &_connectEvent))
    {
        const int32 errCode = ::WSAGetLastError();
        if (errCode != WSA_IO_PENDING)
        {
            HandleError(errCode);
            return false;
        }
    }

    return true;
}

bool Session::RegisterDisconnect()
{
    if (false == SocketUtils::DisconnectEx(_socket, &_disconnectEvent, TF_REUSE_SOCKET, 0))
    {
        const int32 errCode = ::WSAGetLastError();
        if (errCode != WSA_IO_PENDING)
        {
            HandleError(errCode);
            return false;
        }
    }
    return true;
}

void Session::RegisterRecv()
{
    if (IsConnected() == false)
        return;

    WSABUF wsaBuf;
    wsaBuf.buf = reinterpret_cast<char*>(_recvBuffer.WritePos());
    wsaBuf.len = static_cast<ULONG>(_recvBuffer.RemainSize());
    DWORD numOfBytes = 0;
    DWORD flags = 0;

    if (SOCKET_ERROR == ::WSARecv(_socket, &wsaBuf, 1, &numOfBytes, &flags, &_recvEvent, nullptr))
    {
        const int32 errCode = ::WSAGetLastError();
        if (errCode != WSA_IO_PENDING)
        {
            HandleError(errCode);
        }
    }
}

// scatter - gatter
void Session::RegisterSend()
{
    if (IsConnected() == false)
        return;

    DWORD processSize = 0;

    // sendQueue에 있는 녀석들 꺼내오기
    {
        int writeCount = 0;
        WRITE_LOCK;
        processSize = _sendQueue.size();
        _pendingSendBuffers.clear();
        _pendingSendBuffers.reserve(processSize);

        while (!_sendQueue.empty())
        {
            SendBufferRef sendBuffer = _sendQueue.front();
            writeCount += sendBuffer->DataSize();

            // 한 번에 처리할 데이터 사이즈를 제어하려면 여기에서

            _sendQueue.pop();
            _pendingSendBuffers.push_back(sendBuffer);
        }
    }

    ASSERT_CRASH(processSize != 0);
    

    vector<WSABUF> wsaBufs;
    wsaBufs.reserve(processSize);

    for (SendBufferRef& buffer : _pendingSendBuffers)
    {
        WSABUF wsaBuf;
        wsaBuf.buf = reinterpret_cast<char*>(buffer->Buffer());
        wsaBuf.len = static_cast<ULONG>(buffer->DataSize());
        wsaBufs.push_back(wsaBuf);
    }

    ASSERT_CRASH(wsaBufs.size() == _pendingSendBuffers.size());

    DWORD numOfBytes = 0;

    if (SOCKET_ERROR == ::WSASend(_socket, wsaBufs.data(), processSize, &numOfBytes, 0, &_sendEvent, nullptr))
    {
        const int32 errCode = ::WSAGetLastError();
        if (errCode != WSA_IO_PENDING)
        {
            HandleError(errCode);
            _pendingSendBuffers.clear();
            _sendRegistered.store(false);
        }
    }
}

void Session::ProcessConnect()
{
    _connected.store(true);

    GetService()->AddSession(GetSessionRef());

    OnConnect();

    RegisterRecv();
}

void Session::ProcessDisconnect()
{
    ClearRefs();

    OnDisconnect();

    GetService()->ReleaseSession(GetSessionRef());
}

void Session::ProcessRecv(int32 numOfBytes)
{
    if (numOfBytes == 0)
    {
        Disconnect(L"RECV 0");
        return;
    }

    if (_recvBuffer.UpdateWritePos(numOfBytes) == false)
    {
        Disconnect(L"Write Overflow");
        return;
    }

    const uint32 dataSize = _recvBuffer.DataSize();

    int32 processed = OnRecv(_recvBuffer.ReadPos(), dataSize);

    if (processed < 0 || processed > dataSize || _recvBuffer.UpdateReadPos(processed) == false)
    {
        Disconnect(L"Read Overflow");
        return;
    }

    _recvBuffer.CleanUpPos();

    RegisterRecv();
}

void Session::ProcessSend(int32 numOfBytes)
{
    if (numOfBytes == 0)
    {
        Disconnect(L"SEND 0");
        return;
    }

    OnSend(numOfBytes);

    bool goRegister = false;
    {
        WRITE_LOCK;
        if (!_sendQueue.empty())
        {
            goRegister = true;
        }
    }
    if (goRegister)
        RegisterSend();
    else
        _sendRegistered.store(false);
}

void Session::HandleError(int32 errCode)
{
    switch (errCode)
    {
    case WSAECONNRESET:
        // Intentional fall through
    case WSAECONNABORTED:
        Disconnect(L"HANDLE ERROR PROBLEM");
        break;
    default:
        // 일단 로그만 찍자
        cout << "HANDLE ERROR : " << errCode << endl;
    }
}



/* -------------------------
        Packet Session
----------------------------*/


PacketSession::PacketSession()
{
}

PacketSession::~PacketSession()
{
}

// [size(2) | id(2)] [ Data...  ] [size(2) | id(2)] [ Data...  ] ...
// Header의 사이즈를 보고 그 크기 이상의 데이터가 들어왔을 때에만
// OnRecvPacket() 함수를 호출함(패킷을 파싱)
int32 PacketSession::OnRecv(BYTE* buffer, int32 len)
{
    int32 processed = 0;

    while (true)
    {
        int32 dataSize = len - processed;

        if (dataSize < sizeof(PacketHeader))
            break;

        const int32 packetSize = *(reinterpret_cast<uint16*>(&buffer[processed]));

        if (dataSize < packetSize)
            break;

        OnRecvPacket(&buffer[processed], packetSize);

        processed += packetSize;
    }

    return processed;
}
