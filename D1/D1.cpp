#include "D1.h"
#include "Screen.h"
#include "Player.h"
#include "Sprite.h"
// 추후 키 입력을 winAPI로 변경시 제거
#include <stdio.h>
#include <conio.h>
#include <sstream>

double MoveBaseSpeed = 5.0;
double RotationSpeed = 5.0;

const double PHI = 3.14159265358979323846;
double DeltaTime = 0.0;
const int NumOfSprite = 1;
const int SpriteTextureTest_RowSize = 12;

FPlayer			Player;
Screen			GScreen;
FKeyState		KeyState;
FPrevKeyInfo	PrevKeyInfo;
FSprite*		Sprites;
vector<int>		SpriteOrder;
vector<double>	SpriteDistance;

#define mapWidth 24
#define mapHeight 24
int WorldMap[mapWidth][mapHeight] =
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

void Init()
{
	Player = { 18, 12, 
		0.0, -1.0, 
		1.0, 0.0, 
		0.66, 0.0, 
		0 };

	Sprites = new FSprite[NumOfSprite]
	{
		{
			5.5, 2.5, 0, new const wchar_t* [SpriteTextureTest_RowSize]
			{
			L"▒▒▒▒▒▒▒▒▒▒▒▒",
			L"▒▒▒▒▒▒▒▒▒▒▒▒",
			L"▒▒▒▒▓▒▒▓▒▒▒▒",
			L"▒▒▒▒▓▒▒▓▒▒▒▒",
			L"▒▒▒▒▓▒▒▓▒▒▒▒",
			L"▒▒▒▒▒▒▒▒▒▒▒▒",
			L"▒▒▒▒▒▒▒▒▒▒▒▒",
			L"▒▒▒▒▒▒▒▒▒▒▒▒",
			L"▒▓▒▒▒▒▒▒▒▒▓▒",
			L"▒▒▓▒▒▒▒▒▒▓▒▒",
			L"▒▒▒▓▓▓▓▓▓▒▒▒",
			L"▒▒▒▒▒▒▒▒▒▒▒▒",

			}
		},
		/*{
			5.5, 2.5, 0, new const wchar_t* [SpriteTextureTest_RowSize]
			{
				L"                           ",
				L"                           ",
				L"                           ",
				L"                           ",
				L"                           ",
				L"                ⡖⡆⡴⡒⡄      ",
				L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⡆⣇⠱⡷          ",
				L"⠀⠀⠀⠀⣀⣀⡤⠤⠼⢻⣿⡀⣧⣤⣄⠀⢇          ",
				L"⢠⣿⣿⣿⣿⡆⠀⠀⠀⠐⠿⠇⠙⠿⠂⠀⠘⡄         ",
				L"⢸⣿⣿⣿⣿⣟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⡇        ",
				L"⠘⢿⣿⣿⣿⡏⠀⠀⠀⠀⠀⠀⠀⢀⠠⢀⠇⢇         ",
				L"⠀⠈⠙⠻⢏⣀⣀⡀⠀⠀⠠⢤⢖⠊⠁⠀⢸          ",
				L"⠀⠀⠀⠀⠀⠀⠀⣸⠤⣀⣈⣉⣉⢤⣴⡾⠛⠢⡀        ",
				L"⠀⠀⠀⠀⠀⠀⢰⢱⠀⠀⠉⠉⠉⠉⠀⢰⠇⠀⢁        ",
				L"⠀⠀⠀⠀⠀⠀⠰⢦⠀⠀⠀⠀⠀⠀⠀⠀⡆⡄⠀⡆       ",
				L"⠀⠀⠀⠀⠀⠀⠐⢬⠦⣤⣤⢄⣠⣠⣤⢼⡀⠀⡰        ",
				L"⠀⠀⠀⠀⠀⠀⠀⠈⡆⠈⠻⠿⡭⠁⠀⠀⠩⠉⡇        ",
				L"⠀⠀⠀⠀⠀⠀⠀⠀⠱⠠⡀⠀⣃⠀⡀⠀⠀⢰⡇        ",
				L"⠀⠀⠀⠀⠀⠀⠀⠀⠘⠤⠀⠀⠸⠄⣈⣀⣀⣽⠃        ",
				L"⠀⠀⠀⠀⠀⠀⠀⠀⣤⣶⣤⣤⣤⠂⠀⡄⣤⣤⡆        ",
				L"⠀⠀⠀⠀⠀⠀⠀⢊⠀⠀⠙⠿⣭⠟⠛⠿⣯⣷⡅        ",
				L"⠀⠀⠀⠀⠀⠀⠀⠈⠑⠒⠒⠉⠙⢂⣀⣀⡊⠟         ",
				L"                           ",
				L"                           ",
				L"                           ",
				L"                           ",
				L"                           ",
				L"                           ",
			}
		},*/

		/*{
			22.5, 11.5, 0,new const wchar_t* [SpriteTextureTest_RowSize]
			{
				{L"###" },
				{L"# #" },
				{L"###" },
			}	
		},

		{
			19.5, 3.5,16, new const wchar_t* [SpriteTextureTest_RowSize]
			{
				{L"@@@" },
				{L"@ @" },
				{L"@@@" },
			}
		},*/

	};

	SpriteOrder = vector<int>(NumOfSprite);
	SpriteDistance = vector<double>(NumOfSprite);
	

	GScreen.Init();
}

