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

class Creature : public PObject
{

protected:
	// Stat
	FCreatureBaseStat	Stat;
	ECreatureState		State;


public:
	//constructor
							Creature() = default;
							Creature(FCreatureBaseStat _Stat, FPos _Pos, FVec _DirVec, double PlaneSize);
	virtual					~Creature();

	//stat
	int						GetHp();
	int						GetAttackDamage();
	void					SetAttackDamage();

	virtual void			Attack();
	virtual void			GetDamage(int Amount);
	virtual void			GetHeal(int Amount);
	virtual void			OnAttacked(Creature* From);
	virtual void			OnDead();

	void					MoveTo(float NextX, float NextY);
	void					RotateTo(double NextX, double NextY, double _Theta);

	inline ECreatureState	GetState() const { return State; }
	inline void				SetState(ECreatureState _State) { State = _State; }


};