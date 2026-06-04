#include "Creature.h"

Creature::Creature(FCreatureBaseStat _Stat, FPos _Pos, FVec _DirVec, double PlaneSize)
	: PObject(_Pos, _DirVec, PlaneSize, 0.0), Stat(_Stat), State(ECreatureState::Idle)
{

}

Creature::~Creature()
{

}


int Creature::GetHp()
{
	return Stat.Hp;
}

int Creature::GetAttackDamage()
{
	return 0;
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

//TODO Attack 구현
void Creature::Attack()
{
	
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


void Creature::MoveTo(float NextX, float NextY)
{
	Pos.X = NextX;
	Pos.Y = NextY;
}

void Creature::RotateTo(double NextX, double NextY, double _Theta)
{
	//TODO Sqrt 안쓰고 해보기
	auto Len = sqrt(NextX * NextX + NextY * NextY);

	// 방향벡터 수정
	DirVec.DirX = NextX / Len;
	DirVec.DirY = NextY / Len;

	// 오른쪽을 가리키는 벡터 수정
	RightVec.DirX = -DirVec.DirY;
	RightVec.DirY = DirVec.DirX;

	// 카메라벡터 수정 (사실 RightVec이랑 똑같은데) 길이만 다름
	CameraDirVec.DirX = RightVec.DirX * PlaneSize;
	CameraDirVec.DirY = RightVec.DirY * PlaneSize;

	// 회전각 기록
	Theta += _Theta;

}

