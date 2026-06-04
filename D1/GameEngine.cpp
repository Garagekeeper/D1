#include "GameEngine.h"

void GameEngine::Init()
{
	SInputManager	= new InputManager();
	SWorldManager	= new WorldManager();
	SRenderer		= new Renderer();
	SWorldManager->Init();
	SRenderer->Init();
}

void GameEngine::Run()
{
	SInputManager->ClearInput();
	SInputManager->GetInput();
	SWorldManager->Update();
}
