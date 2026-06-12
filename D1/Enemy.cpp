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
	UpdateState(World);
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
	ECreatureState CurrState = GetState();
	if (CurrState == ECreatureState::Dead) return;
	if (CurrState == ECreatureState::Idle) return;


	if (CurrState == ECreatureState::Patrol)
	{
		FPos NextPos;

		double Dx = 0.0f;
		double Dy = 0.0;

		FVec DirVec = Transform.GetDirVec();
		FPos Pos = Transform.GetPos();
		double DirSize = GetSqrLen(DirVec);
		double DeltaTime = GameEngine::GetInstance()->GetDeltaTime();

		Dx = (DirVec.DirX / DirSize) * EnemyMoveBaseSpeed * DeltaTime;
		Dy = (DirVec.DirY / DirSize) * EnemyMoveBaseSpeed * DeltaTime;

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
		double MapMaxX = GScreen->SceneHorSize - ColliderRadius;
		double MapMaxY = GScreen->SceneVerSize - ColliderRadius;

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
	else if (CurrState == ECreatureState::MoveToTarget)
	{
		if (!CheckPlayerInDetectRange(World))
		{
			SetState(ECreatureState::Idle);
			AmountTime = 0.0;
			return;
		}

		if (CheckPlayerInAttackRange(World))
		{
			SetState(ECreatureState::Attack);
			AmountTime = 0.0;
			return;
		}

		FPos PlayerPos = World->GetPlayerPos();
		FPos Pos = Transform.Pos;
		FPos NextCell;
		FVec DirVec;

		std::vector<FPos> Path = World->FindPath(Transform.Pos, World->GetPlayerTransform()->Pos, FindDepth);
		if (Path.size() <= 1)
		{
			DirVec = { PlayerPos.X - Pos.X, PlayerPos.Y - Pos.Y };
		}
		else
		{
			NextCell = Path[1];
			DirVec = {NextCell.X - Transform.Pos.X, NextCell.Y - Transform.Pos.Y };
		}
		FPos NextPos;

		double Dx = 0.0;
		double Dy = 0.0;

		
		double DirSize = std::sqrt(GetSqrLen(DirVec));
		double DeltaTime = GameEngine::GetInstance()->GetDeltaTime();

		Dx = (DirVec.DirX / DirSize) * EnemyMoveBaseSpeed * DeltaTime;
		Dy = (DirVec.DirY / DirSize) * EnemyMoveBaseSpeed * DeltaTime;

		double ColliderRadius = 0.3;

		// 다음 좌표
		double NextX = Pos.X + Dx;
		double NextY = Pos.Y + Dy;

		// 다음 좌표의 충돌 체크
		double ColliderEdgeX = NextX + (Dx > 0 ? ColliderRadius : -ColliderRadius);
		double ColliderEdgeY = NextY + (Dy > 0 ? ColliderRadius : -ColliderRadius);
		if (World->GetWorldMap()[(int)Pos.Y][(int)ColliderEdgeX] != static_cast<int>(Env::WALL))
		{
			Pos.X = static_cast<float>(NextX);
		}



		if (World->GetWorldMap()[(int)ColliderEdgeY][(int)Pos.X] != static_cast<int>(Env::WALL))
		{
			Pos.Y = static_cast<float>(NextY);
		}


		// 콘솔 바운더리를 넘어가지는 못한다.
		double MapMin = ColliderRadius;
		Screen* GScreen = GameEngine::GetInstance()->GetScreen();
		double MapMaxX = GScreen->SceneHorSize - ColliderRadius;
		double MapMaxY = GScreen->SceneVerSize - ColliderRadius;

		if (Pos.X < ColliderRadius || Pos.X > MapMaxX || Pos.Y < ColliderRadius || Pos.Y > MapMaxY)
		{
			SetState(ECreatureState::Idle);
			if (Pos.X < ColliderRadius) Pos.X = static_cast<float>(ColliderRadius);
			if (Pos.X > MapMaxX) Pos.X = static_cast<float>(MapMaxX);
			if (Pos.Y < ColliderRadius) Pos.Y = static_cast<float>(ColliderRadius);
			if (Pos.Y > MapMaxY) Pos.Y = static_cast<float>(MapMaxY);
		}


		NextPos = { static_cast<float>(Pos.X), static_cast<float>(Pos.Y) };

		MoveToWithArray(NextPos, World);
	}
}

