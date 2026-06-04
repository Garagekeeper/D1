#pragma once
#include <vector>
#include <list>
#include "Player.h"
#include "Enemy.h"

class WorldManager
{
private:
	FPlayer*										Player;
	std::vector<FEnemy*>							EnemyVec;
	std::vector<std::vector<int>>					WorldMap;
	std::vector<std::vector<std::list<Creature*>>>	CreatureMap;

public:
	const int mapWidth = 24;
	const int mapHeight = 24;

public:
	void Init();
	void Update();
	void SpawnEnemy(FTransform EnemyTranform, FSprite EnemySprite);

	inline FPlayer* GetPlayer() const { return Player; }

	//TODO const 여부
	inline const std::vector<std::vector<int>>& GetWorldMap() const { return WorldMap; }
	inline const std::vector<std::vector<std::list<Creature*>>>& GetCreatureMap() const { return CreatureMap; }
	inline const std::vector<FEnemy*>* GetEnemyVec() const { return &EnemyVec; }
};

