#pragma once
#include "Player.h"
class MyPlayer : public Player
{
	using Super = Player;
public:
	MyPlayer();
	virtual ~MyPlayer();

	void BeginPlay() override;
	void Tick() override;
	void Render(HDC hdc) override;

	bool IsKeyPressed() const { return _keyPressed; }
	void SetChatBox(class ChatBox* chatbox) { _chatBox = chatbox; }

private:
	virtual void TickIdle() override;
	virtual void TickMove() override;
	virtual void TickAttack() override;

	void SyncToServer();

	void TickInput();
	void TryMove();
	

private:
	
	bool _keyPressed = false;
	uint64 _sendDelay = 0;
	uint64 _moveDelay = 0;

	class ChatBox* _chatBox = nullptr;
	
};

