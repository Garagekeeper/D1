#pragma once
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
#include <list>
#include<utility>
#include "Define.h"
#include "Transform.h"

using namespace std;

class Creature;
struct FPos;

enum EDir
{
	LEFT = 75,
	UP = 72,
	RIGHT = 77,
	DOWN = 80,
	NORTH = 'w',
	EAST = 'd',
	SOUTH = 's',
	WEST = 'a',
};


void Init();

#pragma region Game loop
void Input();
void Update();
void Render();
void ClearInput();
#pragma endregion

#pragma region Update loop
void HandleInput();
void PlayerRotate();
void PlayerMove();
void UpdatePlayer();
#pragma endregion

#pragma region Render loop
void DrawPlayer();
void Draw2dGrid();
//FRaycasterResult DDA(int X, ERayCastLayer TargetLayer);
void DrawWallVer(wchar_t Wchar, int X, int DrawStart, int DrawEnd, const int Attribute = SCREEN_TEXT_COLOR_WHITE);
void DrawCeiling();
void DrawFloor();
void DrawWall();
void Draw3dGrid();
void SortSprite(vector<int>* OrderVec, vector<double>* DistVec, int Amount);
void DrawSprite();
void DrawEnemySprite();
void DrawPlayerHUD();

void DrawInfo();
void ClearScreen();
#pragma endregion




