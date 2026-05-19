#include "D1.h"
#include "Screen.h"
// 추후 키 입력을 winAPI로 변경시 제거
#include <stdio.h>
#include <conio.h>

typedef struct KeyState
{
	bool QKey = false;
	bool LeftArrow = false;
	bool RightArrow = false;
	bool UpArrow = false;
	bool DownArrow = false;
}KeyState;

Player player;
Screen screen;
KeyState keyState;

//vector<wstring> Grid =
//{
//	L"■■■■■■■■■■■■■■■■",
//	L"■	             ■",
//	L"■	             ■",
//	L"■	             ■",
//	L"■	             ■",
//	L"■	             ■",
//	L"■	             ■",
//	L"■	             ■",
//	L"■	             ■",
//	L"■■■■■■■■■■■■■■■■",
//};


#define mapWidth 24
#define mapHeight 24
#define screenWidth 640
#define screenHeight 480

int worldMap[mapWidth][mapHeight] =
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,1,0,1,0,1,0,0,0,1},
  {1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1},
  {1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,1,1,0,1,1,0,0,0,0,1,0,1,0,1,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

enum EDir
{
	LEFT = 75,
	UP = 72,
	RIGHT = 77,
	DOWN = 80,
};

enum Env
{
	EMPTY	= 0,
	WALL	= 1, 
	PLAYER	= 9,
};

void DrawPlayer()
{
	screen.SetPixel(L"※", GotoXY(player.x, player.y));
}

void DrawGrid()
{
	for (int i = 0; i < 24; i++)
	{
		COORD pos = { 0, i };
		wstring str;
		for (int j = 0; j < 24; j++)
		{
			if (worldMap[i][j] == 1)
				str.append(1 ,L'■');
			else 
				str.append(1, ' ');
		}
		screen.SetPixel(str, pos);
	}
}

void ClearScreen()
{
	screen.ChangeScreenBuffer();
}

void Init()
{
	player = { 11, 11 };
	screen.Init();
}



void Input()
{
	int InKey = 0;
	int XDir = 0;
	int YDir = 0;
	// TODO 키 입력을 winAPI로 변경
	if (_kbhit())
	{
		InKey = _getch();
		if (InKey == 224)
			InKey = _getch();


		if (InKey == 'q')
			keyState.QKey = true;
		if (InKey == UP)
			keyState.UpArrow = true;
		if (InKey == RIGHT)
			keyState.RightArrow = true;
		if (InKey == DOWN)
			keyState.DownArrow = true;
		if (InKey == LEFT)
			keyState.LeftArrow = true;

	}
}

void HandleInput(int* Dx, int* Dy)
{
	if (keyState.QKey)
		exit(0);

	if (keyState.UpArrow)		*Dy -= 1;
	if (keyState.RightArrow)	*Dx += 1;
	if (keyState.DownArrow)		*Dy += 1;
	if (keyState.LeftArrow)		*Dx -= 1;
}

void PlayerMove(int Dx, int Dy)
{
	int Nx = player.x + Dx;
	int Ny = player.y + Dy;

	// 콘솔 바운더리를 넘어가지는 못한다.
	if (Nx >= screen.horSize || Nx < 0) return;
	if (Ny >= screen.verSize || Ny < 0) return;
	// 벽을 지나칠 수는 없다.
	if (worldMap[Ny][Nx] == WALL) return;
	
	int CurrX = player.x;
	int CurrY = player.y;
	worldMap[CurrY][CurrX] = EMPTY;

	player.x = Nx;
	player.y = Ny;
	worldMap[Ny][Nx] = PLAYER;
}

void Update()
{
	int Dx = 0;
	int Dy = 0;
	HandleInput(&Dx, &Dy);
	PlayerMove(Dx, Dy);
}

void Render()
{
	DrawGrid();
	DrawPlayer();
	ClearScreen();
}

COORD GotoXY(int x, int y)
{
	COORD pos = { (short)x, (short)y };
	return pos;
}

void ClearInput()
{
	keyState.QKey		= false;
	keyState.UpArrow	= false;
	keyState.RightArrow = false;
	keyState.DownArrow	= false;
	keyState.LeftArrow	= false;
}

int main()
{
	Init();
	while (true)
	{
		Input();
		Update();
		Render();
		ClearInput();
	}
}


