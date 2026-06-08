#pragma once
#include "Transform.h"

class WorldManager;

enum class ERayCastLayer
{
	WALL,
	Creature,
};

struct FRaycasterResult
{
	bool bHit = false;
	FPos MapPos = { 0.0f, 0.0f };
	double PerpDist = 0.0;
	int Side = 0;
};

FRaycasterResult DDA(int X, ERayCastLayer TargetLayer, const WorldManager* World);

