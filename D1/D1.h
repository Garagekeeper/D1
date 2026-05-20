#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <Windows.h>

using namespace std;

typedef struct FPlayer
{
	// player pos
	double X;
	double Y;

	// player dir vec
	double DirX;
	double DirY;

	// camera plane
	double PlaneX;
	double PlaneY;
}FPlayer;

COORD GotoXY(int x, int y);

