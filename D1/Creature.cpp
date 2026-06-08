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

void Creature::GetDamage(int Amount)
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
	// TODO 무적등 상태 적용
	// TODO 데미지 계산을 여기서 할지 게임 시스템에서 할지 결정
	GetDamage(From->GetAttackDamage());
}

//TODO Ondead 구현
void Creature::OnDead()
{
	
}


