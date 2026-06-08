#include "WorldManager.h"
#include "Utils.h"
#include "GameEngine.h"
#include "Raycaster.h"

WorldManager::~WorldManager()
{
	delete Player;
	Player = nullptr;

	for (int i = 0; i < EnemyVec.size(); i++)
	{
		delete EnemyVec[i];
		EnemyVec[i] = nullptr;
	}
}

void WorldManager::Init()
{
	WorldMap =
	{
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,1,0,1,0,1,0,0,0,1},
		{1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1},
		{1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,1,1,0,1,1,0,0,0,0,1,0,1,0,1,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,1,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,1,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,1,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,1,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};

	CreatureMap = std::vector<std::vector<std::list<Creature*>>>(mapWidth, std::vector<std::list<Creature*>>(mapHeight, std::list<Creature*>()));


	FCreatureBaseStat PlayerStat = { 50, 100, 100 };
	FPos PlayerPos = { 15, 16 };
	FVec PlayerDirVec = { 0.0, -1.0 };
	double PlaneSize = 0.66;
	FTransform PlayerTranform =
	{
		PlayerPos,
		PlayerDirVec,
		PlaneSize
	};

	FSprite PlayerWeponHudSprite =
	{
		0,
		0,
		0,
		{
			{
				L"         ⠶        ",
				L"      ⡔⠉⠉⠉⠉⠉⢢     ",
				L"     ⢰⠁⠄⠄⠄⠄⠠⠈⡆    ",
				L"     ⡥⠭⢭⠉⠉⠉⡭⠭⢬    ",
				L"    ⣌⣀⣛⣸⣤⣤⣤⣇⣛⣀⣡   ",
				L"   ⣸⣉⣉⣉⣉⣉⣉⣉⣉⣉⣉⣉⣇  ",
				L"   ⡇⣿⣿⡞⠉⠉⠉⠉⠉⢳⣿⣿⢸  ",
				L"   ⡇⣿⣿⡇⠄⠄⠄⠄⠄⢸⣿⣿⢸  ",
				L"   ⡇⣿⣿⡇⠄⠄⠄⠄⠄⢸⣿⣿⢸  ",
				L"   ⡇⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⢸  ",
				L"    ⠙⢿⣿⣿⣿⣿⣿⣿⣿⡿⠋   ",
				L"     ⢸⣿⣿⣿⣿⣿⣿⣿⡇    ",
			},

			{
				L"     ⣿⣿⣿⣿⠶⣿⣿⣿⣿    ",
				L"  ⣿⣿⣿⣿⡔⠉⠉⠉⠉⠉⢢⣿⣿⣿⣿ ",
				L"  ⣿⣿⣿⢰⠁⠄⠄⠄⠄⠠⠈⡆⣿⣿⣿ ",
				L"  ⣿⣿⣿⡥⠭⢭⠉⠉⠉⡭⠭⢬⣿⣿⣿ ",
				L" ⣿⣿⣿⣌⣀⣛⣸⣤⣤⣤⣇⣛⣀⣡⣿⣿⣿",
				L" ⣿⣿⣸⣉⣉⣉⣉⣉⣉⣉⣉⣉⣉⣉⣇⣿⣿",
				L"   ⡇⣿⣿⡞⠉⠉⠉⠉⠉⢳⣿⣿⢸  ",
				L"   ⡇⣿⣿⡇⠄⠄⠄⠄⠄⢸⣿⣿⢸  ",
				L"   ⡇⣿⣿⡇⠄⠄⠄⠄⠄⢸⣿⣿⢸  ",
				L"   ⡇⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⢸  ",
				L"    ⠙⢿⣿⣿⣿⣿⣿⣿⣿⡿⠋   ",
				L"     ⢸⣿⣿⣿⣿⣿⣿⣿⡇    ",
			}
		}
	};

	Player = new FPlayer(PlayerStat, PlayerTranform, PlayerWeponHudSprite);

	FCreatureBaseStat EnemyStat = { 10, 200, 200 };
	FPos EnemyPos = { 15, 12 };
	FVec EnemyDirVec = { 0.0, -1.0 };
	PlaneSize = 0.66;
	FTransform EnemyTranform =
	{
		EnemyPos,
		EnemyDirVec,
		PlaneSize
	};

	FSprite EnemySprite =
	{
		0,
		0,
		TargetHeight / (float)4,
		{
			{
				{
				L"  ▒▒▒▒▒▒▒▒  ",
				L" ▒▒▒▒▒▒▒▒▒▒ ",
				L"▒▒▒▒▓▒▒▓▒▒▒▒",
				L"▒▒▒▒▓▒▒▓▒▒▒▒",
				L"▒▒▒▒▓▒▒▓▒▒▒▒",
				L"▒▒▒▒▒▒▒▒▒▒▒▒",
				L"▒▒▒▒▒▒▒▒▒▒▒▒",
				L"▒▒▒▒▒▒▒▒▒▒▒▒",
				L"▒▓▒▒▒▒▒▒▒▒▓▒",
				L"▒▒▓▒▒▒▒▒▒▓▒▒",
				L" ▒▒▓▓▓▓▓▓▒▒",
				L"  ▒▒▒▒▒▒▒▒  ",
				}
			},

			{
				{
				L"  ▒▒▒▒▒▒▒▒  ",
				L" ▒▒▒▒▒▒▒▒▒▒ ",
				L"▒▒▒▒▓▒▒▓▒▒▒▒",
				L"▒▒▒▒▓▒▒▓▒▒▒▒",
				L"▒▒▒▒▓▒▒▓▒▒▒▒",
				L"▒▒▒▒▒▒▒▒▒▒▒▒",
				L"▒▒▒▒▒▒▒▒▒▒▒▒",
				L"▒▒▒▒▒▒▒▒▒▒▒▒",
				L"▒▓▒▒▒▒▒▒▒▒▓▒",
				L"▒▒▓▒▒▒▒▒▒▓▒▒",
				L" ▒▒▓▓▓▓▓▓▒▒",
				L"  ▒▒▒▒▒▒▒▒  ",
				}
			},
		}
	};

	SpawnEnemy(EnemyTranform, EnemySprite);
}

