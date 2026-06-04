#pragma once
#include "InputManager.h"
#include "WorldManager.h"
#include "Renderer.h"
class GameEngine
{
private:
	InputManager*	SInputManager;
	WorldManager*	SWorldManager;
	Renderer*		SRenderer;
public:
	void			Init();
	void			Run();
};

