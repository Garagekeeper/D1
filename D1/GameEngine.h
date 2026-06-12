#pragma once
#include <set>
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
	std::vector<std::pair<int, std::string>> ClearRecordVec;
	std::string					PlayerName;


	double						deltaTime = 0.0;
	bool						IsPause = false;
	bool						IsExit = false;
	bool						IsExitComplete = false;
	const std::string			ClearRecordsPath = ".\\Data\\ClearRecords.txt";


								GameEngine() = default;
								~GameEngine();
public:
					

	void						Init();
	void						GameRun();
	void						LoopByState();
	void						ParseRecord(std::string DataString);
	void static					ClearEngine();
	void static					StartEngine();
	void						WriteRecord(const int Score, const std::string Name);
	std::string					ReadClearRecords();
	inline double				GetDeltaTime() const { return deltaTime; }

	inline InputManager*		GetInputManager() const { return SInputManager; }
	inline WorldManager*		GetWorldManager() const { return SWorldManager; }
	inline Renderer*			GetRenderer() const { return SRenderer; }
	inline static GameEngine*	GetInstance() { return Instance; }
	inline Screen*				GetScreen() { return GScreen; }
	inline bool					GetIsPuase() const { return IsPause; }
	inline void					SetIsPuase(bool InValue) {IsPause = InValue; }
	inline bool					GetIsExit() const { return IsExit; }
	inline void					SetIsExit(bool InValue) { IsExit = InValue; }
	inline bool					GetIsExitComplete() { return IsExitComplete; }
	inline void					SetIsExitComplete(bool InValue) { IsExitComplete = InValue; }
	inline void					SetGameState(EGameState InValue) { GameState = InValue; }
	inline std::set<int>*		GetClearRecord() { &ClearRecordVec; }

};