void Input()
{
	int InKey = 0;
	int XDir = 0;
	int YDir = 0;

	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		KeyState.UpArrowDown = true;
	}
	if (GetAsyncKeyState(VK_UP) & 1)
	{
		KeyState.UpArrowDown = false;
	}


	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		KeyState.RightArrowDown = true;
	}
	else
	{
		KeyState.RightArrowDown = false;
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		KeyState.DownArrowDown = true;
	}
	else
	{
		KeyState.DownArrowDown = false;
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		KeyState.LeftArrowDown = true;
	}
	else
	{
		KeyState.LeftArrowDown = false;
	}

	if (GetAsyncKeyState('Q') & 0x8000) exit(0);
	if (GetAsyncKeyState('W') & 0x8001)	KeyState.KEYW = true;
	if (GetAsyncKeyState('D') & 0x8001)	KeyState.KEYD = true;
	if (GetAsyncKeyState('S') & 0x8001)	KeyState.KEYS = true;
	if (GetAsyncKeyState('A') & 0x8001)	KeyState.KEYA = true;
}

void Update()
{
	HandleInput();
	PlayerRotate();
	PlayerMove();
}

void Render()
{
	//Draw2dGrid();
	//DrawPlayer();
	Draw3dGrid();
	DrawSprite();
	DrawInfo();
	ClearScreen();
}

void ClearInput()
{
	KeyState.KEYW = false;
	KeyState.KEYD = false;
	KeyState.KEYS = false;
	KeyState.KEYA = false;
}

void HandleInput()
{
	if (KeyState.QKey)
		exit(0);
}

void PlayerRotate()
{

	// 회전 처리
	// TODO 상하 처리는 나중에 하거나 안넣거나
	//if (KeyState.UpArrow)			*Dy -= MoveSpeed;
	//if (KeyState.DownArrow)		*Dy += MoveSpeed;
	//// 아래는 45도 기준으로 
	//double Theta = 45.0;
	//if (KeyState.LeftArrowDown && !PrevKeyInfo.PrevLeftArrow)
	//{
	//	Theta *= -1.0;
	//	Player.PlayerTheta += Theta;
	//}
	//if (KeyState.RightArrowDown && !PrevKeyInfo.PrevRightArrow)
	//{
	//	Player.PlayerTheta += Theta;
	//}

	//Player.SetDirVec();
	//PrevKeyInfo.PrevLeftArrow = KeyState.LeftArrowDown;
	//PrevKeyInfo.PrevRightArrow = KeyState.RightArrowDown;

	// Theta * deltaTime을 곱하는 방식
	// TODO 회전을 하면 할 수록 벽의 높이가 작아짐
	double Theta = 35.0;
	double FinalTheta = 0.0;
	if (!KeyState.LeftArrowDown && !KeyState.RightArrowDown) return;

	if (KeyState.LeftArrowDown)
	{
		FinalTheta -= Theta * RotationSpeed * DeltaTime;
	}

	if (KeyState.RightArrowDown)
	{
		FinalTheta += Theta * RotationSpeed * DeltaTime;
	}

	double FinalRad = FinalTheta * PHI / 180.0;


	// 회전 변환
	// 플레이어가 바라보는 방향 회전
	// 이런 실수는 하지 말자, 위에서  x 변경하고 밑에서 바로 사용하면 의미가 왜곡됨
	// 이런 방식은 오차가 누적됨
	//Player.DirX = Player.DirX * cos(FinalRad) - Player.DirY * sin(FinalRad);
	//Player.DirY = Player.DirX * sin(FinalRad) + Player.DirY * cos(FinalRad);

	auto NextDirX = Player.DirX * cos(FinalRad) - Player.DirY * sin(FinalRad);
	auto NextDirY = Player.DirX * sin(FinalRad) + Player.DirY * cos(FinalRad);

	auto Len = sqrt(NextDirX * NextDirX + NextDirY * NextDirY);

	Player.DirX = NextDirX / Len;
	Player.DirY = NextDirY / Len;

	// 플레이어의 오른쪽을 가리키는 벡터 변환
	///auto NextRightX = Player.RightX * cos(FinalRad) - Player.RightY * sin(FinalRad);
	//auto NextRightY = Player.RightX * sin(FinalRad) + Player.RightY * cos(FinalRad);

	Player.RightX = -Player.DirY;
	Player.RightY = Player.DirX;

	//카메라 평면 변환
	//카메라 평면은 Right와 평행
	//auto NextPlaneX = Player.PlaneX * cos(FinalRad) - Player.PlaneY * sin(FinalRad);
	//auto NextPlaneY = Player.PlaneX * sin(FinalRad) + Player.PlaneY * cos(FinalRad);

	const double PlaneSize = 0.66;

	Player.PlaneX = Player.RightX * PlaneSize;
	Player.PlaneY = Player.RightY * PlaneSize;

	Player.PlayerTheta += FinalTheta;
}

