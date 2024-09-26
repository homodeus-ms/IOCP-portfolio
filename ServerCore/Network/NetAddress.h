#pragma once
class NetAddress sealed
{
public:
	NetAddress() = default;
	NetAddress(SOCKADDR_IN sockAddr);
	NetAddress(wstring ip, uint16 port);

	SOCKADDR_IN& GetSockAddr() { return _sockAddr; }
	wstring GetIpAddress();
	uint16 GetPort() { return ::ntohs(_sockAddr.sin_port); }

	static IN_ADDR IpToAddress(const WCHAR* ip);

private:
	SOCKADDR_IN _sockAddr = { 0, };

};

