#include <algorithm>

#include "Enemy.h"
#include "Utils.h"
#include "GameEngine.h"
#include "WorldManager.h"

void FEnemy::MoveToWithArray(FPos NextPos, WorldManager* World)
{
	FPos CurrPos = Transform.Pos;
	World->UpdateCreatureMap(CurrPos, NextPos, this);
	MoveTo(NextPos);
}

void FEnemy::Update(WorldManager* World)
{
	UpdateState();
	Rotate();
	Move(World);
}

void FEnemy::Rotate()
{
	if (State == ECreatureState::Dead) return;
	if (State == ECreatureState::Idle) return;
}

void FEnemy::Move(WorldManager* World)
{
	if (State == ECreatureState::Dead) return;
	if (State == ECreatureState::Idle) return;


	if (State == ECreatureState::Patrol)
	{
 		FPos NextPos;

		double Dx = 0.0f;
		double Dy = 0.0;

		FVec DirVec = Transform.GetDirVec();
		FPos Pos = Transform.GetPos();
		double DirSize = GetSqrLen(DirVec);
		double DeltaTime = GameEngine::GetInstance()->GetDeltaTime();

		Dx = (DirVec.DirX / DirSize) * PlayerMoveBaseSpeed * DeltaTime;
		Dy = (DirVec.DirY / DirSize) * PlayerMoveBaseSpeed * DeltaTime;

		double ColliderRadius = 1;

		// 다음 좌표
		double NextX = Pos.X + Dx;
		double NextY = Pos.Y + Dy;

		// 다음 좌표의 충돌 체크
		double ColliderEdgeX = NextX + (Dx > 0 ? ColliderRadius : -ColliderRadius);
		if (World->GetWorldMap()[(int)Pos.Y][(int)ColliderEdgeX] != static_cast<int>(Env::WALL))
		{
			Pos.X = static_cast<float>(NextX);
		}
		else 
		{
			State = ECreatureState::Idle;
		}

		double ColliderEdgeY = NextY + (Dy > 0 ? ColliderRadius : -ColliderRadius);
		if (World->GetWorldMap()[(int)ColliderEdgeY][(int)Pos.X] != static_cast<int>(Env::WALL))
		{
			Pos.Y = static_cast<float>(NextY);
		}
		else
		{
			State = ECreatureState::Idle;
		}

		// 콘솔 바운더리를 넘어가지는 못한다.
		double MapMin = ColliderRadius;
		Screen* GScreen = GameEngine::GetInstance()->GetScreen();
		double MapMaxX = GScreen->HorSize - ColliderRadius;
		double MapMaxY = GScreen->VerSize - ColliderRadius;

		if (Pos.X < ColliderRadius || Pos.X > MapMaxX || Pos.Y < ColliderRadius || Pos.Y > MapMaxY)
		{
			State = ECreatureState::Idle;
			if (Pos.X < ColliderRadius) Pos.X = static_cast<float>(ColliderRadius);
			if (Pos.X > MapMaxX) Pos.X = static_cast<float>(MapMaxX);
			if (Pos.Y < ColliderRadius) Pos.Y = static_cast<float>(ColliderRadius);
			if (Pos.Y > MapMaxY) Pos.Y = static_cast<float>(MapMaxY);
		}
		

		NextPos = { static_cast<float>(Pos.X), static_cast<float>(Pos.Y) };

		MoveToWithArray(NextPos, World);
	}
}

void FEnemy::UpdateState()
{
	
	if (State == ECreatureState::Dead) return;
	if (State == ECreatureState::OnAttacked)
	{
		if (PrevState == ECreatureState::Idle)
			AmountTime = 0.0;

		if (AmountTime >= AnimDelay)
		{
			AmountTime = 0.0;
			State = ECreatureState::Idle;
		}
		else
		{
			AmountTime += GameEngine::GetInstance()->GetDeltaTime();
		}
	}
	else if (State == ECreatureState::Idle)
	{
		// TODO 하드 코딩 고치기
		if (AmountTime >= 1)
		{
			AmountTime = 0.0;
			// 패트롤 상태가 되면 패트롤 할 방향 정하기
			if (GetRandRange() < EnemyPatrolPerCentage)
			{

				////TODO 고칠 수 있으면 고치기
				int RandIndex = GetRandInt(static_cast<int>(EDir::EDirLen) - 1);
				auto NextDirX = 0.0;
				auto NextDirY = -1.0;
				auto FinalTheta = 0.0;

				switch (RandIndex)
				{
					case 0:
						NextDirX = 0.0;
						NextDirY = -1.0;
						FinalTheta = 0.0;
						break;
					case 1:
						NextDirX = 1.0;
						NextDirY = 0.0;
						FinalTheta = 90.0;
						break;
					case 2:
						NextDirX = 0.0;
						NextDirY = 1.0;
						FinalTheta = 180.0;
						break;
					case 3:
						NextDirX = -1.0;
						NextDirY = 0.0;
						FinalTheta = 270.0;
						break;
				}

				FVec NextPlayerDIr = { NextDirX, NextDirY };
				RotateTo(NextPlayerDIr, FinalTheta);

				State = ECreatureState::Patrol;
			}
		}
		else
		{
			AmountTime += GameEngine::GetInstance()->GetDeltaTime();
		}
		
	}
	else if (State == ECreatureState::Patrol)
	{
		if (AmountTime > 1)
		{
			AmountTime = 0.0;
			if (GetRandRange() < EnemyIdlePerCentage)
			{
				State = ECreatureState::Idle;
			}
		}
		else
		{
			AmountTime += GameEngine::GetInstance()->GetDeltaTime();
		}
	}
	PrevState = State;
}

void FEnemy::GetDamage(int Amount, Creature* From)
{
	Creature::GetDamage(Amount, From);
	if (Stat.Hp <= 0)
		From->AddScore(Amount);
}