void PlayerMove()
{
	double Dx = 0.0;
	double Dy = 0.0;
	double theta = 45;
	double DirSize = pow(Player.DirX, 2) + pow(Player.DirY, 2);
	DirSize = sqrt(DirSize);

	double RightSize = pow(Player.RightX, 2) + pow(Player.RightY, 2);
	RightSize = sqrt(RightSize);
	// 현재 바라보는 방향을 기준으로 앞뒤좌우 (상대적임)
	// 이동 처리
	if (!KeyState.KEYW && !KeyState.KEYD && !KeyState.KEYS && !KeyState.KEYA) return;
	if (KeyState.KEYW)
	{
		Dx = (Player.DirX / DirSize) * MoveBaseSpeed * DeltaTime;
		Dy = (Player.DirY / DirSize) * MoveBaseSpeed * DeltaTime;
	}
	if (KeyState.KEYD)
	{
		Dx = (Player.RightX / RightSize) * MoveBaseSpeed * DeltaTime;
		Dy = (Player.RightY / RightSize) * MoveBaseSpeed * DeltaTime;
	}
	if (KeyState.KEYS)
	{
		Dx = (Player.DirX / DirSize) * MoveBaseSpeed * DeltaTime * -1.0;
		Dy = (Player.DirY / DirSize) * MoveBaseSpeed * DeltaTime * -1.0;
	}
	if (KeyState.KEYA)
	{
		Dx = (Player.RightX / RightSize) * MoveBaseSpeed * DeltaTime * -1.0;
		Dy = (Player.RightY / RightSize) * MoveBaseSpeed * DeltaTime * -1.0;
	}



	double ColliderRadius = 0.3;

	// 다음 좌표
	double NextX = Player.X + Dx;
	double NextY = Player.Y + Dy;

	// 다음 좌표의 충돌 체크
	double ColliderEdgeX = NextX + (Dx > 0 ? ColliderRadius : -ColliderRadius);
	if (WorldMap[(int)Player.Y][(int)ColliderEdgeX] != WALL)
	{
		Player.X = NextX;
	}

	double ColliderEdgeY = NextY + (Dy > 0 ? ColliderRadius : -ColliderRadius);
	if (WorldMap[(int)ColliderEdgeY][(int)Player.X] != WALL)
	{
		Player.Y = NextY;
	}

	// 콘솔 바운더리를 넘어가지는 못한다.
	double MapMin = ColliderRadius;
	double MapMaxX = GScreen.HorSize - 0.3;
	double MapMaxY = GScreen.VerSize - 0.3;

	if (Player.X < ColliderRadius) Player.X = ColliderRadius;
	if (Player.X > MapMaxX) Player.X = MapMaxX;
	if (Player.Y < ColliderRadius) Player.Y = ColliderRadius;
	if (Player.Y > MapMaxY) Player.Y = MapMaxY;
}

void DrawPlayer()
{
	GScreen.PrintString(L"※", (int)Player.X, (int)Player.Y);
}

void Draw2dGrid()
{
	//Draw topDown Grid
	for (int i = 0; i < 24; i++)
	{
		wstring str;
		for (int j = 0; j < 24; j++)
		{
			if (WorldMap[i][j] == 1)
				str.append(1, L'■');
			else
				str.append(1, ' ');
		}
		GScreen.PrintString(str, 0, i);
	}
}

