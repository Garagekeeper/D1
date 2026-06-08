#pragma once
#include "Creature.h"
#include "vector"
#include "list"

class WorldManager;
class FEnemy : public Creature
{

private:
	const float		EnemyPatrolPerCentage = 1.0f;
	float			PatrolPercentage = EnemyPatrolPerCentage;

public:

					FEnemy(FCreatureBaseStat _Stat, FTransform _Transform, FSprite _Sprites)
						: Creature(_Stat, _Transform, _Sprites)
					{}
					FEnemy(FTransform _Transform, FSprite _Sprites)
						: Creature({ 10,500,500 }, _Transform, _Sprites)
					{}


	void			MoveToWithArray(FPos NextPos, WorldManager* World);
	void			Update(WorldManager* World);
	void			Rotate();
	void			Move(WorldManager* World);
	void			UpdateState();

};

