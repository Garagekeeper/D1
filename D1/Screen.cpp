#include "Screen.h"

bool Screen::Init()
{
	// 버퍼를 2개 생성하고
	// 커서를 지움
	// 그 뒤에 버퍼 정보를 가져옴

	int TargetWidth = 120;
	int TargetHeight = 30;

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

	//SMALL_RECT wRect = { 0, 0,TargetWidth - 1, TargetHeight - 1 };
	//SetConsoleWindowInfo(ScreenBuffer[0], TRUE, &wRect);
	//SetConsoleWindowInfo(ScreenBuffer[1], TRUE, &wRect);

	//버퍼의 정보를 info에 담아옴
	if (GetConsoleScreenBufferInfo(ScreenBuffer[0], &BufferInfo))
	{
		HorSize = BufferInfo.dwSize.X;
		VerSize = BufferInfo.dwSize.Y;
	}

	// Screen 버퍼에 쓰기 전에 여기에 써놓고
	// Screen 버퍼에 쓸 때 배열을 한번에 출력
	InnerBuffer = new wchar_t[HorSize * VerSize];
	ClearScreen();

	return true;
}

void Screen::PrintString(const wstring& str, const COORD& position)
{
	for (int i = 0; i < str.length(); i++)
		PrintChar(str[i], { static_cast<SHORT>(position.X + i), position.Y });
}

void Screen::PrintChar(const wchar_t ch, const COORD& position)
{
	// 버퍼의 크기를 벗어나면 버퍼에 쓰지 않는다.
	if (position.X >= 0 && position.X < HorSize
		&& position.Y >= 0 && position.Y < VerSize)
	{
		// 스크린버퍼에 쓰는게 아니라 내부 버퍼의 기록
		InnerBuffer[position.Y * HorSize + position.X] = ch;
	}
}

void Screen::PrintHor(const wchar_t ch, const COORD& position, int length)
{
	wstring wstr(length, ch);
	PrintString(wstr, position);
}

void Screen::PrintVer(const wchar_t ch, const COORD& position, int length)
{
	for (int i = 0; i < length; i++)
	{
		COORD NewPos = { position.X, position.Y + i };
		PrintChar(ch, NewPos);
	}

}

bool Screen::ChangeScreenBuffer()
{
	DWORD dwWritenByte = 0;
	COORD Pos = { 0,0 };
	
	WriteConsoleOutputCharacterW(
		ScreenBuffer[CurrentScreenBufferIndex],
		InnerBuffer,
		HorSize * VerSize,
		Pos,
		&dwWritenByte
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
		InnerBuffer[i] = L' ';
}

Screen::~Screen()
{
	CloseHandle(ScreenBuffer[0]);
	CloseHandle(ScreenBuffer[1]);
}