double DDA(int X, const int WIDTH, const int HEIGHT, int& Side)
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

	double CameraX = 2 * X / double(WIDTH) - 1;
	double RayDirX = Player.DirX + Player.PlaneX * CameraX;
	double RayDirY = Player.DirY + Player.PlaneY * CameraX;

	// 현재 우리가 서 있는 위치
	int MapPosX = (int)Player.X;
	int MapPosY = (int)Player.Y;

	// ray가 출발해서 처음으로 x에 수직인 선을 만난 위치까지의 거리
	// ray가 출발해서 처음으로 y에 수직은 선을 만난 위치까지의 거리
	double SideDistX;
	double SideDistY;

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
	double DeltaDistX = (RayDirX == 0) ? 1e30 : std::abs(1 / RayDirX);
	double DeltaDistY = (RayDirY == 0) ? 1e30 : std::abs(1 / RayDirY);
	// 나중에 Ray의 거리를 구하는데 사용
	double PerpWallDist;

	//DDA는 반복할 때마다 맵을 정확히 한칸씩 이동하는데 (대각선 안됨)
	//한칸 넘었을 떄 X에 닿았는지 Y에 닿았는지 둘 중 하나만 먼저 발생
	//이동 방향은 ray의 방향에따라서 결정되고, 그방향을 여기에 저장한다.
	int StepX;
	int StepY;

	// 벽에 부딪혔나?
	bool bHit = false;
	// X에 수직선에 감지? Y수직선에 감지?
	// X쪽이면 0, Y쪽이면 1

	//초기 sideDist 계산
	//현재 위치에서 가장 가까운 다음 격자선까지 raY가 얼마나 가야하는가
	if (RayDirX < 0)
	{
		StepX = -1;
		// 왼쪽 방향으로 가면
		// 처음 x를 만날때 까지의 실제 거리 * deltaDistX 인데
		// deltaDistX는 다음과 같다 x가 +- 1증가할 때 ray의 전체 길이는 얼마나 증가했나?
		// 원래는 그런데 길이를 나눠나서 모호하게 보일 수 있음
		SideDistX = (Player.X - MapPosX) * DeltaDistX;
	}
	else
	{
		StepX = 1;
		SideDistX = (MapPosX + 1.0 - Player.X) * DeltaDistX;
	}

	if (RayDirY < 0)
	{
		StepY = -1;
		SideDistY = (Player.Y - MapPosY) * DeltaDistY;
	}
	else
	{
		StepY = 1;
		SideDistY = (MapPosY + 1.0 - Player.Y) * DeltaDistY;
	}

	// 진짜 DDA시작
	// 매 루프마다 격자 1칸을 이동(충돌할 때 까지)
	while (bHit == false)
	{
		//다음 격자로 이동, x나 y 방향으로
		if (SideDistX < SideDistY)
		{
			// 직전에 x를 먼저 만났으면 x 쪽으로 이동
			// 직전에 Y를 먼저 만났으면 Y 쪽으로 이동

			// ray 이동
			SideDistX += DeltaDistX;
			// 격자 이동
			MapPosX += StepX;
			// 직전에 x를 먼저 만나서 0으로 표시
			Side = 0;
		}
		else
		{
			SideDistY += DeltaDistY;
			MapPosY += StepY;
			Side = 1;
		}

		// 해당 격자에 벽이 있는지 확인, 있으면 종료
		if (WorldMap[MapPosY][MapPosX] == WALL) bHit = true;
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

	if (Side == 0)
		PerpWallDist = (SideDistX - DeltaDistX);
	else
		PerpWallDist = (SideDistY - DeltaDistY);

	if (PerpWallDist < 0.001)
		PerpWallDist = 0.001;

	return PerpWallDist;
}

void DrawWallVer(wchar_t Wchar, int X, int DrawStart, int DrawEnd)
{
	int Length = DrawEnd - DrawStart;
	GScreen.PrintVer(Wchar, X, DrawStart, Length);
}

void DrawCeiling()
{
	const int WIDTH = GScreen.HorSize;
	const int HEIGHT = GScreen.VerSize;
}

