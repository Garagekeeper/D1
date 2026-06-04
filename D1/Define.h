#pragma once

#include <Windows.h>
#include <stdlib.h>
#include <iostream>

const double MoveBaseSpeed = 5.0;
const double RotationSpeed = 100.0;


// 이 타겟들은 순수 노가다를 통해서 찾음
const int TargetWidth = 138;
const int TargetHeight = 34;

const int TargetScreenWidth = 1280;
const int TargetScreenHeight = 720;


const int SCREEN_TEXT_COLOR_WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
const int SCREEN_BACKGROUND_COLOR_WHITE = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;


const double PlayerAttackCoolTime = 0.2;

// Move?
enum class ECreatureState
{
	Idle = 0,
	Attack,
	ECreatureStateLen,
};
