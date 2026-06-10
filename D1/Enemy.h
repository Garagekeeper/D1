#pragma once
#include "Creature.h"
#include "vector"
#include "list"

class WorldManager;
class FEnemy : public Creature
{

private:
	const float		EnemyPatrolPerCentage = 0.3f;
	const float		EnemyIdlePerCentage = 0.5f;
	const float		AttackDelayMax = 1;
	int				RewardScore = 10;
	int				FindDepth = 10;
	float			PatrolPercentage = EnemyPatrolPerCentage;
	double			DetectRadius = 5.0;
	double			AttackRange = 1.1;
	double			CurrentAttackDelay = AttackDelayMax;
	bool			bCanAttack = true;

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
	void			UpdateState(WorldManager* World);
	bool			CheckPlayerInDetectRange(WorldManager* World);
	bool			CheckPlayerInAttackRange(WorldManager* World);
	virtual void	GetDamage(int Amount, Creature* From) override;
};

