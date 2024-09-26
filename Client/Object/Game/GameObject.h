#pragma once
#include "FlipbookActor.h"

enum class CobjectType
{
	Creature,
	Projectile,
	Effect,
};

class GameObject : public FlipbookActor
{
	using Super = FlipbookActor;
public:
	GameObject(CobjectType type) : _objectType(type) {}
	virtual ~GameObject() {}

	virtual void BeginPlay();
	virtual void Tick();
	virtual void Render(HDC hdc);

	virtual bool IsPlayerType() { return false; }
	

	void SetCellPos(VectorInt pos);
	void SetDestPos(VectorInt dest);
	CobjectType GetCobjectType() const { return _objectType; }

	// Info¿« Getter
	uint64 GetObjectId() const { return _info.objectid(); }
	Protocol::OBJECT_TYPE GetObjectType();
	ObjectState GetState() const { return _info.movestat().state(); }
	Dir GetDir() const { return _info.movestat().dir(); }
	string GetName() const { return _info.name(); }
	Stat GetStat() const { return _info.stat(); }
	VectorInt GetCellPos() override { return { _info.movestat().x(), _info.movestat().y() }; }
	VectorInt& GetDestCellPos() { return _destCell; }

	Protocol::ObjectInfo& GetInfo() { return _info; }
	void SetInfo(const Protocol::ObjectInfo& info) { _info = info; }

	void TryRemoveObject(uint64 objectId);
	//void OnAttacked(GameObject* attacker, int32 damage);

	

public:
	virtual void TickIdle();
	virtual void TickMove();
	virtual void TickAttack();
	
	VectorInt GetFrontCell();
	void SetState(ObjectState state, bool sendToServer = true);
	void SetDir(Dir dir, bool sendToServer = true);
	
	
	virtual void UpdateAnimation() {};

	
protected:
	const VectorInt DIR_OFFSET[4] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

	CobjectType _objectType;
	VectorInt _oldCell;
	VectorInt _destCell = {};
	

	bool _dirtyFlag = false;
	
	Protocol::ObjectInfo _info;
};

