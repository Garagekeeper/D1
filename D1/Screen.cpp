#include "Screen.h"

bool Screen::Init()
{
	// 버퍼를 2개 생성하고
	// 커서를 지움
	// 그 뒤에 버퍼 정보를 가져옴

	int TargetWidth = 120;
	int TargetHeight = 30;

	// 터미널 크기 키우는 코드라는데, win11에서는 동작하지 않는 듯.
	for (int i = 0; i < 2; i++)
	{
		SMALL_RECT minRect = { 0, 0, 0, 0 };
		SetConsoleWindowInfo(ScreenBuffer[i], TRUE, &minRect);

		COORD bSize = { static_cast<SHORT>(TargetWidth), static_cast<SHORT>(TargetHeight) };
		SetConsoleScreenBufferSize(ScreenBuffer[i], bSize);

		SMALL_RECT wRect = { 0, 0, static_cast<SHORT>(TargetWidth - 1), static_cast<SHORT>(TargetHeight - 1) };
		SetConsoleWindowInfo(ScreenBuffer[i], TRUE, &wRect);
	}

	// 스크린 버퍼 생성
	ScreenBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
		0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	if (ScreenBuffer[0] == INVALID_HANDLE_VALUE)
		return false;

	ScreenBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
		0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	if (ScreenBuffer[1] == INVALID_HANDLE_VALUE)
		return false;

	//커서 제거
	CONSOLE_CURSOR_INFO CursorInfo = { 1,FALSE };

	if (SetConsoleCursorInfo(ScreenBuffer[0], &CursorInfo) == false)
	{
		std::wcout << GetLastError();
		return false;
	}

	if (SetConsoleCursorInfo(ScreenBuffer[1], &CursorInfo) == false)
	{
		std::wcout << GetLastError();
		return false;
	}


	//버퍼의 정보를 info에 담아옴
	if (GetConsoleScreenBufferInfo(ScreenBuffer[0], &BufferInfo))
	{
		HorSize = BufferInfo.dwSize.X;
		VerSize = BufferInfo.dwSize.Y;
	}

	CharInfoBuffer = new CHAR_INFO[HorSize * VerSize];


	Zbuffer = vector<double>(HorSize);
	ClearScreen();

	return true;
}

void Screen::PrintString(const wstring& str, const int x, const int y, const int Attributes)
{
	for (int i = 0; i < str.length(); i++)
		PrintChar(str[i], x + i, y, Attributes);
}

void Screen::PrintChar(const wchar_t ch, const int X, const int Y, const int Attributes)
{
	// 버퍼의 크기를 벗어나면 버퍼에 쓰지 않는다.
	if (X >= 0 && X < HorSize
		&& Y >= 0 && Y < VerSize)
	{
		// 스크린버퍼에 쓰는게 아니라 내부 버퍼의 기록
		//InnerBuffer[Y * HorSize + X ] = ch;
		CharInfoBuffer[Y * HorSize + X].Char.UnicodeChar = ch;
		CharInfoBuffer[Y * HorSize + X].Attributes = Attributes;
	}
}

void Screen::PrintHor(const wchar_t ch, const int x, const int y, const int length, const int Attributes)
{
	wstring wstr(length, ch);
	PrintString(wstr, x, y, Attributes);
}

void Screen::PrintVer(const wchar_t ch, const int x, const int y, const int length, const int Attributes)
{
	for (int i = 0; i < length; i++)
	{
		PrintChar(ch, x, y + i, Attributes);
	}

}

bool Screen::ChangeScreenBuffer()
{
	DWORD dwWritenByte = 0;
	COORD BufferSize = { static_cast<SHORT>(HorSize), static_cast<SHORT>(VerSize) };
	COORD BufferCoord = { 0, 0};
	SMALL_RECT WriteRegion = { 0, 0, static_cast<SHORT>(HorSize - 1), static_cast<SHORT>(VerSize - 1) };
	
	WriteConsoleOutputW(
		ScreenBuffer[CurrentScreenBufferIndex],
		CharInfoBuffer,
		BufferSize,
		BufferCoord,
		&WriteRegion
	);

	// 쓰기 버퍼를 출력하여 읽기 버퍼로 바꾼다.
	if (SetConsoleActiveScreenBuffer(ScreenBuffer[CurrentScreenBufferIndex]) == false)
		return false;
	CurrentScreenBufferIndex = CurrentScreenBufferIndex == 0 ? 1 : 0;
	ClearScreen();

	return true;
}

void Screen::ClearScreen()
{
	// 다음 프레임에 쓸 버퍼를 초기화한다.
	// 공백문자로 채워서 초기화
	for (int i = 0; i < HorSize * VerSize; i++)
	{
		CharInfoBuffer[i].Char.UnicodeChar = ' ';
		CharInfoBuffer[i].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	}

	// Z버퍼 초기화
	for (int i = 0; i < HorSize; i++)
		Zbuffer[i] = 0.0;
}

Screen::~Screen()
{
	CloseHandle(ScreenBuffer[0]);
	CloseHandle(ScreenBuffer[1]);


	delete[] CharInfoBuffer;
	CharInfoBuffer = nullptr;
}
