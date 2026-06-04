#include "GameEngine.h"

// 싱글톤은 선언이 끝이 아니라 실제 선언까지 해줘야함\
// 정확히 말하면 static 멤버는 선언과 생성이 다 되어야함
GameEngine* GameEngine::Instance = nullptr;

GameEngine::~GameEngine()
{
	delete SInputManager;
	SInputManager = nullptr;

	delete SWorldManager;
	SWorldManager = nullptr;

	delete SRenderer;
	SRenderer = nullptr;
}

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
	LARGE_INTEGER Frequency;
	LARGE_INTEGER PrevTime;
	LARGE_INTEGER CurrentTime;

	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&PrevTime);
	while (true)
	{
		QueryPerformanceCounter(&CurrentTime);
		deltaTime = static_cast<double>(CurrentTime.QuadPart - PrevTime.QuadPart) / Frequency.QuadPart;
		//DeltaTime = 0.001;
		PrevTime = CurrentTime;

		SInputManager->ClearInput();
		SInputManager->GetInput();
		SWorldManager->Update();
		SRenderer->Render(SWorldManager);
	}
	
}

void GameEngine::StartEngine()
{
	if (Instance == nullptr)
	{
		Instance = new GameEngine();
	}
}
