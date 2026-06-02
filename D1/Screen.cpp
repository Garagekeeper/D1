#include "Screen.h"

bool Screen::Init()
{
	// 버퍼를 2개 생성하고
	// 커서를 지움
	// 그 뒤에 버퍼 정보를 가져옴

	// 스크린 버퍼 생성
	ScreenBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
		0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	if (ScreenBuffer[0] == INVALID_HANDLE_VALUE)
		return false;

	ScreenBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
		0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	if (ScreenBuffer[1] == INVALID_HANDLE_VALUE)
		return false;

	for (int i = 0; i < 2; i++)
	{
		HANDLE ConsoleHandle = ScreenBuffer[i];

		HWND hwnd = GetConsoleWindow();
		Sleep(10);
		HWND owner = GetWindow(hwnd, GW_OWNER);
		RECT Rect = { 0, 0, TargetScreenWidth, TargetScreenHeight };

		// 창을 최대화 시키기
		//ShowWindow(owner, SW_SHOWMAXIMIZED);


		COORD bSize = { static_cast<SHORT>(TargetWidth), static_cast<SHORT>(TargetHeight) };
		SetConsoleScreenBufferSize(ConsoleHandle, bSize);


		//폰트 변경(적용 안됨)
		//CONSOLE_FONT_INFO fontInfo = { 0, {0,0} };
		//CONSOLE_FONT_INFOEX fontInfoex;
		//fontInfoex.cbSize = sizeof(fontInfoex);
		//fontInfoex.nFont = 0;
		//fontInfoex.dwFontSize.X = 4; // 가로 크기
		//fontInfoex.dwFontSize.Y = 32; // 세로 크기
		//fontInfoex.FontFamily = FF_DONTCARE;
		//fontInfoex.FontWeight = FW_NORMAL;
		//std::wstring fontName = L"Consolas";
		//wcscpy_s(fontInfoex.FaceName, fontName.c_str());
		//SetCurrentConsoleFontEx(ConsoleHandle, FALSE, &fontInfo);



		// 창크기 조절, 위에서 최대화 시키는게 제일 깔끔함
		AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, true);
		//MoveWindow(owner, 0, 0, Rect.right - Rect.left, Rect.bottom- Rect.top, true);
		SetWindowPos(owner, nullptr, 0, 0, Rect.right - Rect.left, Rect.bottom - Rect.top, SWP_NOZORDER);
		//SetWindowLong(owner, GWL_STYLE, WS_VISIBLE | WS_POPUP | WS_MAXIMIZE);
		//GetCurrentConsoleFont(ConsoleHandle, false, &fontInfo);
		//SetCurrentConsoleFontEx(ConsoleHandle, false, &fontInfoex);
	}



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
	COORD BufferCoord = { 0, 0 };
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
