#include "Player.h"
#include "GameEngine.h"
#include "Utils.h"
#include "Raycaster.h"
#include "WorldManager.h"

//void FPlayer::SetDirVec()
//{
//	int NumForDecision = static_cast<int>(PlayerTheta);
//	if (NumForDecision < 0)
//		NumForDecision += 360;
//	if (NumForDecision >= 360)
//		NumForDecision -= 360;
//	//  0.0, -1.0, 0.66, 0.0
//	switch (NumForDecision)
//	{
//		case 0:
//			DirX = 0.0;		DirY = -1.0;
//			PlaneX = 0.66, PlaneY = 0.0;
//			RightX = -1.0, RightY = 0.0;
//			break;
//
//		case 45:
//			DirX = 1.0;	DirY = -1.0;
//			PlaneX = 0.4667, PlaneY = 0.4667;
//			RightX = -1.0, RightY = -1.0;
//			break;
//
//		case 90:
//			DirX = 1.0;	DirY = 0.0;
//			PlaneX = 0.0, PlaneY = 0.66;
//			RightX = 0.0, RightY = -1.0;
//			break;
//
//		case 135:
//			DirX = 1.0;	DirY = 1.0;
//			PlaneX = -0.4667, PlaneY = 0.4667;
//			RightX = 1.0, RightY = -1.0;
//			break;
//
//		case 180:
//			DirX = 0.0;		DirY = 1.0;
//			PlaneX = -0.66, PlaneY = 0.0;
//			RightX = 1.0, RightY = 0.0;
//			break;
//
//		case 225:
//			DirX = -1.0;	DirY = 1.0;
//			PlaneX = -0.4667, PlaneY = -0.4667;
//			RightX = 1.0, RightY = 1.0;
//			break;
//
//		case 270:
//			DirX = -1.0;	DirY = 0.0;
//			PlaneX = 0.0, PlaneY = -0.66;
//			RightX = 0.0, RightY = 1.0;
//			break;
//
//		case 315:
//			DirX = -1.0;	DirY = -1.0;
//			PlaneX = 0.4667, PlaneY = -0.4667;
//			RightX = -1.0, RightY = 1.0;
//			break;
//	}
//}


void FPlayer::Update(const WorldManager* World)
{
	Roate(World);
	Move(World);
	UpdateState();
	UpdateStateBehavior(World);
}

void FPlayer::Roate(const WorldManager* World)
{
	// 회전 처리
	//if (KeyState.UpArrow)			*Dy -= MoveSpeed;
	//if (KeyState.DownArrow)		*Dy += MoveSpeed;
	//// 아래는 45도 기준으로 
	//double Theta = 45.0;
	//if (KeyState.LeftArrowDown && !PrevKeyInfo.PrevLeftArrow)
	//{
	//	Theta *= -1.0;
	//	Player.PlayerTheta += Theta;
	//}
	//if (KeyState.RightArrowDown && !PrevKeyInfo.PrevRightArrow)
	//{
	//	Player.PlayerTheta += Theta;
	//}

	//Player.SetDirVec();
	//PrevKeyInfo.PrevLeftArrow = KeyState.LeftArrowDown;
	//PrevKeyInfo.PrevRightArrow = KeyState.RightArrowDown;

	// Theta * deltaTime을 곱하는 방식
	double DeltaTime = GameEngine::GetInstance()->GetDeltaTime();
	FKeyState KeyState = GameEngine::GetInstance()->GetInputManager()->GetKeyState();
	double FinalTheta = 0.0;
	if (!KeyState.LeftArrowDown && !KeyState.RightArrowDown) return;

	if (KeyState.LeftArrowDown)
	{
		FinalTheta -= RotationSpeed * DeltaTime;
	}

	if (KeyState.RightArrowDown)
	{
		FinalTheta += RotationSpeed * DeltaTime;
	}

	double FinalRad = FinalTheta * PHI / 180.0;


	// 회전 변환
	// 플레이어가 바라보는 방향 회전

	FVec PlayerDir = Transform.GetDirVec();
	double PlayerX = PlayerDir.DirX;
	double PlayerY = PlayerDir.DirY;

	auto NextDirX = PlayerX * cos(FinalRad) - PlayerY * sin(FinalRad);
	auto NextDirY = PlayerX * sin(FinalRad) + PlayerY * cos(FinalRad);

	FVec NextPlayerDIr = { NextDirX, NextDirY };
	RotateTo(NextPlayerDIr, FinalTheta);
}

