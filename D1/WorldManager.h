#pragma once
#include <vector>
#include <list>
#include <map>

#include "Player.h"
#include "Enemy.h"
#include "Define.h"

struct FRaycasterResult;

struct PQNode
{
	FPos Pos = { 0, 0 };
	int Huristic = 0;
	int Depth = 0;

	bool operator<(const PQNode others) const
	{
		return this->Huristic > others.Huristic;
	}
};


class WorldManager
{
private:
	// GameLoop
	FPlayer*														Player = nullptr;
	std::vector<FEnemy*>											EnemyVec;
	std::vector<std::vector<int>>									WorldMap;
	std::vector<std::vector<std::list<Creature*>>>					CreatureMap;

	// PauseLoop
	EPauseMenu 														PauseMenuIndex = EPauseMenu::None;
	EMainMenu 														MainMenuIndex = EMainMenu::None;

public:
	const int														mapWidth = 24;
	const int														mapHeight = 24;

public:
																	~WorldManager();

	void															Init();
	void															UpdateBeforeGameLoop();
	void															UpdateGameLoop();
	void															UpdatePauseLoop();
	void															PlayerUpdate(const WorldManager* World);
	void															EnemiesUpdate(WorldManager* World);
	void															SpawnEnemy(FCreatureBaseStat Stat, FTransform EnemyTranform, FSprite EnemySprite);
	void															HandleInput();
	void															UpdateCreatureMap(FPos Before, FPos After, FEnemy* Target);
	void															HandlePuaeMenu();
	void															HandleMainMenu();
	std::vector<FPos>												FindPath(FPos InStartPos, FPos InDestPos, int InMaxDepth);
	std::vector<FPos>												CalcPath(std::map<FPos, FPos>& Parent, FPos DestPos);
	bool															CanGo(FPos NextPos);

	inline FPlayer*													GetPlayer() const { return Player; }

	//TODO const 여부
	inline const std::vector<std::vector<int>>&						GetWorldMap() const { return WorldMap; }
	// 이 const는 어디까지 지켜주는 거지
	inline const std::vector<std::vector<std::list<Creature*>>>&	GetCreatureMap() const { return CreatureMap; }
	inline const std::vector<FEnemy*>*								GetEnemyVec() const { return &EnemyVec; }

	inline const EPauseMenu											GetPauseIndex() const { return PauseMenuIndex; }
	inline void														SetPauseIndex(EPauseMenu InVal) { PauseMenuIndex = InVal; }
	inline const EMainMenu											GetMainIndex() const { return MainMenuIndex; }
	inline void														SetMainIndex(EPauseMenu InVal) { PauseMenuIndex = InVal; }
};

