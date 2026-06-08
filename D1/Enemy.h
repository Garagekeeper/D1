#pragma once
#include "Creature.h"
#include "vector"
#include "list"
class FEnemy : public Creature
{

public:

	FEnemy(FCreatureBaseStat _Stat, FTransform _Transform, FSprite _Sprites)
		: Creature(_Stat, _Transform, _Sprites)
	{}
	FEnemy(FTransform _Transform, FSprite _Sprites)
		: Creature({ 10,500,500 }, _Transform, _Sprites)
	{}


	void MoveToWithArray(FPos NextPos, std::vector<std::vector<std::list<Creature*>>>* CreatureMap);
	void Update();
};

