#pragma once
#include "GameObject.h"

enum
{
	SWORD_POWER = 50,
	BOW_POWER = 18,
};

class Player : public GameObject
{
	using Super = GameObject;
public:
	Player();
	virtual ~Player() {}
	virtual void Attack(GameObjectRef target) override;
	
	void SetSession(GameSessionRef& session) { _session = session; }
	GameSessionRef GetSession() { return _session; }
	
	WeaponType GetWeaponType() { return _info.weapontype(); }
	void SetWeapon(WeaponType type);
	VectorInt GetPlayerSpawnPos();

private:
	

private:
	
	GameSessionRef _session = nullptr;
	
};

