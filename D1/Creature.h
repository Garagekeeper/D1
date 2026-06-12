#pragma once

#include "Define.h"
#include "PObject.h"
#include "Sprite.h"


struct FCreatureBaseStat
{
	int						AttackDamage	= 10;
	int						Hp				= 100;
	int						MaxHp			= 100;
};

// State변경시 반드시 SetState를 사용할 것
// State확인시 반드시 GetState를 사용할 것
enum class ECreatureState
{
	Idle = 0,
	Attack,
	OnAttacked,
	Dead,
	OnDead,
	Patrol,
	MoveToTarget,
	ECreatureStateLen,
};

enum class ECreatureSpriteIndex
{
	Idle = 0,
	AttackStart = 1,
	AttackEnd,
	Dead,
	OnAttacked,
};

class Creature : public PObject
{

protected:
	// Stat
	FCreatureBaseStat		Stat;
	ECreatureState			State;
	ECreatureState			PrevState = ECreatureState::Idle;
	bool					IsInSight = false;
	std::string				Name = "Creature";

public:
	//constructor
							Creature() = default;
							Creature(FCreatureBaseStat _Stat, FTransform _TransForm, FSprite _Sprites)
								: Stat(_Stat), PObject(_TransForm, _Sprites), State(ECreatureState::Idle){ }
	virtual					~Creature();

	//stat
	int						GetHp();
	int						GetAttackDamage();
	void					SetAttackDamage();

	virtual void			Attack(Creature* Target);
	virtual void			GetDamage(int Amount, Creature* From);
	virtual void			GetHeal(int Amount);
	virtual void			OnAttacked(Creature* From);
	virtual void			OnDead();

	inline ECreatureState	GetState() const { return State; }
	inline void				SetState(ECreatureState _State) { State = _State; AmountTime = 0.0; }

	inline bool				GetIsInSight() const { return IsInSight; }
	inline void				SetIsInSight(bool InValue) { IsInSight = InValue; }
	inline std::string		GetName() { return Name; }

};