void FEnemy::UpdateState(WorldManager* World)
{
	double DeltaTime = GameEngine::GetInstance()->GetDeltaTime();
	ECreatureState CurrState = GetState();
	if (bBlink)
	{
		CurrenBlinkDuration -= DeltaTime;
		if (CurrenBlinkDuration <= 0)
		{
			CurrenBlinkDuration = BlinkDurationMax;
			bBlink = false;
		}
	}
	if (!bCanAttack) CurrentAttackDelay -= DeltaTime;
	if (CurrentAttackDelay <= 0) bCanAttack = true;


	if (CurrState == ECreatureState::Dead)
	{
		SpriteIndex = ECreatureSpriteIndex::Dead;
		return;
	}
	if (CurrState == ECreatureState::OnDead)
	{

		SpriteIndex = ECreatureSpriteIndex::Dead;

		//TODO 하드코딩 고치기
		if (AmountTime >= 0.5)
		{
			SetState(ECreatureState::Dead);
		}
		else
		{
  			AmountTime += GameEngine::GetInstance()->GetDeltaTime();
		}
	}
	else if (CurrState == ECreatureState::OnAttacked)
	{
		SpriteIndex = ECreatureSpriteIndex::OnAttacked;
		bBlink = true;

		if (AmountTime >= AnimDelay)
		{
			SetState(ECreatureState::Idle);
		}
		else
		{
			AmountTime += GameEngine::GetInstance()->GetDeltaTime();
		}
	}
	else if (CurrState == ECreatureState::Idle)
	{
		SpriteIndex = ECreatureSpriteIndex::Idle;

		if (CheckPlayerInAttackRange(World))
		{
			SetState(ECreatureState::Attack);
			return;
		}

		if (CheckPlayerInDetectRange(World))
		{
			SetState(ECreatureState::MoveToTarget);
			return;
		}

		// TODO 하드 코딩 고치기
		if (AmountTime >= 1)
		{
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

				SetState(ECreatureState::Patrol);
			}
		}
		else
		{
			AmountTime += GameEngine::GetInstance()->GetDeltaTime();
		}

	}
	else if (CurrState == ECreatureState::Patrol)
	{
		SpriteIndex = ECreatureSpriteIndex::OnAttacked;
		if (CheckPlayerInAttackRange(World))
		{
			SetState(ECreatureState::Attack);
			return;
		}

		if (CheckPlayerInDetectRange(World))
		{
			SetState(ECreatureState::MoveToTarget);
			return;
		}

		if (AmountTime > 1)
		{
			if (GetRandRange() < EnemyIdlePerCentage)
			{
				SetState(ECreatureState::Idle);
			}
		}
		else
		{
			AmountTime += GameEngine::GetInstance()->GetDeltaTime();
		}
	}
	else if (CurrState == ECreatureState::MoveToTarget)
	{
		SpriteIndex = ECreatureSpriteIndex::Idle;
		if (CheckPlayerInAttackRange(World))
		{
			SetState(ECreatureState::Attack);
			return;
		}
	}
	else if (CurrState == ECreatureState::Attack)
	{
		SpriteIndex = ECreatureSpriteIndex::OnAttacked;
		if (!bCanAttack)
		{
			SetState(ECreatureState::Idle);
			return;
		}

		if (AmountTime < 1 * 0.5)
		{
			SpriteIndex = ECreatureSpriteIndex::AttackStart;
		}
		else
		{
			SpriteIndex = ECreatureSpriteIndex::AttackEnd;
			if (AmountTime >= 0.6 && !bApplyDamege)
			{
				Attack(World->GetPlayer());
				bApplyDamege = true;
			}
		}

		if (AmountTime >= 1)
		{
			bCanAttack = false;
			bApplyDamege = false;
			CurrentAttackDelay = AttackDelayMax;
			State = ECreatureState::Idle;
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

bool FEnemy::CheckPlayerInDetectRange(WorldManager* World)
{
	FPos PlayerPos = World->GetPlayerPos();

	return GetSqrDist(Transform.Pos, PlayerPos) < DetectRadius * DetectRadius;
}

bool FEnemy::CheckPlayerInAttackRange(WorldManager* World)
{
	FPos PlayerPos = World->GetPlayerPos();
	return GetSqrDist(Transform.Pos, PlayerPos) < AttackRange * AttackRange;
}





