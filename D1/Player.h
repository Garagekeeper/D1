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
	FSprite WeponHudSprite = 
	{
		0,
		0,
		0,
		{
			{
				L"        ⠶        ",
				L"     ⡔⠉⠉⠉⠉⠉⢢     ",
				L"    ⢰⠁⠄⠄⠄⠄⠠⠈⡆    ",
				L"    ⡥⠭⢭⠉⠉⠉⡭⠭⢬    ",
				L"   ⣌⣀⣛⣸⣤⣤⣤⣇⣛⣀⣡   ",
				L"  ⣸⣉⣉⣉⣉⣉⣉⣉⣉⣉⣉⣉⣇  ",
				L"  ⡇⣿⣿⡞⠉⠉⠉⠉⠉⢳⣿⣿⢸  ",
				L"  ⡇⣿⣿⡇⠄⠄⠄⠄⠄⢸⣿⣿⢸  ",
				L"  ⡇⣿⣿⡇⠄⠄⠄⠄⠄⢸⣿⣿⢸  ",
				L"  ⡇⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⢸  ",
				L"   ⠙⢿⣿⣿⣿⣿⣿⣿⣿⡿⠋   ",
				L"    ⢸⣿⣿⣿⣿⣿⣿⣿⡇    ",
			},

			{
				L"    ⣿⣿⣿⣿⠶⣿⣿⣿⣿    ",
				L" ⣿⣿⣿⣿⡔⠉⠉⠉⠉⠉⢢⣿⣿⣿⣿ ",
				L" ⣿⣿⣿⢰⠁⠄⠄⠄⠄⠠⠈⡆⣿⣿⣿ ",
				L" ⣿⣿⣿⡥⠭⢭⠉⠉⠉⡭⠭⢬⣿⣿⣿ ",
				L"⣿⣿⣿⣌⣀⣛⣸⣤⣤⣤⣇⣛⣀⣡⣿⣿⣿",
				L"⣿⣿⣸⣉⣉⣉⣉⣉⣉⣉⣉⣉⣉⣉⣇⣿⣿",
				L"  ⡇⣿⣿⡞⠉⠉⠉⠉⠉⢳⣿⣿⢸  ",
				L"  ⡇⣿⣿⡇⠄⠄⠄⠄⠄⢸⣿⣿⢸  ",
				L"  ⡇⣿⣿⡇⠄⠄⠄⠄⠄⢸⣿⣿⢸  ",
				L"  ⡇⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⢸  ",
				L"   ⠙⢿⣿⣿⣿⣿⣿⣿⣿⡿⠋   ",
				L"    ⢸⣿⣿⣿⣿⣿⣿⣿⡇    ",
			}
		}
	};
};


class FPlayer : public Creature
{
public:
	FWeaponHud WeaponHud;

public:
	//constructor
	FPlayer(FCreatureBaseStat _Stat, FPos _Pos, FVec _DirVec, double PlaneSize);
	FPlayer(FPos _Pos, FVec _DirVec, double PlaneSize);

	inline FWeaponHud* GetWeaponHud() { return &WeaponHud; }

};