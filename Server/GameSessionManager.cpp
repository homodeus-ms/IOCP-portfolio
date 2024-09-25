#include "pch.h"
#include "GameSessionManager.h"
#include "GameSession.h"


GameSessionManager GSessionManager;

void GameSessionManager::AddSession(GameSessionRef session)
{
	WRITE_LOCK;
	_sessions.insert(session);
}

void GameSessionManager::RemoveSession(GameSessionRef session)
{
	WRITE_LOCK;
	_sessions.erase(session);
}

void GameSessionManager::Broadcast(SendBufferRef sendBuffer)
{
	WRITE_LOCK;
	for (const GameSessionRef& session : _sessions)
	{
		session->Send(sendBuffer);
	}
}
