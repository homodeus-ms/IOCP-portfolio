#pragma once
#include "Session.h"

class GameSession : public PacketSession
{
public:
	~GameSession() { cout << "GameSession Disconnect!" << endl; };

	virtual void OnConnect() override;
	virtual void OnDisconnect() override;
	virtual void OnRecvPacket(BYTE* buffer, int32 len) override;
	virtual void OnSend(int32 len) override;

	void SetGameRoom(GameRoomRef gameRoom) { _gameRoom = gameRoom; }
	void SetPlayer(PlayerRef& player) { _player = player; }

	GameSessionRef GetGameSessionRef() { return static_pointer_cast<GameSession>(shared_from_this()); }
	GameRoomRef GetGameRoomRef() { return _gameRoom.lock(); }
	PlayerRef GetPlayerRef() { return _player.lock(); }

public:
	weak_ptr<GameRoom> _gameRoom;
	weak_ptr<Player> _player;
};

