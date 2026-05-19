#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <Windows.h>

using namespace std;

typedef struct Player
{
	int x;
	int y;
}Player;

COORD GotoXY(int x, int y);

