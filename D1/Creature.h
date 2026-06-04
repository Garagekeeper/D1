#pragma once

#include "Define.h"
#include "Sprite.h"



struct FPos
{
	float X				= 0;
	float Y				= 0;
};

struct FVec
{
	double DirX			= 0;
	double DirY			= -1;

	double GEtSqrLen()
	{
		return (DirX * DirX) + (DirY * DirY);
	}
};

struct FCreatureBaseStat
{
	int AttackDamage	= 10;
	int Hp				= 100;
	int MaxHp			= 100;
};

class Creature
{

private:
	// Stat
	FCreatureBaseStat	Stat;
	ECreatureState		State;

	// Pos and Vector
	FPos				Pos;
	FVec				CameraDirVec;
	FVec				DirVec;
	FVec				RightVec;
	double				PlaneSize;
	double				Theta;


public:
	//constructor
						Creature() = default;
						Creature(FCreatureBaseStat _Stat, FPos _Pos, FVec _DirVec, double PlaneSize);
	virtual				~Creature();

	//stat
	int					GetHp();
	int					GetAttackDamage();
	void				SetAttackDamage();

	// Pos and Vector
	FPos				GetPos();
	FVec				GetDirVec();
	FVec				GetCameraDirVec();
	FVec				GetRightDirVec();
	double				GetTheta();

	virtual void		Attack();
	virtual void		GetDamage(int Amount);
	virtual void		GetHeal(int Amount);
	virtual void		OnAttacked(Creature* From);
	virtual void		OnDead();

	void				MoveTo(float NextX, float NextY);
	void				RotateTo(double NextX, double NextY, double _Theta);

	inline ECreatureState GetState() const { return State; }
	inline void SetState(ECreatureState _State) { State = _State; }


};