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

const double AnimBlinkSpeed = 20.0;
const int BlinkFrequency = 2;

struct FKeyState
{
	bool QKey = false;

	bool LeftArrowDown = false;
	bool LeftArrowUP = true;

	bool RightArrowDown = false;
	bool RightArrowUP = true;
	
	bool UpArrowWasDown = false;
	bool UpArrowDown = false;
	bool UpArrowUP = true;
	
	bool DownArrowWasDown = false;
	bool DownArrowDown = false;
	bool DownArrowUP = true;
	
	bool KEYSpaceWasDown = false;
	bool KEYSpaceDown = false;
	bool KEYSpaceUp = false;
	
	bool KEYEscapeWasDown = false;
	bool KEYEscapeDown = false;
	
	bool KEYW = false;
	bool KEYD = false;
	bool KEYS = false;
	bool KEYA = false;
	bool KEYR = false;
};

enum class EPauseMenu
{
	None = 0,
	Resume,
	Exit,
	EPauseMenuLen
};

enum class EMainMenu
{
	None = 0,
	StartGame,
	ExitGame,
	EMainMenuLen
};

enum class EGameState
{
	BeforeGame,
	InGame,
	Pause
};
enum class Env
{
	EMPTY = 0,
	WALL = 1,
	PLAYER = 9,
};
