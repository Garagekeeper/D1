#pragma once

#include "Define.h"
#include "PObject.h"
#include "Sprite.h"


struct FCreatureBaseStat
{
	int AttackDamage	= 10;
	int Hp				= 100;
	int MaxHp			= 100;
};

// Move?
enum class ECreatureState
{
	Idle = 0,
	Attack,
	GetAttacked,
	ECreatureStateLen,
};

class Creature : public PObject
{

protected:
	// Stat
	FCreatureBaseStat	Stat;
	ECreatureState		State;


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
	virtual void			GetDamage(int Amount);
	virtual void			GetHeal(int Amount);
	virtual void			OnAttacked(Creature* From);
	virtual void			OnDead();

	inline ECreatureState	GetState() const { return State; }
	inline void				SetState(ECreatureState _State) { State = _State; }


};