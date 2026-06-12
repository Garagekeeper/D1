#pragma once
#include <vector>
#include <list>
#include <map>
#include <unordered_map>

#include "Player.h"
#include "Enemy.h"
#include "Define.h"

struct FRaycasterResult;

struct PQNode
{
	FPos Pos = { 0, 0 };
	double Huristic = 0.0f;
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
	std::vector<PObject*>											ObjectVec;
	std::vector<std::vector<int>>									WorldMap;
	std::vector<std::vector<std::list<Creature*>>>					CreatureMap;
	FIntPos															ExitPos;

	// PauseLoop
	EPauseMenu 														PauseMenuIndex = EPauseMenu::None;
	EMainMenu 														MainMenuIndex = EMainMenu::None;
	EDeathMenu 														DeathMenuIndex = EDeathMenu::None;
	double															AmountTime = 0.0;
	bool															IsClear = false;
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
	void															UpdateOnPlayerDeath();
	void															UpdateOnClear();
	void															SpawnEnemy(FCreatureBaseStat Stat, FTransform EnemyTranform, FSprite EnemySprite);
	void															UpdateCreatureMap(FPos Before, FPos After, FEnemy* Target);
	void															HandlePuaeMenu();
	void															HandleDeathMenu();
	void															HandleMainMenu();
	void															HandleClear();
	void															SpawnObject(FTransform EnemyTranform, FSprite EnemySprite);
	std::vector<FPos>												FindPath(FPos InStartPos, FPos InDestPos, int InMaxDepth);
	std::vector<FPos>												CalcPath(std::unordered_map<FPos,FPos>& Parent, FPos DestPos);
	bool															CanGo(FPos NextPos);
	bool															CheckExitCondition();
	inline double													GetAmountTime() const { return AmountTime; }
	inline FPlayer*													GetPlayer() const { return Player; }
	inline FTransform*												GetPlayerTransform() const { return Player->GetTransform(); }
	inline const FPos												GetPlayerPos() const {return Player->GetTransform()->GetPos(); }

	//TODO const 여부
	inline const std::vector<std::vector<int>>&						GetWorldMap() const { return WorldMap; }
	// 이 const는 어디까지 지켜주는 거지
	inline const std::vector<std::vector<std::list<Creature*>>>&	GetCreatureMap() const { return CreatureMap; }
	inline const std::vector<FEnemy*>*								GetEnemyVec() const { return &EnemyVec; }

	inline const EPauseMenu											GetPauseIndex() const { return PauseMenuIndex; }
	inline void														SetPauseIndex(EPauseMenu InVal) { PauseMenuIndex = InVal; }
	inline const EMainMenu											GetMainIndex() const { return MainMenuIndex; }
	inline void														SetMainIndex(EPauseMenu InVal) { PauseMenuIndex = InVal; }
	inline bool														GetIsPlayerDead() const { return Player->GetState() == ECreatureState::Dead; }
	inline bool														GetIsClear() const { return IsClear; }
};

