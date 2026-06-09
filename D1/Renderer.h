#pragma once
#include "Screen.h"
#include "Transform.h"

class WorldManager;

class Renderer
{
private:

public:
	void				Init();
	void				RenderGamePlay(const WorldManager* World);
	void				RenderGamePause(const WorldManager* World);

	void				ClearScreen();
	void				Draw2DGrid(const WorldManager* World);
	void				DrawCeiling(const WorldManager* World);
	void				DrawFloor(const WorldManager* World);
	void				Draw3DGrid(const WorldManager* World);
	void				DrawWall(const WorldManager* World);
	void				DrawSprite(const WorldManager* World);
	void				DrawObject(const WorldManager* World);
	void				DrawPlayer(const WorldManager* World);
	void				DrawEnemy(const WorldManager* World);
	void				DrawInfo(const WorldManager* World);
	void				DrawPlayerHud(const WorldManager* World);
	void				SortSprite(vector<int>* OrderVec, vector<double>* DistVec, int Amount);
	void				DrawWallVer(wchar_t Wchar, int X, int DrawStart, int DrawEnd, const int Attribute = SCREEN_TEXT_COLOR_WHITE);
	void				DrawPauseMenu(const WorldManager* World);
};

