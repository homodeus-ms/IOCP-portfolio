#pragma once

#include "Creature.h"

class Player : public Creature
{
	using Super = Creature;

public:
	Player();
	virtual ~Player();
	
	void BeginPlay() override;
	void Tick() override;
	void Render(HDC hdc) override;

	virtual bool IsPlayerType() override { return true; }
	//void SetWeapon(WeaponType type) { _weaponType = type; }
	WeaponType GetWeaponType() { return _info.weapontype(); }
	void ChangeWeapon(WeaponType type);
	

protected:

	void UpdateAnimation() override;
	void TickIdle();
	void TickMove();
	void TickAttack();

	// Collider 관련, 현재 사용 X
	void BeginCollision(Collider* src, Collider* dst) override;
	void EndCollision(Collider* src, Collider* dst) override;


	bool _transported = true;

private:
	
	Flipbook* _flipbookIdle[4] = {};
	Flipbook* _flipbookMove[4] = {};
	Flipbook* _flipbookAttack[4] = {};
	Flipbook* _flipbookArrow[4] = {};
	Flipbook* _fb_AttackEffect;
	
};

