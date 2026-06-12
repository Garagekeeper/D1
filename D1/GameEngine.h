#pragma once

#include "InputManager.h"
#include "WorldManager.h"
#include "Renderer.h"


class GameEngine
{
private:
	static GameEngine*			Instance;

	InputManager*				SInputManager = nullptr;
	WorldManager*				SWorldManager = nullptr;
	Renderer*					SRenderer = nullptr;
	Screen*						GScreen = nullptr;
	EGameState					GameState = EGameState::BeforeGame;


	double						deltaTime = 0.0;
	bool						IsPause = false;
	bool						IsExit = false;
	bool						IsExitComplete = false;


								GameEngine() = default;
								~GameEngine();
public:
					

	void						Init();
	void						GameRun();
	void						LoopByState();
	void static					ClearEngine();
	void static					StartEngine();
	inline double				GetDeltaTime() const { return deltaTime; }

	inline InputManager*		GetInputManager() const { return SInputManager; }
	inline WorldManager*		GetWorldManager() const { return SWorldManager; }
	inline Renderer*			GetRenderer() const { return SRenderer; }
	inline static GameEngine*	GetInstance() { return Instance; }
	inline Screen*				GetScreen() { return GScreen; }
	inline bool					GetIsPuase() { return IsPause; }
	inline void					SetIsPuase(bool InValue) {IsPause = InValue; }
	inline bool					GetIsExit() { return IsExit; }
	inline void					SetIsExit(bool InValue) { IsExit = InValue; }
	inline bool					GetIsExitComplete() { return IsExitComplete; }
	inline void					SetIsExitComplete(bool InValue) { IsExitComplete = InValue; }
	inline void					SetGameState(EGameState InValue) { GameState = InValue; }

};

