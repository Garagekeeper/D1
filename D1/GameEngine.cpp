#include "GameEngine.h"

// 싱글톤은 선언이 끝이 아니라 실제 선언까지 해줘야함\
// 정확히 말하면 static 멤버는 선언과 생성이 다 되어야함
GameEngine* GameEngine::Instance = nullptr;

GameEngine::~GameEngine()
{

	if (SInputManager)
	{
		delete SInputManager;
		SInputManager = nullptr;
	}

	if (SWorldManager)
	{
		delete SWorldManager;
		SWorldManager = nullptr;
	}

	if (SRenderer)
	{
		delete SRenderer;
		SRenderer = nullptr;
	}

	if (GScreen)
	{
		delete GScreen;
		GScreen = nullptr;
	}
}

void GameEngine::Init()
{
	SInputManager = new InputManager();
	SWorldManager = new WorldManager();
	SRenderer = new Renderer();
	GScreen = new Screen();

	if(SInputManager)
		SWorldManager->Init();

	if (SWorldManager)
		SRenderer->Init(SWorldManager);

	if(GScreen)
		GScreen->Init();
}

void GameEngine::GameRun()
{
	LARGE_INTEGER Frequency;
	LARGE_INTEGER PrevTime;
	LARGE_INTEGER CurrentTime;

	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&PrevTime);
	//TODO 조건을 정해서 while을 탈출할 수 있도록
	while (!IsExit)
	{
		QueryPerformanceCounter(&CurrentTime);
		deltaTime = static_cast<double>(CurrentTime.QuadPart - PrevTime.QuadPart) / Frequency.QuadPart;
		//DeltaTime = 0.001;
		PrevTime = CurrentTime;

		SInputManager->ClearInput();
		SInputManager->GetInput();
		LoopByState();
		AmountTime += deltaTime;
	}
}

void GameEngine::LoopByState()
{
	if (GameState == EGameState::BeforeGame)
	{
		SWorldManager->UpdateBeforeGameLoop();
		SRenderer->RenderBeforeGame(SWorldManager);
	}
	else if (GameState == EGameState::InGame)
	{
		SWorldManager->UpdateGameLoop();
		SRenderer->RenderGamePlay(SWorldManager);

		if (SInputManager->GetKeyState().KEYEscapeDown)
		{
			IsPause = !IsPause;
			GameState = EGameState::Pause;
		}
	}
	else if (GameState == EGameState::Pause)
	{
		SWorldManager->UpdatePauseLoop();
		SRenderer->RenderGamePause(SWorldManager);

		if (SInputManager->GetKeyState().KEYEscapeDown)
		{
			IsPause = !IsPause;
			GameState = EGameState::InGame;
		}
	}
}

// 이래 하는게 맞는지..?
void GameEngine::ClearEngine()
{
	if (Instance != nullptr)
	{
		delete Instance;
	}

	Instance = nullptr;
}

void GameEngine::StartEngine()
{
	if (Instance == nullptr)
	{
		Instance = new GameEngine();
	}
}
