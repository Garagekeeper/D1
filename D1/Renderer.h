#pragma once
#include "Screen.h"
#include "Transform.h"

class WorldManager;

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

class Renderer
{
private:
	Screen	GScreen;

public:
	void Init();
	void Render(const WorldManager* World);

	void ClearScreen();
	void Draw2DGrid(const WorldManager* World);
	void DrawCeiling(const WorldManager* World);
	void DrawFloor(const WorldManager* World);
	void Draw3DGrid(const WorldManager* World);
	void DrawWall(const WorldManager* World);
	void DrawObject(const WorldManager* World);
	void DrawPlayer(const WorldManager* World);
	void DrawEnemy(const WorldManager* World);
	void DrawInfo(const WorldManager* World);
	void DrawPlayerHud(const WorldManager* World);
	FRaycasterResult DDA(int X, ERayCastLayer TargetLayer, const WorldManager* World);
	//void DrawWallVer(wchar_t Wchar, int X, int DrawStart, int DrawEnd, const int Attribute = SCREEN_TEXT_COLOR_WHITE);

};

