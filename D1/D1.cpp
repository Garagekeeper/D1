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
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
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
	const int WIDTH = 120;
	const int HEIGHT = 30;
	for (int x = 0; x < WIDTH; x++)
	{
	/*
	y	
		\	     /
		 \ ____ /
		  \    /
		   \  /
							x
	여기서 카메라 플레인 (가로선)의 x범위를 -1 ~ 1로 만들어준다. 
	dir + Plane에 이 감소된 범위를 곱해서 방향을 결정해준다
	음수는 카메라의 왼쪽, 양수는 카메라의 오른쪽, 0은 정 중앙
	*/
		for (int x = 0; x < WIDTH; x++)
		{
			double cameraX = 2 * x / double(WIDTH) - 1;
			double rayDirX = player.dirX + player.PlaneX * cameraX;
			double rayDirY = player.dirY + player.PlaneY * cameraX;

			// 현재 우리가 서 있는 위치
			int MapPosX = (int)player.x;
			int MapPosY = (int)player.y;

			// ray가 출발해서 처음으로 x에 수직인 선을 만난 위치까지의 거리
			// ray가 출발해서 처음으로 y에 수직은 선을 만난 위치까지의 거리
			double sideDistX;
			double sideDistY;

			// ray가 그 다음으로 x축에 수직인 선을 만났을 때 처음 만났던점에서 지금까지의 거리
			// ray가 그 다음으로 y축에 수직인 선을 만났을 때 처음 만났던점에서 지금까지의 거리
			
			// 그림으로 보면 직각 삼각형 형태로 피타고라스로 계산할 수있다.
			// deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX))
			// deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY))
			
			// 이를 단순화하면 (계산하고 정리하면 이렇게 됨)
			// deltaDistX = abs(|rayDir|(길이) / rayDirX)
			// deltaDistY = abs(|rayDir|(길이) / rayDirY)
			
			// 여기서 한 술 더 떠서 |rayDir|을 1로 게산해 버리는데 DDA알고리즘에서
			// 길이가 중요한게 아니라 비율이 중요한거라 둘다 길이로 나눠버린다고 한다.
			// 0으로 나눌 수는 없으니까 큰 값을 넣어서 사실상 0으로 만든다.
			double deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1 / rayDirX);
			double deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1 / rayDirY);
			// 나중에 Ray의 거리를 구하는데 사용
			double perpWallDist;

			//DDA는 반복할 때마다 맵을 정확히 한칸씩 이동하는데 (대각선 안됨)
			//한칸 넘었을 떄 X에 닿았는지 Y에 닿았는지 둘 중 하나만 먼저 발생
			//이동 방향은 ray의 방향에따라서 결정되고, 그방향을 여기에 저장한다.
			int stepX;
			int stepY;

			// 벽에 부딪혔나?
			int hit = 0;
			// X에 수직선에 감지? Y수직선에 감지?
			// X쪽이면 0, Y쪽이면 1
			int side;

			//초기 sideDist 계산
			//현재 위치에서 가장 가까운 다음 격자선까지 raY가 얼마나 가야하는가
			if (rayDirX < 0)
			{
				stepX = -1;
				// 왼쪽 방향으로 가면
				// 처음 x를 만날때 까지의 실제 거리 * deltaDistX 인데
				// deltaDistX는 다음과 같다 x가 +- 1증가할 때 ray의 전체 길이는 얼마나 증가했나?
				// 원래는 그런데 길이를 나눠나서 모호하게 보일 수 있음
				sideDistX = (player.x - MapPosX) * deltaDistX;
			}
			else
			{
				stepX = 1;
				sideDistX = (MapPosX + 1.0 - player.x) * deltaDistX;
			}
			
			if (rayDirY < 0)
			{
				stepY = -1;
				sideDistY = (player.y - MapPosY) * deltaDistY;
			}
			else
			{
				stepY = 1;
				sideDistY = (MapPosY + 1.0 - player.y) * deltaDistY;
			}

			// 진짜 DDA시작
			// 매 루프마다 격자 1칸을 이동(충돌할 때 까지)
			while (hit == 0)
			{
				//다음 격자로 이동, x나 y 방향으로
				if (sideDistX < sideDistY)
				{
					// 직전에 x를 먼저 만났으면 x 쪽으로 이동
					// 직전에 Y를 먼저 만났으면 Y 쪽으로 이동

					// ray 이동
					sideDistX += deltaDistX;
					// 격자 이동
					MapPosX += stepX;
					// 직전에 x를 먼저 만나서 0으로 표시
					side = 0;
				}
				else
				{
					sideDistY += deltaDistY;
					MapPosY += stepY;
					side = 1;
				}

				// 해당 격자에 벽이 있는지 확인, 있으면 종료
				if (worldMap[MapPosX][MapPosY] == 1) hit = 1;
			}

			// DDA가 끝나면 실제 Ray의 거리를 계산
			// 유클리드 효과거리를 사용하면 어안효과고 있다고 하는데.. 잘 몰루
			//		플레이어의 위치를 기준으로 유클리드 하면
			//		ㅁㅁㅁ
			//        p
			//		이 상황에서 p에서 각 벽까지의 거리가 다 달라서 중간게 제일 커보이고
			//		나머지는 작아보임 이러면 어안처럼 둥글게 보인다
			// 카메라 평면을 사용하면 
			//      ㅁㅁㅁ
			//      
			//      ------- 
			// 평면에서 벽까지의 수직거리를 측정해서 어느 점에서나 같은 값이 나온다.
			// 그래서 어안효과 사라짐.
			// perpWallDist이 값이 벽에 수직인 거리를 말하는 것

			// 벽을 발견했다? -> 벽에 들어와있다(한칸 더 갔다)
			// 한칸 뒤로가자 (ray도 온 만큼 뒤돌아가자)
			// 근데 왜 한칸 뒤로 가는게 실제 수직 거리이죠?
			//		아까 double deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1 / rayDirX);
			//		여기서 길이를 1로 바꿔버려서 대각선 성분이 없어지고 수직 성분만 남았다고!
			//		(솔직히 좀 놀라움)

			if (side == 0)
				perpWallDist = (sideDistX - deltaDistX);
			else
				perpWallDist = (sideDistY - deltaDistY);

			// 화면에 그릴 높이 계산 (가까우면 길게)
			int lineHeight = (int)(HEIGHT / perpWallDist);

			//세로로 벽을 그리는데, 그리기 시작하는 위치와 끝내는 위치를 정함
			int drawStart = -lineHeight / 2 + HEIGHT / 2;
			if (drawStart < 0)drawStart = 0;
			int drawEnd = lineHeight / 2 + HEIGHT / 2;
			if (drawEnd >= HEIGHT)drawEnd = HEIGHT - 1;

			//TODO Draw velLine

		}
	}
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
	player = { 22, 12, -1, 0, 0, 0.66 };
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
	//DrawPlayer();
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
	double time = 0;
	double oldTime = 0;

	Init();
	while (true)
	{
		//Input();
		//Update();
		Render();
		ClearInput();
	}
}


