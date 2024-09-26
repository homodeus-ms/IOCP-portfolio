#include "pch.h"
#include "Service.h"

Service::Service(ServiceType type, NetAddress address, IocpCoreRef core, SessionFactory factory, uint32 maxSessionCount)
	: _type(type), _address(address), _iocpCore(core), _factory(factory), _maxSessionCount(maxSessionCount)
{
}

Service::~Service()
{
	_sessions.clear();
}

bool Service::CanStart()
{
	return _factory != nullptr && _iocpCore != nullptr;
}

bool Service::AddSession(SessionRef session)
{
	WRITE_LOCK;
	if (_sessionCount++ == _maxSessionCount)
		return false;
	_sessions.insert(session);
	return true;
}

bool Service::ReleaseSession(SessionRef session)
{
	WRITE_LOCK;
	if (_sessionCount-- == 0)
		return false;
	_sessions.erase(session);
	return true;
}

SessionRef Service::CreateSession()
{
	SessionRef session = _factory();
	session->SetService(shared_from_this());
	session->InitSession();

	if (GetIocpCore()->Register(session) == false)
	{
		ASSERT_CRASH(false);
		return nullptr;
	}

	return session;
}

void Service::CloseService()
{
	// TODO..
}



// Client Service


ClientService::ClientService(NetAddress address, IocpCoreRef core, SessionFactory factory, uint32 maxSessionCount)
	: Service(ServiceType::Client, address, core, factory, maxSessionCount)
{
}

ClientService::~ClientService()
{
}

bool ClientService::Start()
{
	if (CanStart() == false)
		return false;

	const uint32 sessionCount = GetMaxSessionCount();
	for (uint32 i = 0; i < sessionCount; ++i)
	{
		SessionRef session = CreateSession();
		if (session->Connect() == false)
			return false;
	}

	return true;
}


// Server Service

ServerService::ServerService(NetAddress address, IocpCoreRef core, SessionFactory factory, uint32 maxSessionCount)
	: Service(ServiceType::Server, address, core, factory, maxSessionCount)
{
	_listener = make_shared<Listener>();
	ASSERT_CRASH(_listener != nullptr);
}

ServerService::~ServerService()
{
}

bool ServerService::Start()
{
	if (CanStart() == false)
		return false;

	_listener = make_shared<Listener>();

	ServerServiceRef service = static_pointer_cast<ServerService>(shared_from_this());
	if (_listener->StartAccept(service) == false)
		return false;

	return true;
}

void ServerService::CloseService()
{
	Service::CloseService();
}
