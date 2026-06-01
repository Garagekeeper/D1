#pragma once
#include "Creature.h"

class FPlayer : public Creature
{
public:
	//constructor
	FPlayer(FCreatureBaseStat _Stat, FPos _Pos, FVec _DirVec, double PlaneSize);
		
};