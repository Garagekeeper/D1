#include "Renderer.h"
#include "Define.h"
#include "Player.h"// 이 순서가 중요
#include "WorldManager.h"

void Renderer::Init()
{
	GScreen.Init();
}

void Renderer::Render(const WorldManager* World)
{
	ClearScreen();
	Draw3DGrid(World);
	//DrawEnemy(World);
	//DrawInfo(World);
	//DrawPlayerHud(World);
}

void Renderer::ClearScreen()
{
	GScreen.ChangeScreenBuffer();
}

void Renderer::DrawFloor(const WorldManager* World)
{
	// 벽의 레이캐스팅은 X를 돌면서 벽에 ray를 쏴서 거리를 뽑아오고 그 거리에 비례해서 세로선을 그림
	// 바닥의 레이캐스트는 현재 row(y)과 카메라 사이의 거리비를 구한다.(-1 < dist < 1)
	//		- 최대 거리는 화면 정중앙	(height / 2)
	//		- 최소는 화면 최하단 (플레이어의 발 바로 앞) (height)
	//		- 이 값을 기준으로 현재 값을 뽑아 낼 수 있음
	//		- current Dist = (0.5 * Height) / (y - Height / 2) (-1 =< Dist <= 1)
	// 해당 거리를 통해서 실제 그 거리를 가진 맵 위의 점을 찾고 

	FTransform* PlayerTransform = World->GetPlayer()->GetTransform();
	double PlayerDirX = PlayerTransform->GetDirVec().DirX;
	double PlayerDirY = PlayerTransform->GetDirVec().DirY;

	double CameraDirX = PlayerTransform->GetCameraDirVec().DirX;
	double CameraDirY = PlayerTransform->GetCameraDirVec().DirY;

	float PlayerPosX = PlayerTransform->GetPos().X;
	float PlayerPosY = PlayerTransform->GetPos().Y;

	const int WIDTH = GScreen.HorSize;
	const int HEIGHT = GScreen.VerSize;

	for (int Y = HEIGHT / 2; Y < HEIGHT; Y++)
	{
		// 플레이어 시야의 왼쪽 끝
		double Ray_DirLeftEndX = PlayerDirX - CameraDirX;
		double Ray_DirLeftEndY = PlayerDirY - CameraDirY;

		// 플레이어 시야의 오른쪽 끝
		double Ray_DirRightEndX = PlayerDirX + CameraDirX;
		double Ray_DirRightEndY = PlayerDirY + CameraDirY;



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
		int CurrentY = Y - HEIGHT / 2;
		// 카메라 평면에서 현재 Y(row) 까지의 거리
		// Y가 Height/2일 때 무한(1) (가로 길이는 0)
		// Y가 Height일 때 1(0) (가로 길이는 width)
		//double HorDistFromCamToFloorRatio = CameraY / CurrentY;
		double HorDistFromCamToFloorRatio = CameraY / CurrentY;

		// x=0에서 x=width 로 한칸 이동할 때마다 X와 Y의 이동량 (delta)
		double FloorStepX = HorDistFromCamToFloorRatio * (Ray_DirRightEndX - Ray_DirLeftEndX) / WIDTH;
		double FloorStepY = HorDistFromCamToFloorRatio * (Ray_DirRightEndY - Ray_DirLeftEndY) / WIDTH;

		// 시야각의 맨 왼쪽 방향의 방향 벡터쪽의 바닥(현재 시야각에서 가장 왼쪽 바닥)
		double FloorX = PlayerPosX + HorDistFromCamToFloorRatio * Ray_DirLeftEndX;
		double FloorY = PlayerPosY + HorDistFromCamToFloorRatio * Ray_DirLeftEndY;

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

void Renderer::DrawWall(const WorldManager* World)
{
	const int WIDTH = GScreen.HorSize;
	const int HEIGHT = GScreen.VerSize;

	for (int X = 0; X < WIDTH; X++)
	{
		int OutSide = 0;
		FRaycasterResult DDARes = DDA(X, ERayCastLayer::WALL, World);
		double PerpWallDist = DDARes.PerpDist;

		// 화면에 그릴 높이 계산 (가까우면 길게)
		int LineHeight = (int)(HEIGHT / PerpWallDist);

		//세로로 벽을 그리는데, 그리기 시작하는 위치와 끝내는 위치를 정함
		//사실 식이 어떻게 되는지는 잘 몰루
		int DrawStart = -LineHeight / 2 + HEIGHT / 2;
		if (DrawStart < 0)DrawStart = 0;
		int DrawEnd = LineHeight / 2 + HEIGHT / 2;
		if (DrawEnd >= HEIGHT)DrawEnd = HEIGHT - 1;

		int Attribute = SCREEN_TEXT_COLOR_WHITE;

		if (PerpWallDist < 3.0)
			Attribute = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; // 아주 밝은 흰색
		else if (PerpWallDist < 6.0)
			Attribute = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; // 일반 흰색 (회색 느낌)
		else if (PerpWallDist < 12.0)
			Attribute = FOREGROUND_INTENSITY; // 어두운 회색
		else
			Attribute = 0; // 거리가 너무 멀면 검은색(안개 속으로 사라짐)

		wchar_t WallChar = L' ';
		if (PerpWallDist < 2)
			WallChar = L'█';
		else if (PerpWallDist < 4)
			WallChar = L'▓';
		else if (PerpWallDist < 8)
			WallChar = L'▒';
		else
			WallChar = L'░';

		// 현재 X좌표에서 만난 벽까지의 거리
		GScreen.Zbuffer[X] = PerpWallDist;
		//DrawWallVer((OutSide == 1) ? L'\u2588' : L'\u2593', X, DrawStart, DrawEnd, Attribute);
		//DrawWallVer(WallChar, X, DrawStart, DrawEnd, Attribute);
	}
}

void Renderer::Draw3DGrid(const WorldManager* World)
{
	//DrawCeiling();
	DrawFloor(World);
	DrawWall(World);
}

FRaycasterResult Renderer::DDA(const int X, ERayCastLayer TargetLayer, const WorldManager* World)
{
	FRaycasterResult Res;

	const int WIDTH = GScreen.HorSize;
	const int HEIGHT = GScreen.VerSize;
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

	FTransform* PlayerTransform = World->GetPlayer()->GetTransform();
	double PlayerDirX = PlayerTransform->GetDirVec().DirX;
	double PlayerDirY = PlayerTransform->GetDirVec().DirY;

	double CameraDirX = PlayerTransform->GetCameraDirVec().DirX;
	double CameraDirY = PlayerTransform->GetCameraDirVec().DirY;

	float PlayerPosX = PlayerTransform->GetPos().X;
	float PlayerPosY = PlayerTransform->GetPos().Y;

	double CameraX = 2 * X / double(WIDTH) - 1;
	double RayDirX = PlayerDirX + CameraDirX * CameraX;
	double RayDirY = PlayerDirY + CameraDirY * CameraX;

	// 현재 우리가 서 있는 위치
	int MapPosX = (int)PlayerTransform->GetPos().X;
	int MapPosY = (int)PlayerTransform->GetPos().Y;

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
		SideDistX = (PlayerPosX - MapPosX) * DeltaDistX;
	}
	else
	{
		StepX = 1;
		SideDistX = (MapPosX + 1.0 - PlayerPosX) * DeltaDistX;
	}

	if (RayDirY < 0)
	{
		StepY = -1;
		SideDistY = (PlayerPosY - MapPosY) * DeltaDistY;
	}
	else
	{
		StepY = 1;
		SideDistY = (MapPosY + 1.0 - PlayerPosY) * DeltaDistY;
	}

	// 진짜 DDA시작
	// 매 루프마다 격자 1칸을 이동(충돌할 때 까지)
	while (Res.bHit == false)
	{
		if (MapPosX <= 0 || MapPosX - 1 >= World->mapWidth) break;
		if (MapPosY <= 0 || MapPosY - 1 >= World->mapHeight) break;
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
			Res.Side = 0;
		}
		else
		{
			SideDistY += DeltaDistY;
			MapPosY += StepY;
			Res.Side = 1;
		}

		if (TargetLayer == ERayCastLayer::WALL)
		{
			// 해당 격자에 벽, 오브젝트, 적이 있는지 확인
			if (World->GetWorldMap()[MapPosY][MapPosX] == static_cast<int>(Env::WALL)) Res.bHit = true;
		}
		else if (TargetLayer == ERayCastLayer::Creature)
		{
			if (World->GetCreatureMap()[MapPosY][MapPosX].empty()) Res.bHit = true;
		}
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

	if (Res.Side == 0)
		Res.PerpDist = (SideDistX - DeltaDistX);
	else
		Res.PerpDist = (SideDistY - DeltaDistY);

	if (Res.PerpDist < 0.001)
		Res.PerpDist = 0.001;

	return Res;
}
