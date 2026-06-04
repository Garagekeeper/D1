#pragma once
#include "Creature.h"
class FEnemy : public Creature
{

public:
	FEnemy(FCreatureBaseStat _Stat, FTransform _Transform, FSprite _Sprites)
		: Creature(_Stat, _Transform, _Sprites)
	{}
	FEnemy(FTransform _Transform, FSprite _Sprites)
		: Creature({ 10,500,500 }, _Transform, _Sprites)
	{}
};

