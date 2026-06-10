#include <queue>
#include <unordered_map>

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
		if (EnemyVec[i])
		{
			delete EnemyVec[i];
			EnemyVec[i] = nullptr;
		}
	}
}

void WorldManager::Init()
{
	// Game Loop
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
				L"▒▒▒▒▒▒▒▒▒▒▒▒",
				L"▒▒▓▓▓▓▓▓▓▓▒▒",
				L" ▒▒▒▒▒▒▒▒▒▒",
				L"  ▒▒▒▒▒▒▒▒  ",
				}
			},
		}
	};

	SpawnEnemy(EnemyStat, EnemyTranform, EnemySprite);


	// Pause Loop
	PauseMenuIndex = EPauseMenu::Resume;
	MainMenuIndex = EMainMenu::StartGame;

}

void WorldManager::UpdateBeforeGameLoop()
{
	FKeyState KeyState = GameEngine::GetInstance()->GetInputManager()->GetKeyState();
	if (KeyState.KEYSpaceDown)
	{
		HandleMainMenu();
	}
	else if (KeyState.DownArrowDown)
	{
		int CurrentIndex = static_cast<int>(MainMenuIndex);
		int UpperBound = static_cast<int>(EMainMenu::EMainMenuLen);
		if (CurrentIndex + 1 >= UpperBound)
		{
			MainMenuIndex = EMainMenu::StartGame;
		}
		else
		{
			MainMenuIndex = static_cast<EMainMenu>(CurrentIndex + 1);
		}
	}
	else if (KeyState.UpArrowDown)
	{
		int CurrentIndex = static_cast<int>(MainMenuIndex);
		int LowerBound = static_cast<int>(EMainMenu::None);
		if (CurrentIndex - 1 <= LowerBound)
		{
			MainMenuIndex = EMainMenu::ExitGame;
		}
		else
		{
			MainMenuIndex = static_cast<EMainMenu>(CurrentIndex - 1);
		}
	}
}

void WorldManager::UpdateGameLoop()
{
	PlayerUpdate(this);
	EnemiesUpdate(this);
}

void WorldManager::UpdatePauseLoop()
{
	FKeyState KeyState = GameEngine::GetInstance()->GetInputManager()->GetKeyState();
	if (KeyState.KEYSpaceDown)
	{
		HandlePuaeMenu();
	}
	else if (KeyState.DownArrowDown)
	{
		int CurrentIndex = static_cast<int>(PauseMenuIndex);
		int UpperBound = static_cast<int>(EPauseMenu::EPauseMenuLen);
		if (CurrentIndex + 1 >= UpperBound)
		{
			PauseMenuIndex = EPauseMenu::Resume;
		}
		else
		{
			PauseMenuIndex = static_cast<EPauseMenu>(CurrentIndex + 1);
		}
	}
	else if (KeyState.UpArrowDown)
	{
		int CurrentIndex = static_cast<int>(PauseMenuIndex);
		int LowerBound = static_cast<int>(EPauseMenu::None);
		if (CurrentIndex - 1 <= LowerBound)
		{
			PauseMenuIndex = EPauseMenu::Exit;
		}
		else
		{
			PauseMenuIndex = static_cast<EPauseMenu>(CurrentIndex - 1);
		}
	}
}

void WorldManager::PlayerUpdate(const WorldManager* World)
{
	Player->Update(World);
}

