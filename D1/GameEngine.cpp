#include <fstream>
#include <sstream>
#include <algorithm>
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

	std::string RecordDataString = ReadClearRecords();
	ParseRecord(RecordDataString);
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
		//deltaTime = static_cast<double>(CurrentTime.QuadPart - PrevTime.QuadPart) / Frequency.QuadPart;
		//TODO 반드시 완성전에 deltaTime을 원상복구 해놔
		deltaTime = 0.001;
		PrevTime = CurrentTime;

		SInputManager->ClearInput();
		SInputManager->GetInput();
		LoopByState();
		
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

void GameEngine::ParseRecord(std::string DataString)
{
	if (DataString.empty())
		return;

	std::stringstream Ss(DataString);
	std::string RowString;
	while (Ss >> RowString)
	{
		std::string Buf;
		std::vector<std::string> Info;
		std::stringstream Ss2(RowString);
		while (getline(Ss2, Buf, ','))
		{
			Info.push_back(Buf);
		}
		// 점수 이름순으로 넣기
		ClearRecordVec.push_back(make_pair(stoi(Info[0]), Info[1]));
	}

	sort(ClearRecordVec.begin(), ClearRecordVec.end(),
		[](std::pair<int, std::string> A, std::pair<int, std::string>B)
		{
			if (A.first == B.first)
				return A.second < B.second;

			return A.first > B.first;
		});
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

std::string GameEngine::ReadClearRecords()
{
	std::ifstream InputFile(ClearRecordsPath);
	if (InputFile.is_open())
	{
		std::string FileTexts((std::istreambuf_iterator<char>(InputFile)),std::istreambuf_iterator<char>());

		InputFile.close();
		return FileTexts;
	}

	return std::string();
}

void GameEngine::WriteRecord(const int Score, const std::string Name)
{
	std::ofstream OutFile(ClearRecordsPath, std::ios::app);
	if (OutFile.is_open())
	{
		OutFile << Score << "," << Name << '\n';
	}
	else
	{
		printf("파일 생성 실패");
	}
}
