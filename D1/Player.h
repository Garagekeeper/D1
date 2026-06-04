#pragma once
#include "Creature.h"

struct SpriteSize
{
	int Width;
	int Height;
};

struct FWeaponHud
{
	// HUD구조체를 sprite랑 합치기
	// https://en.wikipedia.org/wiki/Braille_Patterns 점자패턴
	
};


class FPlayer : public Creature
{
public:

public:
	//constructor
	FPlayer(FCreatureBaseStat _Stat, FTransform _Transform, FSprite _Sprites)
		: Creature(_Stat, _Transform, _Sprites) {}
	FPlayer(FTransform _Transform, FSprite _Sprites)
		: Creature({ 100,100,100 }, _Transform, _Sprites) {}

	Creature* FindTargetOnRay();

};