#pragma once
#include "InputManager.h"
#include "WorldManager.h"
#include "Renderer.h"
class GameEngine
{
private:
	static GameEngine*			Instance;
	InputManager*				SInputManager;
	WorldManager*				SWorldManager;
	Renderer*					SRenderer;

	double						deltaTime = 0.0;


								GameEngine() = default;
								~GameEngine();
public:
					

	void						Init();
	void						Run();
	void static					StartEngine();
	inline double				GetDeltaTime() const { return deltaTime; }
	inline InputManager*		GetInputManager() const { return SInputManager; }
	inline WorldManager*		GetWorldManager() const { return SWorldManager; }
	inline Renderer*			GetRenderer() const { return SRenderer; }
	inline static GameEngine*	GetInstance() { return Instance; }

};