void WorldManager::Update()
{

	PlayerUpdate();
	EnemiesUpdate();
}

void WorldManager::PlayerMove()
{
	double Dx = 0.0;
	double Dy = 0.0;
	double theta = 45;

	FTransform* PlayerTransform = Player->GetTransform();
	FVec PlayerDirVec = PlayerTransform->GetDirVec();
	double PlayerDirX = PlayerDirVec.DirX;
	double PlayerDirY = PlayerDirVec.DirY;

	FVec PlayerRightDirVec = PlayerTransform->GetRightDirVec();
	double PlayerRightX = PlayerRightDirVec.DirX;
	double PlayerRightY = PlayerRightDirVec.DirY;

	FPos PlayerPos = PlayerTransform->GetPos();
	double PlayerPosX = PlayerPos.X;
	double PlayerPosY = PlayerPos.Y;

	double DirSize = GetSqrLen(PlayerTransform->GetDirVec());
	DirSize = sqrt(DirSize);

	double RightSize = GetSqrLen(PlayerTransform->GetRightDirVec());
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
	if (WorldMap[(int)PlayerPosY][(int)ColliderEdgeX] != static_cast<int>(Env::WALL))
	{
		PlayerPosX = NextX;
	}

	double ColliderEdgeY = NextY + (Dy > 0 ? ColliderRadius : -ColliderRadius);
	if (WorldMap[(int)ColliderEdgeY][(int)PlayerPosX] != static_cast<int>(Env::WALL))
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
	Player->MoveTo(NextPos);
}

void WorldManager::PlayerRotate()
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

	FVec PlayerDir = Player->GetTransform()->GetDirVec();
	double PlayerX = PlayerDir.DirX;
	double PlayerY = PlayerDir.DirY;

	auto NextDirX = PlayerX * cos(FinalRad) - PlayerY * sin(FinalRad);
	auto NextDirY = PlayerX * sin(FinalRad) + PlayerY * cos(FinalRad);

	FVec NextPlayerDIr = { NextDirX, NextDirY };
	Player->RotateTo(NextPlayerDIr, FinalTheta);
}

void WorldManager::PlayerStateUpdate()
{
	double DeltaTime = GameEngine::GetInstance()->GetDeltaTime();
	FKeyState KeyState = GameEngine::GetInstance()->GetInputManager()->GetKeyState();
	if (KeyState.KEYSpaceDown && Player->GetState() == ECreatureState::Idle)
	{
		Player->SetState(ECreatureState::Attack);
		//TODO Gscreen으로 바꾸기
		FRaycasterResult Res = DDA(TargetWidth / 2, ERayCastLayer::Creature, this);
		//TODO Gscreen으로 바꾸기
		if (Res.bHit
			&& GameEngine::GetInstance()->GetScreen()->Zbuffer[TargetWidth / 2] > Res.PerpDist)
		{
			std::list<Creature*> CreatureList = CreatureMap[static_cast<int>(Res.MapPos.Y)][static_cast<int>(Res.MapPos.X)];
			Creature* MinCreature = nullptr;

			double Min = INT32_MAX;
			for (auto e : CreatureList)
			{
				double Dist = GetSqrDist(e->GetTransform()->Pos, Player->GetTransform()->Pos);
				if (Dist < Min)
				{
					Min = Dist;
					MinCreature = e;
				}
			}

			Creature* Target = MinCreature;
			Player->Attack(Target);
		}
		AmountTime = 0.0;

		return;
	}

	if (Player->GetState() == ECreatureState::Attack)
	{
		AmountTime += DeltaTime;
		if (AmountTime > PlayerAttackCoolTime)
		{
			AmountTime = 0.0;
			Player->SetState(ECreatureState::Idle);
		}
	}
}

void WorldManager::PlayerUpdate()
{
	PlayerRotate();
	PlayerMove();
	PlayerStateUpdate();
}

void WorldManager::EnemiesUpdate()
{

}

void WorldManager::SpawnEnemy(FTransform EnemyTranform, FSprite EnemySprite)
{
	//TODO null check
	auto NewEnemy = new FEnemy(EnemyTranform, EnemySprite);
	NewEnemy->MoveToWithArray(EnemyTranform.Pos, &CreatureMap);
	EnemyVec.push_back(NewEnemy);
}

void WorldManager::HandleInput()
{
	FKeyState KeyState = GameEngine::GetInstance()->GetInputManager()->GetKeyState();
	if (KeyState.QKey)
		exit(0);
}