void DrawFloor()
{
	// 벽의 레이캐스팅은 X를 돌면서 벽에 ray를 쏴서 거리를 뽑아오고 그 거리에 비례해서 세로선을 그림
	// 바닥의 레이캐스트는 현재 row(y)과 카메라 사이의 거리비를 구한다.(-1 < dist < 1)
	//		- 최대 거리는 화면 정중앙	(height / 2)
	//		- 최소는 화면 최하단 (플레이어의 발 바로 앞) (height)
	//		- 이 값을 기준으로 현재 값을 뽑아 낼 수 있음
	//		- current Dist = (0.5 * Height) / (y - Height / 2) (-1 =< Dist <= 1)
	// 해당 거리를 통해서 실제 그 거리를 가진 맵 위의 점을 찾고 
	const int WIDTH = GScreen.HorSize;
	const int HEIGHT = GScreen.VerSize;

	for (int Y = HEIGHT / 2; Y < HEIGHT; Y++)
	{
		// 플레이어 시야의 왼쪽 끝
		double Ray_DirLeftEndX	= Player.DirX - Player.PlaneX;
		double Ray_DirLeftEndY	= Player.DirY - Player.PlaneY;

		// 플레이어 시야의 오른쪽 끝
		double Ray_DirRightEndX = Player.DirX + Player.PlaneX;
		double Ray_DirRightEndY = Player.DirY + Player.PlaneY;



		/*
		0		Ceiling
				------------------------------------
		       /
			  /
			 /
			/
		Cam  < - Height / 2;
			\
			 \
			  \
			   \
				------------------------------------- <- HEight
		Height		Floor

		높이가 Height / 2일때 거리 = 1 (무한)
		그렇다면 CurrentY일때 거리는 = 1/x (반비례 관계라서)
		1 : posZ = 1/x : p (반비례 관계니깐) 
		posZ/x = p;
		posZ/p = x;
		----
		\   | -
		 \  | CurrentY
		  \ | -
		   -
		*/

		// 현재 카메라 평면의 Y좌표
		double CameraY = 0.5 * HEIGHT;
		// 카메라 평면을 기준으로 현재 Y 좌표가 얼마나 떨어져 있는지
		int CurrentY = Y - HEIGHT/2;
		// 카메라 평면에서 현재 Y(row) 까지의 거리
		// Y가 Height/2일 때 무한(1) (가로 길이는 0)
		// Y가 Height일 때 1(0) (가로 길이는 width)
		//double HorDistFromCamToFloorRatio = CameraY / CurrentY;
		double HorDistFromCamToFloorRatio = CameraY / CurrentY;

		// x=0에서 x=width 로 한칸 이동할 때마다 X와 Y의 이동량 (delta)
		double FloorStepX = HorDistFromCamToFloorRatio * (Ray_DirRightEndX - Ray_DirLeftEndX) / WIDTH;
		double FloorStepY = HorDistFromCamToFloorRatio * (Ray_DirRightEndY - Ray_DirLeftEndY) / WIDTH;

		// 시야각의 맨 왼쪽 방향의 방향 벡터쪽의 바닥(현재 시야각에서 가장 왼쪽 바닥)
		double FloorX = Player.X + HorDistFromCamToFloorRatio * Ray_DirLeftEndX;
		double FloorY = Player.Y + HorDistFromCamToFloorRatio * Ray_DirLeftEndY;

		for (int X = 0; X < WIDTH; X++)
		{
			// 바닥 좌표의 정수부만 취함 (월드 좌표, 월드 내부에서 벡터가 가리키는 좌표)
			int CellX = static_cast<int>(FloorX);
			int CellY = static_cast<int>(FloorY);

			// 결국 화면의 x,y가 Map의 Cellx, Celly의 지점을 그리고 있다 라고 해석이 가능
			// 그래서 아래와 같이 체크무늬 모양도 가능

			// 실제 출력할 좌표
			int checkerBoard = (std::abs(CellX) + std::abs(CellY)) % 2;
			wchar_t tileChar = (checkerBoard == 0) ? L'·' : L' ';
			GScreen.PrintChar(tileChar, X, Y);

			FloorX += FloorStepX;
			FloorY += FloorStepY;
		}
	}
}

void DrawWall()
{
	const int WIDTH = GScreen.HorSize;
	const int HEIGHT = GScreen.VerSize;
	
	for (int X = 0; X < WIDTH; X++)
	{
		int OutSide = 0;
		double PerpWallDist = DDA(X, WIDTH, HEIGHT, OutSide);

		// 화면에 그릴 높이 계산 (가까우면 길게)
		int LineHeight = (int)(HEIGHT / PerpWallDist);

		//세로로 벽을 그리는데, 그리기 시작하는 위치와 끝내는 위치를 정함
		//사실 식이 어떻게 되는지는 잘 몰루
		int DrawStart = -LineHeight / 2 + HEIGHT / 2;
		if (DrawStart < 0)DrawStart = 0;
		int DrawEnd = LineHeight / 2 + HEIGHT / 2;
		if (DrawEnd >= HEIGHT)DrawEnd = HEIGHT - 1;

		// 현재 X좌표에서 만난 벽까지의 거리
		GScreen.Zbuffer[X] = PerpWallDist;
		DrawWallVer((OutSide == 1) ? L'\u2588' : L'\u2593', X, DrawStart, DrawEnd);
	}
}

