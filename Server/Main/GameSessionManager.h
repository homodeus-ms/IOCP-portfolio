#pragma once

class GameSession;

using GameSessionRef = std::shared_ptr<GameSession>;

class GameSessionManager
{
	
public:
	void AddSession(GameSessionRef session);
	void RemoveSession(GameSessionRef session);
	void Broadcast(SendBufferRef sendBuffer);


private:
	USE_LOCK;
	set<GameSessionRef> _sessions;
};

extern GameSessionManager GSessionManager;