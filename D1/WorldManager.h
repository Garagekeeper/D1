#pragma once
#include <vector>
#include <list>
#include "Player.h"
#include "Enemy.h"
struct FRaycasterResult;

class WorldManager
{
private:
	FPlayer*										Player = nullptr;
	std::vector<FEnemy*>							EnemyVec;
	std::vector<std::vector<int>>					WorldMap;
	std::vector<std::vector<std::list<Creature*>>>	CreatureMap;

public:
	const int mapWidth = 24;
	const int mapHeight = 24;

public:
	~WorldManager();

	void Init();
	void Update();
	void PlayerUpdate(const WorldManager* World);
	void EnemiesUpdate(WorldManager* World);
	void SpawnEnemy(FCreatureBaseStat Stat, FTransform EnemyTranform, FSprite EnemySprite);
	void HandleInput();
	void UpdateCreatureMap(FPos Before, FPos After, FEnemy* Target);

	inline FPlayer* GetPlayer() const { return Player; }

	//TODO const 여부
	inline const std::vector<std::vector<int>>& GetWorldMap() const { return WorldMap; }

	// 이 const는 어디까지 지켜주는 거지
	inline const std::vector<std::vector<std::list<Creature*>>>& GetCreatureMap() const { return CreatureMap; }
	inline const std::vector<FEnemy*>* GetEnemyVec() const { return &EnemyVec; }
};

