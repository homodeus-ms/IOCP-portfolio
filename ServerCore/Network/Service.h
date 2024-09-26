#pragma once

#include <functional>
#include "NetAddress.h"
#include "IocpCore.h"
#include "Listener.h"

enum class ServiceType : uint8
{
	Client,
	Server,
};

using SessionFactory = function<SessionRef(void)>;

class Service : public enable_shared_from_this<Service>
{
public:
	Service(ServiceType type, NetAddress address, IocpCoreRef core, SessionFactory factory, uint32 maxSessionCount = 1);
	virtual ~Service();

	bool CanStart();
	virtual bool Start() abstract;

	// getter | setter
	ServiceType GetServiceType() const { return _type; }
	NetAddress GetAddress() const { return _address; }
	IocpCoreRef& GetIocpCore() { return _iocpCore; }
	SessionFactory GetSessionFactory() const { return _factory; }
	uint32 GetSessionCount() const { return _sessionCount; }
	uint32 GetMaxSessionCount() const { return _maxSessionCount; }

	bool AddSession(SessionRef session);
	bool ReleaseSession(SessionRef session);

	SessionRef CreateSession();
	virtual void CloseService();  // For Server Service override


protected:

	USE_LOCK;

	ServiceType _type;
	NetAddress _address;
	IocpCoreRef _iocpCore = nullptr;
	SessionFactory _factory = nullptr;
	uint32 _sessionCount = 0;
	uint32 _maxSessionCount = 0;

	set<SessionRef> _sessions;
};

class ClientService : public Service
{
public:
	ClientService(NetAddress address, IocpCoreRef core, SessionFactory factory, uint32 maxSessionCount = 1);
	virtual ~ClientService();

	virtual bool Start() override;

private:

};

class ServerService : public Service
{
public:
	ServerService(NetAddress address, IocpCoreRef core, SessionFactory factory, uint32 maxSessionCount = 1);
	virtual ~ServerService();

	virtual bool Start() override;

	virtual void CloseService() override;

private:
	ListenerRef _listener = nullptr;
};

