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

typedef struct FKeyState
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
}FKeyState;

typedef struct FPrevKeyInfo
{
	bool PrevLeftArrow = false;
	bool PrevRightArrow = false;
	bool PrevDownrrow = false;
	bool PrevUpArrow = false;

}FPrevKeyInfo;

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

enum Env
{
	EMPTY = 0,
	WALL = 1,
	PLAYER = 9,
};

enum class ERayCastLayer
{
	WALL,
	Creature,
};

struct FRaycasterResult
{
	bool bHit = false;
	FPos MapPos = { 0.0f, 0.0f };
	double PerpDist = 0.0;
	int Side = 0;
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
FRaycasterResult DDA(int X, ERayCastLayer TargetLayer);
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




