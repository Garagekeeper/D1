#pragma once
#include <vector>
#include <list>

class FPlayer;
class Creature;

class WorldManager
{
private:
	FPlayer* Player;
	std::vector<std::vector<int>> WorldMap;
	std::vector<std::vector<std::list<Creature*>>> CreatureMap;

public:
	const int mapWidth = 24;
	const int mapHeight = 24;

public:
	void Init();
	void Update();

	inline FPlayer* GetPlayer() const { return Player; }

	//TODO const 여부
	inline const std::vector<std::vector<int>>& GetWorldMap() const { return WorldMap; }
	inline const std::vector<std::vector<std::list<Creature*>>>& GetCreatureMap() const { return CreatureMap; }
};