void WorldManager::EnemiesUpdate(WorldManager* World)
{
	// 죽은 Enemy WolrdManger에서 일괄 삭제
	// Ondead에서는 죽었다고 표시만
	// 여기서 순회해서 표시된 아이들을 지움
	for (int i = 0; i < EnemyVec.size(); i++)
	{
		// 죽은 상태이면
		if (EnemyVec[i]->GetState() == ECreatureState::Dead)
		{
			// 격자 정보에서 지우기
			FPos CurrPos = EnemyVec[i]->GetTransform()->GetPos();
			std::list<Creature*> CurrList = CreatureMap[static_cast<int>(CurrPos.Y)][static_cast<int>(CurrPos.X)];
			auto itr = std::find(CurrList.begin(), CurrList.end(), EnemyVec[i]);
			if (itr != CurrList.end())
			{
				CurrList.erase(itr);
			}

			// 할당 해제
			delete EnemyVec[i];
			EnemyVec[i] = nullptr;

			// 적 정보 리스트에서 삭제
			EnemyVec.erase(EnemyVec.begin() + i);
		}
		else
		{
			//시야각 안에있는 적들만 Update();
			if (EnemyVec[i]->GetIsInSight())
				EnemyVec[i]->Update(World);

			EnemyVec[i]->SetIsInSight(false);
		}
	}
}

void WorldManager::SpawnEnemy(FCreatureBaseStat Stat, FTransform EnemyTranform, FSprite EnemySprite)
{
	//TODO null check
	auto NewEnemy = new FEnemy(Stat, EnemyTranform, EnemySprite);
	CreatureMap[static_cast<int>(EnemyTranform.Pos.Y)][static_cast<int>(EnemyTranform.Pos.X)].push_back(NewEnemy);
	EnemyVec.push_back(NewEnemy);
}

void WorldManager::HandleInput()
{
	FKeyState KeyState = GameEngine::GetInstance()->GetInputManager()->GetKeyState();
	// TODO GameEngine Run을 종료하도록 변경
	if (KeyState.QKey)
		exit(0);
}

void WorldManager::UpdateCreatureMap(FPos Before, FPos After, FEnemy* Target)
{
	if (static_cast<int>(Before.X) == static_cast<int>(After.X) && static_cast<int>(Before.Y) == static_cast<int>(After.Y))
		return;

	std::list<Creature*>& CurrList = CreatureMap[static_cast<int>(Before.Y)][static_cast<int>(Before.X)];
	auto itr = std::find(CurrList.begin(), CurrList.end(), Target);
	if (itr != CurrList.end())
	{
		CreatureMap[static_cast<int>(Before.Y)][static_cast<int>(Before.X)].erase(itr);
	}

	CreatureMap[static_cast<int>(After.Y)][static_cast<int>(After.X)].push_back(Target);
}

void WorldManager::HandlePuaeMenu()
{
	if (PauseMenuIndex == EPauseMenu::None || PauseMenuIndex == EPauseMenu::EPauseMenuLen)
		return;

	switch (PauseMenuIndex)
	{
		case EPauseMenu::None:
		case EPauseMenu::EPauseMenuLen:
			break;
		case EPauseMenu::Resume:
			GameEngine::GetInstance()->SetIsPuase(false);
			break;
		case EPauseMenu::Exit:
			GameEngine::GetInstance()->SetIsExit(true);
			break;
	}

	PauseMenuIndex = EPauseMenu::Resume;
}

void WorldManager::HandleMainMenu()
{
	if (MainMenuIndex == EMainMenu::None || MainMenuIndex == EMainMenu::EMainMenuLen)
		return;

	switch (MainMenuIndex)
	{
		case EMainMenu::None:
		case EMainMenu::EMainMenuLen:
			break;
		case EMainMenu::StartGame:
			GameEngine::GetInstance()->SetGameState(EGameState::InGame);
			break;
		case EMainMenu::ExitGame:
			GameEngine::GetInstance()->SetIsExit(true);
			break;
	}

	MainMenuIndex = EMainMenu::StartGame;
}

