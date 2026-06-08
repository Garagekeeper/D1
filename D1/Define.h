#pragma once

#include <Windows.h>
#include <stdlib.h>
#include <iostream>

const double PlayerMoveBaseSpeed = 5.0;
const double EnemyMoveBaseSpeed = 3.0;
const double RotationSpeed = 100.0;


// 이 타겟들은 순수 노가다를 통해서 찾음
const int TargetWidth = 138;
const int TargetHeight = 34;

const int TargetScreenWidth = 1280;
const int TargetScreenHeight = 720;


const int SCREEN_TEXT_COLOR_WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
const int SCREEN_BACKGROUND_COLOR_WHITE = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;


const double AnimDelay = 0.2;
const double PHI = 3.14159265358979323846;

struct FKeyState
{
	bool QKey = false;
	bool LeftArrowDown = false;
	bool LeftArrowUP = true;
	bool RightArrowDown = false;
	bool RightArrowUP = true;
	bool UpArrowDown = false;
	bool UpArrowUP = true;
	bool DownArrowDown = false;
	bool DownArrowUP = true;
	bool KEYSpaceWasDown = false;
	bool KEYSpaceDown = false;
	bool KEYSpaceUp = false;
	bool KEYW = false;
	bool KEYD = false;
	bool KEYS = false;
	bool KEYA = false;
	bool KEYR = false;
};

struct FPrevKeyInfo
{
	bool PrevLeftArrow = false;
	bool PrevRightArrow = false;
	bool PrevDownrrow = false;
	bool PrevUpArrow = false;

};

enum class Env
{
	EMPTY = 0,
	WALL = 1,
	PLAYER = 9,
};
