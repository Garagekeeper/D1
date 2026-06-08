#pragma once
#include "Creature.h"

class WorldManager;

class FPlayer : public Creature
{
private:
	ECreatureState									PrevState = ECreatureState::Idle;

public:

public:
	//constructor
	FPlayer(FCreatureBaseStat _Stat, FTransform _Transform, FSprite _Sprites)
		: Creature(_Stat, _Transform, _Sprites) {}
	FPlayer(FTransform _Transform, FSprite _Sprites)
		: Creature({ 100,100,100 }, _Transform, _Sprites) {}

	void Update(const WorldManager* World);
	void Roate(const WorldManager* World);
	void Move(const WorldManager* World);
	void UpdateState();
	void UpdateStateBehavior(const WorldManager* World);

};