void Draw3dGrid()
{
	// 천장과 바닥이 벽보다 먼저 그려져야 함
	//DrawCeiling();
	DrawFloor();
	DrawWall();
}

void SortSprite(vector<int>* OrderVec, vector<double> *DistVec, int Amount)
{
	vector<pair<double, int>> Sprites(Amount);
	for (int i = 0; i < Amount; i++)
	{
		Sprites[i].first = (*DistVec)[i];
		Sprites[i].second = (*OrderVec)[i];
	}

	sort(Sprites.begin(), Sprites.end());
	for (int i = 0; i < Amount; i++)
	{
		(*DistVec)[i] = Sprites[Amount - i - 1].first;
		(*OrderVec)[i] = Sprites[Amount - i - 1].second;
	}
}

void DrawSprite()
{

	/*
	Sprite render 기초
	1: While raycasting the walls, store the perpendicular distance of each vertical stripe in a 1D ZBuffer
	2: Calculate the distance of each sprite to the player
	3: Use this distance to sort the sprites, from furthest away to closest to the camera
	4: Project the sprite on the camera plane (in 2D): subtract the player position from the sprite position, then multiply the result with the inverse of the 2x2 camera matrix
	5: Calculate the size of the sprite on the screen (both in x and y direction) by using the perpendicular distance
	6: Draw the sprites vertical stripe by vertical stripe, don't draw the vertical stripe if the distance is further away than the 1D ZBuffer of the walls of the current stripe
	7: Draw the vertical stripe pixel by pixel, make sure there's an invisible color or all sprites would be rectangles
	*/

	// sortSprite Far to Close (가장 먼거부터 그려야, 가려질 수 있음, 앞에서부터 그리면 전부 다 나옴)
	// 벡터 거리 저장
	// 벡터 우선순위 초기화

	for (int i = 0; i < NumOfSprite; i++)
	{
		SpriteOrder[i] = i;
		SpriteDistance[i] = ((Player.X - Sprites[i].X) * (Player.X - Sprites[i].X) + (Player.Y - Sprites[i].Y) * (Player.Y - Sprites[i].Y));
	}

	SortSprite(&SpriteOrder, &SpriteDistance, NumOfSprite);

	for (int i = 0; i < NumOfSprite; i++)
	{
		auto *CurrentSprite = &Sprites[SpriteOrder[i]];

		// 카메라에서 스프라이트 까지의 상대적인 위치
		double SpriteX = CurrentSprite->X - Player.X;
		double SpriteY = CurrentSprite->Y - Player.Y;

		// 카메라 행렬(카메라 좌표계)
		// 카메라의 정면 (스크린 안쪽)은 플레이어의 방향벡터
		// 카메라의 오른쪽 (스크린의 오른쪽)은 카메라 평면 벡터
		// 카메라 좌표계 기준 정면으로 1 이동한건 월드 입장에서는 1 * 플레이어 방향벡터 만큼 이동한 것
		// 카메라 좌표계 기준 우측으로 1 이동한건 월드 입장에서는 1 * 카메라 평면의 방향벡터 만큼 이동한 것

		// 카메라 기준으로 Cx, Cy 이동 했으면 최종 월드좌표 Wx,Wy 는 다음과 같다.
		//[Wx]   [PlaneX  DirX]   [Cx]	Wx = PlaneX * Cx + DirX * Cy
		//[  ] = [            ] x [  ]
		//[Wy]   [PlaneY  DirY]   [Cy]	Wx = PlaneY * Cx + DirY * Cy
		// 
		// 여기서 Wx를 계산하는데, DirX * Cy는 왜 들어가는지 고민했는데, 이건 카메라 자체가 회전된 상태일 때 유효하다
		// 카메라가 회전하지 않았다면 초기 방향에따라서 DirX혹은 DirY가 0이다. (방향 벡터가 (1,0) (-1,0) 이런식으로 나오니까)
		// 그래서 (1,0)인 경우에 Wx = PlaneX * Cx + 0 * Cy가 되어서 Wx = PlaneX 가 된다. 하지만 방향 벡터가 회전했다면 
		// 예를 들어 (1,1)이라고 하면 카메라 평면은 (1,-1)이된다. 여기서 카메라의 좌표가 (0,0) 에서 (1,1)이 되면
		// 카메라 기준으로는 오른쪽으로 한칸 안쪽으로 한칸 이동한다. 이걸 월드 입장에서 보면 (1,-1) 방향으로 이동 후 (1,1) 방향으로 이동한게 된다.
		// 맨 처음에는 PlaneX * Cx 만큼 이동하고 1,1 방향으로 이동할 때, DirX * Cy 만큼 이동한다
		//          y  ▲
		//             |  ↘(Plane 방향)    ↗ (Dir 방향) 
		//	           |    ↘            ↗
		//             |      ↘        ↗
		//             |        ↘    ↗
		//             |          ↘↗
		//             -------------------▶ x
		//  방금 말한 상황이 이 그림인데 Wx는 Plane의 x방향으로 Cx만큼 이동한 것 +  Dir의 x방향으로 Cy 이동한 것 
		//  Wy는 Plane의 y방향으로 Cx만큼 이동, 으로 이동한 것 + dir의 y Cy만큼 이동방향으로 이동
		//  상대위치에 카메라의 역행렬을 곱해서 카메라 좌표의 X,Y를 구할 수 있다.(Y는 깊이, 화면 안쪽으로 들어가는)
		//transform sprite with the inverse camera matrix
		// [ planeX   dirX ] -1                                       [ dirY      -dirX ]
		// [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
		// [ planeY   dirY ]                                          [ -planeY  planeX ]

		double invDet = 1.0 / (Player.PlaneX * Player.DirY - Player.DirX * Player.PlaneY);

		// 카메라 중심에서 좌우로 얼마나 떨어져 있는가
		double transformX = invDet * (Player.DirY * SpriteX - Player.DirX * SpriteY); // 화면의 좌우 (양수면 오른쪽)
		// 카메라 중심에서 앞뒤로 얼마나 떨어져 있는가.
		double transformY = invDet * (-Player.PlaneY * SpriteX + Player.PlaneX * SpriteY); // 화면 안쪽으로 얼마나 들어가 있는지

		// ????
		// 원근투영
		// 어떤 물체가 왼쪽으로 2미터 떨어진 위치에 있다.
		// 화면에 가까울때는 화면을 많이 돌려야하고, 크게 보인다.
		// 화면에서 멀때는 화면의 중앙에 가까운곳에 작게 보인다.
		// transformX / transformY 이게 화면에서 멀수록 가운데로오게 해준다.
		// 이 값은 -1 < X < 1인데 화면에는 음수 좌표계가 없으니까 +1
		// 0 < x < 2 범위의 X를 0~130까지의 정수로 변환
		// 스프라이트가 찍힐 X 좌표
		int SpriteScrrenX = int((GScreen.HorSize / 2) * (1 + transformX / transformY));

		//--------------------------
		//Scailing					|
		//--------------------------
		// 세로로 몇배 줄일건지
		const float uDiv = .25;
		// 가로로 몇배 줄일건지
		const float vDiv = .5;
		// 위로 몇칸 갈건지 
		// 이 값들은 스프라이트마다 가지고 있으면 좋을 듯 함;
		int vMoveScrren = int(-CurrentSprite->VMove / transformY);


		// 스프라이트의 높이
		// 어안 렌즈 방지를 위해 실제 거리 말고 transformY 사용
		// 스프라이트의 높이가 화면에 들어가 있을수록 작아짐( 플레이어로 부터 멀리 있을수록 작아짐)
		int SpriteHeight = abs(int(GScreen.VerSize / transformY)) / vDiv;
		//세로 비율 조정

		int DrawStartY = -SpriteHeight / 2 + GScreen.VerSize / 2 + vMoveScrren;
		if (DrawStartY < 0) DrawStartY = 0;
		int DrawEndY = SpriteHeight / 2 + GScreen.VerSize / 2 + vMoveScrren;
		if (DrawEndY >= GScreen.VerSize) DrawEndY = GScreen.VerSize - 1;

		// 스프라이트의 너비
		int SpriteWidth = abs(int(GScreen.VerSize / transformY)) / uDiv;

		int DrawStartX = -SpriteWidth / 2 + SpriteScrrenX;
		if (DrawStartX < 0) DrawStartX = 0;
		int DrawEndX = SpriteWidth / 2 + SpriteScrrenX;
		if (DrawEndX >= GScreen.HorSize) DrawEndX = GScreen.HorSize;

		for (int Stripe = DrawStartX; Stripe < DrawEndX; Stripe++)
		{
			for (int j = DrawStartY; j < DrawEndY; j++)
			{
				// 현재 픽셀이 텍스쳐의 가로에서 몇번째인지 확인
				// (현재 위치- 시작 위치) * 텍스쳐 크기 / 전체 너비
				int texX = int(256 * (Stripe - (-SpriteWidth / 2 + SpriteScrrenX)) * SpriteTextureTest_RowSize / SpriteWidth) / 256;
				//int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * texWidth / spriteWidth) / 256;

				// 경계 안으로 들어 오도록
				if (texX < 0) texX = 0;
				if (texX >= SpriteTextureTest_RowSize) texX = SpriteTextureTest_RowSize - 1;

				// 1. transformY이 0이하면 화면의 뒤쪽
				// 2. i가 화면에 있는지
				// 3. 벽보다 가까이 있는지
				if (transformY > 0 && Stripe >= 0 && Stripe < GScreen.HorSize && transformY < GScreen.Zbuffer[Stripe])
				{
					//256 and 128 factors to avoid floats 실수를 피하기 위해서 이걸 곱했다는데 잘 몰루
					int d = (j- vMoveScrren) * 256 - GScreen.VerSize * 128 + SpriteHeight * 128;
					int texY = ((d * SpriteTextureTest_RowSize) / SpriteHeight) / 256;

					// 경계 안으로 들어 오도록
					if (texY < 0) texY = 0;
					if (texY >= SpriteTextureTest_RowSize) texY = SpriteTextureTest_RowSize - 1;


					wchar_t SpriteChar = CurrentSprite->SpriteTexture[texY][texX];


					GScreen.PrintChar(SpriteChar, Stripe, j );

					//// 공백 처리 (텍스처 배열에서 ' ' 즉, 빈 공간은 투명화 처리하여 그리지 않음)
					//if (SpriteChar != L' ')
					//{
					//	// GScreen의 i(가로), j(세로) 좌표에 글자(spriteChar)를 그리는 함수를 호출하세요.
					//	// 예시: GScreen.Buffer[j][i] = spriteChar;
					//	GScreen.PrintChar(SpriteChar, Stripe, j);
					//}
				}
			}
		}
	}
}

