#pragma once
#include "Screen.h"
#include "Transform.h"

class WorldManager;

class Renderer
{
private:
	std::vector<std::vector<bool>>			MiniMap;

public:
	void									Init(const WorldManager* World);
	void									RenderGamePlay(const WorldManager* World);
	void									RenderGamePause(const WorldManager* World);
	void									RenderBeforeGame(const WorldManager* World);

	void									ClearScreen();
	void									Draw2DGrid(const WorldManager* World);
	void									DrawCeiling(const WorldManager* World);
	void									DrawFloor(const WorldManager* World);
	void									Draw3DGrid(const WorldManager* World);
	void									DrawWall(const WorldManager* World);
	void									DrawSprite(const WorldManager* World);
	void									DrawObject(const WorldManager* World);
	void									DrawPlayer(const WorldManager* World);
	void									DrawEnemy(const WorldManager* World);
	void									DrawInfo(const WorldManager* World);
	void									DrawPlayerHud(const WorldManager* World);
	void									SortSprite(vector<int>* OrderVec, vector<double>* DistVec, int Amount);
	void									DrawWallVer(wchar_t Wchar, int X, int DrawStart, int DrawEnd, const int Attribute = SCREEN_TEXT_COLOR_WHITE);
	void									DrawPauseMenu(const WorldManager* World);
	void									DrawMainMenu(const WorldManager* World);
	void									DrawSceneBorder(const WorldManager* World);
	void									DrawMiniMap(const WorldManager* World);
};

