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