void DrawInfo()
{
	wstringstream Wss;
	// 소수점 아래 자리 고정
	Wss.fixed;
	Wss.precision(3);

	int FPS = static_cast<int>(1 / DeltaTime);
	wstring Wstr = to_wstring(FPS);

	Wss << L"FPS : " << Wstr
		<< L"| Pos (" << Player.X << L", " << Player.Y << L")"
		<< L"| Theta : " << Player.PlayerTheta << L"°";

	GScreen.PrintString(Wss.str(), 0, 0);

}

void ClearScreen()
{
	GScreen.ChangeScreenBuffer();
}

int main()
{
	//system("mode con cols=240 lines=60");

	// delta time을위한  타이머(chrono)
	LARGE_INTEGER Frequency;
	LARGE_INTEGER PrevTime;
	LARGE_INTEGER CurrentTime;

	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&PrevTime);

	Init();
	while (true)
	{
		QueryPerformanceCounter(&CurrentTime);
		DeltaTime = static_cast<double>(CurrentTime.QuadPart - PrevTime.QuadPart) / Frequency.QuadPart;
		PrevTime = CurrentTime;

		Input();
		Update();
		Render();
		ClearInput();
	}

	delete[] Sprites;
	Sprites = nullptr;

	//TODO
	/*

	최적화
	움직임 + 회전 시 프레임 드랍 보완 방법
	2. 회전 연산(삼각함수)의 오버헤드 체크
	3. DeltaTime 독립성 및 입력 루프 분리

	//TODO
	3. color 설정
		글자 무늬만으로는 원근감을 주는 데 한계가 있습니다. Windows API의 SetConsoleTextAttribute나 VT 시퀀스를 이용해 색상을 도입하는 단계입니다.

		구현 방법: 거리가 멀어질수록 벽과 바닥의 색상을 회색 ➡️ 어두운 회색 ➡️ 검은색으로 바꾸는 거리 기반 안개 효과(Fog Effect / Shading)를 줍니다.

		효과: 기호로만 보였던 그래픽에 실시간 음영이 들어가면서 갑자기 고전 패키지 게임 같은 엄청난 비주얼로 업그레이드됩니다.
	4. 게임 방향성 정하기
		- 슈팅
		- 맵 탐험 요소가 있는 턴제 JRPG
	
	
	3. 콘솔 크기 조절시키기 win11은 고정인듯 함.
*/


}
