#include "Creature.h"

Creature::~Creature()
{

}


int Creature::GetHp()
{
	return Stat.Hp;
}

int Creature::GetAttackDamage()
{
	return Stat.AttackDamage;
}

void Creature::SetAttackDamage()
{}

void Creature::GetHeal(int Amount)
{
	Stat.Hp += Amount;
	if (Stat.Hp >= Stat.MaxHp) Stat.Hp = Stat.MaxHp;
}

void Creature::GetDamage(int Amount, Creature* From)
{
  	Stat.Hp -= Amount;
	if (Stat.Hp <= 0)
	{
		Stat.Hp = 0;
		OnDead();
	}
}

void Creature::Attack(Creature* Target)
{
	Target->OnAttacked(this);
}

void Creature::OnAttacked(Creature* From)
{
	// 죽음과 관련한 상태가 아닐때만 데미지 적용
	ECreatureState CurrState = GetState();
	if (CurrState != ECreatureState::OnDead && CurrState != ECreatureState::Dead)
	{
		SetState(ECreatureState::OnAttacked);
		GetDamage(From->GetAttackDamage(), From);
	}
}

//TODO Ondead 구현
void Creature::OnDead()
{
	SetState(ECreatureState::OnDead);
}