void FPlayer::Move(const WorldManager* World)
{
	double Dx = 0.0;
	double Dy = 0.0;
	double theta = 45;

	FVec PlayerDirVec = Transform.GetDirVec();
	double PlayerDirX = PlayerDirVec.DirX;
	double PlayerDirY = PlayerDirVec.DirY;

	FVec PlayerRightDirVec = Transform.GetRightDirVec();
	double PlayerRightX = PlayerRightDirVec.DirX;
	double PlayerRightY = PlayerRightDirVec.DirY;

	FPos PlayerPos = Transform.GetPos();
	double PlayerPosX = PlayerPos.X;
	double PlayerPosY = PlayerPos.Y;

	double DirSize = GetSqrLen(Transform.GetDirVec());
	DirSize = sqrt(DirSize);

	double RightSize = GetSqrLen(Transform.GetRightDirVec());
	RightSize = sqrt(RightSize);

	double DeltaTime = GameEngine::GetInstance()->GetDeltaTime();
	FKeyState KeyState = GameEngine::GetInstance()->GetInputManager()->GetKeyState();
	// 현재 바라보는 방향을 기준으로 앞뒤좌우 (상대적임)
	// 이동 처리
	if (!KeyState.KEYW && !KeyState.KEYD && !KeyState.KEYS && !KeyState.KEYA) return;
	if (KeyState.KEYW)
	{
		Dx = (PlayerDirX / DirSize) * MoveBaseSpeed * DeltaTime;
		Dy = (PlayerDirY / DirSize) * MoveBaseSpeed * DeltaTime;
	}
	if (KeyState.KEYD)
	{
		Dx = (PlayerRightX / RightSize) * MoveBaseSpeed * DeltaTime;
		Dy = (PlayerRightY / RightSize) * MoveBaseSpeed * DeltaTime;
	}
	if (KeyState.KEYS)
	{
		Dx = (PlayerDirX / DirSize) * MoveBaseSpeed * DeltaTime * -1.0;
		Dy = (PlayerDirY / DirSize) * MoveBaseSpeed * DeltaTime * -1.0;
	}
	if (KeyState.KEYA)
	{
		Dx = (PlayerRightX / RightSize) * MoveBaseSpeed * DeltaTime * -1.0;
		Dy = (PlayerRightY / RightSize) * MoveBaseSpeed * DeltaTime * -1.0;
	}

	double ColliderRadius = 0.3;

	// 다음 좌표
	double NextX = PlayerPosX + Dx;
	double NextY = PlayerPosY + Dy;

	// 다음 좌표의 충돌 체크
	double ColliderEdgeX = NextX + (Dx > 0 ? ColliderRadius : -ColliderRadius);
	if (World->GetWorldMap()[(int)PlayerPosY][(int)ColliderEdgeX] != static_cast<int>(Env::WALL))
	{
		PlayerPosX = NextX;
	}

	double ColliderEdgeY = NextY + (Dy > 0 ? ColliderRadius : -ColliderRadius);
	if (World->GetWorldMap()[(int)ColliderEdgeY][(int)PlayerPosX] != static_cast<int>(Env::WALL))
	{
		PlayerPosY = NextY;
	}

	// 콘솔 바운더리를 넘어가지는 못한다.
	double MapMin = ColliderRadius;
	//TODO Gscreen으로 바꾸기
	double MapMaxX = TargetWidth - 0.3;
	double MapMaxY = TargetHeight - 0.3;

	if (PlayerPosX < ColliderRadius) PlayerPosX = ColliderRadius;
	if (PlayerPosX > MapMaxX) PlayerPosX = MapMaxX;
	if (PlayerPosY < ColliderRadius) PlayerPosY = ColliderRadius;
	if (PlayerPosY > MapMaxY) PlayerPosY = MapMaxY;

	FPos NextPos = { static_cast<float>(PlayerPosX), static_cast<float>(PlayerPosY) };
	MoveTo(NextPos);
}

void FPlayer::UpdateState()
{
	PrevState = State;
	double DeltaTime = GameEngine::GetInstance()->GetDeltaTime();
	FKeyState KeyState = GameEngine::GetInstance()->GetInputManager()->GetKeyState();
	if (KeyState.KEYSpaceDown && GetState() == ECreatureState::Idle)
	{
		SetState(ECreatureState::Attack);
		AmountTime = 0.0;

		return;
	}

	if (GetState() == ECreatureState::Attack)
	{
		AmountTime += DeltaTime;
		if (AmountTime > PlayerAttackCoolTime)
		{
			AmountTime = 0.0;
			SetState(ECreatureState::Idle);
		}
	}

}

void FPlayer::UpdateStateBehavior(const WorldManager* World)
{
	if (GetState() == ECreatureState::Attack && PrevState != ECreatureState::Attack)
	{
		// RayCast
		FRaycasterResult Res = DDA(TargetWidth / 2, ERayCastLayer::Creature, World);

		std::list<Creature*> CreatureList = World->GetCreatureMap()[static_cast<int>(Res.MapPos.Y)][static_cast<int>(Res.MapPos.X)];

		Creature* Target = FindClosetTargetFromList(Res,Transform.Pos, CreatureList);

		if (Target != nullptr)
			Attack(Target);
	}
}