std::vector<FIntPos> WorldManager::FindPath(FPos InStartPos, FPos InDestPos, int InMaxDepth)
{
	const int DirLen = 8;
	int Dx[DirLen] = { 0, 1, 0, -1,1,1,-1,-1 };
	int Dy[DirLen] = { 1, 0, -1, 0,1,-1,1,-1 };
	std::priority_queue<PQNode> Pq;
	std::unordered_map<FIntPos, int> Best;
	std::unordered_map<FIntPos, FIntPos> Parent;

	FIntPos CurrentPos = { static_cast<int>(InStartPos.X), static_cast<int>(InStartPos.Y) };
	FIntPos DestPos = { static_cast<int>(InDestPos.X), static_cast<int>(InDestPos.Y) };
	FIntPos ClosestCellPos = { static_cast<int>(InStartPos.X), static_cast<int>(InStartPos.Y) };

	// 현재 좌표에서 계산한 휴리스틱
	int ClosetHuristic = GetSqrDist(DestPos, CurrentPos);
	int Huristic = ClosetHuristic;
	// 시작 죄표를 우선순위 큐에 넣음
	Pq.push({ CurrentPos, Huristic, 1 });

	// 시작 좌표의 부모는 자기 자신
	Parent[CurrentPos] = CurrentPos;
	// 현재 좌표까지의 최적값
	Best[CurrentPos] = Huristic;

	while (!Pq.empty())
	{
		PQNode Node = Pq.top();
		Pq.pop();

		// 현재 노드 갱신
		CurrentPos = Node.Pos;

		//목적지 도달 시 종료
		if (CurrentPos.X == DestPos.X && CurrentPos.Y == DestPos.Y) break;
		// 탐색 깊이를 초과했으면 조기 종료
		if (Node.Depth > InMaxDepth) break;
		for (int i = 0; i < DirLen; i++)
		{
			FIntPos NextPos = { CurrentPos.X + Dx[i], CurrentPos.Y + Dy[i] };
			if (!CanGo(NextPos)) continue;

			// 휴리스틱 갱신
			Huristic = GetSqrDist(DestPos, NextPos);

			// 한번도 안온 경우 
			if (Best[NextPos] == 0)
				Best[NextPos] = INT32_MAX;

			// 이미 이것보다 좋은 경로를 찾은 경우
			if (Best[NextPos] <= Huristic) continue;

			// 다음 좌표까지 최적값 기록
			Best[NextPos] = Huristic;
			
			// 여기까지 왔다는건, NextPos 까지 가는데 최단 Huristic으로 온것
			Pq.push({ NextPos, Huristic, Node.Depth + 1 });
			Parent[NextPos] = CurrentPos;
	
			// 경로가 갱신되면
			// 해당 좌표로 이동
			if (ClosetHuristic > Huristic)
			{
				ClosetHuristic = Huristic;
				ClosestCellPos = NextPos;
			}
		}
	}

	// Dest의 부모가 기록되지 않았으면
	// Depth 내에 Dest로 가는 경로를 찾지 못했으면
	if (Parent.find(DestPos) == Parent.end())
	{
		// 현재 경로들중 가장 huristic이 적인 곳을 목적지로 입력
		DestPos = ClosestCellPos;
	}

	return CalcPath(Parent, DestPos);
}

std::vector<FIntPos> WorldManager::CalcPath(std::unordered_map<FIntPos, FIntPos>& Parent, FIntPos DestPos)
{
	std::vector<FIntPos> Res;
	std::vector<FIntPos> ReverseRes;

	FIntPos Current = DestPos;
	int Cnt = 0;
	// 부모가 자기자신일 떄까지(시작 좌표까지)
	while (Parent[Current] != Current)
	{
		// 너무 긴 경로는 도중에 반환
		if (Cnt++ > 500) break;
		Res.push_back(Current);
		Current = Parent[Current];
	}

	Res.push_back(Current);
	ReverseRes = std::vector<FIntPos>(Res.rbegin(), Res.rend());


	return ReverseRes;
}

bool WorldManager::CanGo(FIntPos NextPos)
{
	int X = NextPos.X;
	int Y = NextPos.Y;

	if (X < 0 || X > mapHeight) return false;
	if (Y < 0 || Y > mapHeight) return false;
	if (WorldMap[Y][X] == static_cast<int>(Env::WALL)) return false;

	return true;
}


