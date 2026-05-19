#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <Windows.h>

using namespace std;

typedef struct Player
{
	// player pos
	double x;
	double y;

	// player dir vec
	double dirX;
	double dirY;

	// camera plane
	double PlaneX;
	double PlaneY;
}Player;

COORD GotoXY(int x, int y);

