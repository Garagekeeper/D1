#include <sstream>
#include <algorithm>
#include <iomanip>

#include "GameEngine.h"
#include "Renderer.h"
#include "Define.h"
#include "WorldManager.h"
#include "Raycaster.h"
#include "Utils.h"

void Renderer::Init(const WorldManager* World)
{

	MiniMap = std::vector<std::vector<bool>>(World->mapHeight, std::vector<bool>(World->mapWidth, false));
}

void Renderer::RenderGamePlay(const WorldManager* World)
{
	RefreshScreen();
	// INScene
	Draw3DGrid(World);
	DrawEnemy(World);
	DrawPlayerHud(World);
	//보기에 좀 이상함..
	//DrawOnAtackEffect(World);

	// OutScene
	DrawInfo(World);
	DrawBorder(World);
	DrawMiniMap(World);

	if (World->GetIsPlayerDead())
	{
		DrawDeathMenu(World);
	}
	else if (World->GetIsClear())
	{
		DrawClearMenu(World);
	}
}

void Renderer::RenderGamePause(const WorldManager* World)
{
	RefreshScreen();
	DrawPauseMenu(World);
}

void Renderer::RenderBeforeGame(const WorldManager* World)
{
	RefreshScreen();
	DrawMainMenu(World);
}

void Renderer::RefreshScreen()
{
	GameEngine::GetInstance()->GetScreen()->ChangeScreenBuffer();
}

void Renderer::Draw2DGrid(const WorldManager* World)
{
	//Draw topDown Grid
	for (int i = 0; i < 24; i++)
	{
		wstring str;
		for (int j = 0; j < 24; j++)
		{
			if (World->GetWorldMap()[i][j] == 1)
				str.append(1, L'■');
			else
				str.append(1, ' ');
		}
		GameEngine::GetInstance()->GetScreen()->PrintString(str, 0, i);
	}
}

void Renderer::DrawCeiling(const WorldManager* World)
{}

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
	Screen* GScreen = GameEngine::GetInstance()->GetScreen();
	double PlayerDirX = PlayerTransform->GetDirVec().DirX;
	double PlayerDirY = PlayerTransform->GetDirVec().DirY;

	double CameraDirX = PlayerTransform->GetCameraDirVec().DirX;
	double CameraDirY = PlayerTransform->GetCameraDirVec().DirY;

	float PlayerPosX = PlayerTransform->GetPos().X;
	float PlayerPosY = PlayerTransform->GetPos().Y;

	const int WIDTH = GScreen->SceneHorSize;
	const int HEIGHT = GScreen->SceneVerSize;

	for (int Y = HEIGHT / 2 + 1; Y < HEIGHT; Y++)
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
			GScreen->PrintChar(tileChar, X+1, Y+1);

			FloorX += FloorStepX;
			FloorY += FloorStepY;
		}
	}
}

void Renderer::DrawWall(const WorldManager* World)
{
	Screen* GScreen = GameEngine::GetInstance()->GetScreen();
	const int WIDTH = GScreen->SceneHorSize;
	const int HEIGHT = GScreen->SceneVerSize;

	for (int X = 0; X < WIDTH; X++)
	{
		int OutSide = 0;
		FRaycasterResult DDARes = DDA(X, ERayCastLayer::WALL, World);
		if (DDARes.bHit)
		{
			if (GetSqrDist(World->GetPlayerPos(), DDARes.MapPos) < SightRange * SightRange)
			{
				MiniMap[static_cast<int>(DDARes.MapPos.X)][static_cast<int>(DDARes.MapPos.Y)] = true;
			}
		}
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
		GScreen->Zbuffer[X] = PerpWallDist;
		//DrawWallVer((OutSide == 1) ? L'\u2588' : L'\u2593', X, DrawStart, DrawEnd, Attribute);
		DrawWallVer(WallChar, X + 1, DrawStart + 1, DrawEnd + 1, Attribute);
	}
}

void Renderer::DrawSprite(const WorldManager* World)
{
	///*
	//Sprite render 기초
	//1: While raycasting the walls, store the perpendicular distance of each vertical stripe in a 1D ZBuffer
	//2: Calculate the distance of each sprite to the player
	//3: Use this distance to sort the sprites, from furthest away to closest to the camera
	//4: Project the sprite on the camera plane (in 2D): subtract the player position from the sprite position, then multiply the result with the inverse of the 2x2 camera matrix
	//5: Calculate the size of the sprite on the screen (both in x and y direction) by using the perpendicular distance
	//6: Draw the sprites vertical stripe by vertical stripe, don't draw the vertical stripe if the distance is further away than the 1D ZBuffer of the walls of the current stripe
	//7: Draw the vertical stripe pixel by pixel, make sure there's an invisible color or all sprites would be rectangles
	//*/

	//// sortSprite Far to Close (가장 먼거부터 그려야, 가려질 수 있음, 앞에서부터 그리면 전부 다 나옴)
	//// 벡터 거리 저장
	//// 벡터 우선순위 초기화
	//FTransform* PlayerTransform = Player->GetTransform();
	//double PlayerDirX = PlayerTransform->GetDirVec().DirX;
	//double PlayerDirY = PlayerTransform->GetDirVec().DirY;

	//double CameraDirX = PlayerTransform->GetCameraDirVec().DirX;
	//double CameraDirY = PlayerTransform->GetCameraDirVec().DirY;

	//float PlayerPosX = PlayerTransform->GetPos().X;
	//float PlayerPosY = PlayerTransform->GetPos().Y;

	//const int WIDTH = GScreen.HorSize;
	//const int HEIGHT = GScreen.VerSize;

	//for (int i = 0; i < NumOfSprite; i++)
	//{
	//	SpriteOrder[i] = i;
	//	SpriteDistance[i] = ((PlayerPosX - Sprites[i].X) * (PlayerPosX - Sprites[i].X) + (PlayerPosY - Sprites[i].Y) * (PlayerPosY - Sprites[i].Y));
	//}

	//SortSprite(&SpriteOrder, &SpriteDistance, NumOfSprite);

	//for (int i = 0; i < NumOfSprite; i++)
	//{
	//	auto* CurrentSprite = &Sprites[SpriteOrder[i]];

	//	// 카메라에서 스프라이트 까지의 상대적인 위치
	//	double SpriteX = CurrentSprite->X - PlayerPosX;
	//	double SpriteY = CurrentSprite->Y - PlayerPosY;

	//	// 카메라 행렬(카메라 좌표계)
	//	// 카메라의 정면 (스크린 안쪽)은 플레이어의 방향벡터
	//	// 카메라의 오른쪽 (스크린의 오른쪽)은 카메라 평면 벡터
	//	// 카메라 좌표계 기준 정면으로 1 이동한건 월드 입장에서는 1 * 플레이어 방향벡터 만큼 이동한 것
	//	// 카메라 좌표계 기준 우측으로 1 이동한건 월드 입장에서는 1 * 카메라 평면의 방향벡터 만큼 이동한 것

	//	// 카메라 기준으로 Cx, Cy 이동 했으면 최종 월드좌표 Wx,Wy 는 다음과 같다.
	//	//[Wx]   [PlaneX  DirX]   [Cx]	Wx = PlaneX * Cx + DirX * Cy
	//	//[  ] = [            ] x [  ]
	//	//[Wy]   [PlaneY  DirY]   [Cy]	Wx = PlaneY * Cx + DirY * Cy
	//	// 
	//	// 여기서 Wx를 계산하는데, DirX * Cy는 왜 들어가는지 고민했는데, 이건 카메라 자체가 회전된 상태일 때 유효하다
	//	// 카메라가 회전하지 않았다면 초기 방향에따라서 DirX혹은 DirY가 0이다. (방향 벡터가 (1,0) (-1,0) 이런식으로 나오니까)
	//	// 그래서 (1,0)인 경우에 Wx = PlaneX * Cx + 0 * Cy가 되어서 Wx = PlaneX 가 된다. 하지만 방향 벡터가 회전했다면 
	//	// 예를 들어 (1,1)이라고 하면 카메라 평면은 (1,-1)이된다. 여기서 카메라의 좌표가 (0,0) 에서 (1,1)이 되면
	//	// 카메라 기준으로는 오른쪽으로 한칸 안쪽으로 한칸 이동한다. 이걸 월드 입장에서 보면 (1,-1) 방향으로 이동 후 (1,1) 방향으로 이동한게 된다.
	//	// 맨 처음에는 PlaneX * Cx 만큼 이동하고 1,1 방향으로 이동할 때, DirX * Cy 만큼 이동한다
	//	//          y  ▲
	//	//             |  ↘(Plane 방향)    ↗ (Dir 방향) 
	//	//	           |    ↘            ↗
	//	//             |      ↘        ↗
	//	//             |        ↘    ↗
	//	//             |          ↘↗
	//	//             -------------------▶ x
	//	//  방금 말한 상황이 이 그림인데 Wx는 Plane의 x방향으로 Cx만큼 이동한 것 +  Dir의 x방향으로 Cy 이동한 것 
	//	//  Wy는 Plane의 y방향으로 Cx만큼 이동, 으로 이동한 것 + dir의 y Cy만큼 이동방향으로 이동
	//	//  상대위치에 카메라의 역행렬을 곱해서 카메라 좌표의 X,Y를 구할 수 있다.(Y는 깊이, 화면 안쪽으로 들어가는)
	//	//transform sprite with the inverse camera matrix
	//	// [ planeX   dirX ] -1                                       [ dirY      -dirX ]
	//	// [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
	//	// [ planeY   dirY ]                                          [ -planeY  planeX ]

	//	double invDet = 1.0 / (CameraDirX * PlayerDirY - PlayerDirX * CameraDirY);

	//	// 카메라 중심에서 좌우로 얼마나 떨어져 있는가
	//	double transformX = invDet * (PlayerDirY * SpriteX - PlayerDirX * SpriteY); // 화면의 좌우 (양수면 오른쪽)
	//	// 카메라 중심에서 앞뒤로 얼마나 떨어져 있는가.
	//	double transformY = invDet * (-CameraDirY * SpriteX + CameraDirX * SpriteY); // 화면 안쪽으로 얼마나 들어가 있는지

	//	// ?????
	//	// 원근투영
	//	// 어떤 물체가 왼쪽으로 2미터 떨어진 위치에 있다.
	//	// 화면에 가까울때는 화면을 많이 돌려야하고, 크게 보인다.
	//	// 화면에서 멀때는 화면의 중앙에 가까운곳에 작게 보인다.
	//	// transformX / transformY 이게 화면에서 멀수록 가운데로오게 해준다.
	//	// 이 값은 -1 < X < 1인데 화면에는 음수 좌표계가 없으니까 +1
	//	// 0 < x < 2 범위의 X를 0~130까지의 정수로 변환
	//	// 스프라이트가 찍힐 X 좌표
	//	int SpriteScrrenX = int((GScreen.HorSize / 2) * (1 + transformX / transformY));

	//	//--------------------------
	//	//Scailing					|
	//	//--------------------------
	//	// rkfh로 몇배 줄일건지
	//	const float uDiv = 1 / (float)4;
	//	// 세로로 몇배 줄일건지
	//	const float vDiv = 1 / (float)2;
	//	// 위로 몇칸 갈건지 
	//	// 이 값들은 스프라이트마다 가지고 있으면 좋을 듯 함;
	//	int vMoveScrren = int(-CurrentSprite->VMove / transformY);


	//	// 스프라이트의 높이
	//	// 어안 렌즈 방지를 위해 실제 거리 말고 transformY 사용
	//	// 스프라이트의 높이가 화면에 들어가 있을수록 작아짐( 플레이어로 부터 멀리 있을수록 작아짐)
	//	int SpriteHeight = (int)(abs(int(GScreen.VerSize / transformY)) / vDiv);
	//	//세로 비율 조정

	//	int DrawStartY = -SpriteHeight / 2 + GScreen.VerSize / 2 + vMoveScrren;
	//	if (DrawStartY < 0) DrawStartY = 0;
	//	int DrawEndY = SpriteHeight / 2 + GScreen.VerSize / 2 + vMoveScrren;
	//	if (DrawEndY >= GScreen.VerSize) DrawEndY = GScreen.VerSize - 1;

	//	// 스프라이트의 너비
	//	int SpriteWidth = (int)(abs(int(GScreen.VerSize / transformY)) / uDiv);

	//	int DrawStartX = -SpriteWidth / 2 + SpriteScrrenX;
	//	if (DrawStartX < 0) DrawStartX = 0;
	//	int DrawEndX = SpriteWidth / 2 + SpriteScrrenX;
	//	if (DrawEndX >= GScreen.HorSize) DrawEndX = GScreen.HorSize;

	//	for (int Stripe = DrawStartX; Stripe < DrawEndX; Stripe++)
	//	{
	//		for (int j = DrawStartY; j < DrawEndY; j++)
	//		{
	//			// 현재 픽셀이 텍스쳐의 가로에서 몇번째인지 확인
	//			// (현재 위치- 시작 위치) * 텍스쳐 크기 / 전체 너비
	//			int texX = int(256 * (Stripe - (-SpriteWidth / 2 + SpriteScrrenX)) * SpriteTextureTest_RowSize / SpriteWidth) / 256;
	//			//int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * texWidth / spriteWidth) / 256;

	//			// 경계 안으로 들어 오도록
	//			if (texX < 0) texX = 0;
	//			if (texX >= SpriteTextureTest_RowSize) texX = SpriteTextureTest_RowSize - 1;

	//			// 1. transformY이 0이하면 화면의 뒤쪽
	//			// 2. i가 화면에 있는지
	//			// 3. 벽보다 가까이 있는지
	//			if (transformY > 0 && Stripe >= 0 && Stripe < GScreen.HorSize && transformY < GScreen.Zbuffer[Stripe])
	//			{
	//				//256 and 128 factors to avoid floats 실수를 피하기 위해서 이걸 곱했다는데 잘 몰루
	//				int d = (j - vMoveScrren) * 256 - GScreen.VerSize * 128 + SpriteHeight * 128;
	//				int texY = ((d * SpriteTextureTest_RowSize) / SpriteHeight) / 256;

	//				// 경계 안으로 들어 오도록
	//				if (texY < 0) texY = 0;
	//				if (texY >= SpriteTextureTest_RowSize) texY = SpriteTextureTest_RowSize - 1;

	//				//TODO 고치기
	//				wchar_t SpriteChar = CurrentSprite->SpriteTexture[0][texY][texX];


	//				//GScreen.PrintChar(SpriteChar, Stripe, j);

	//				// 공백 처리 (텍스처 배열에서 ' ' 즉, 빈 공간은 투명화 처리하여 그리지 않음)
	//				if (SpriteChar != L' ')
	//				{
	//					// GScreen의 i(가로), j(세로) 좌표에 글자(spriteChar)를 그리는 함수를 호출하세요.
	//					// 예시: GScreen.Buffer[j][i] = spriteChar;
	//					GScreen.PrintChar(SpriteChar, Stripe, j);
	//				}
	//			}
	//		}
	//	}
	//}
}

void Renderer::DrawObject(const WorldManager* World)
{}

void Renderer::Draw3DGrid(const WorldManager* World)
{
	//DrawCeiling();
	DrawFloor(World);
	DrawWall(World);
}

void Renderer::DrawPlayer(const WorldManager* World)
{
	//GScreen.PrintString(L"※", (int)Player.X, (int)Player.Y);
}

void Renderer::DrawEnemy(const WorldManager* World)
{
	// sortSprite Far to Close (가장 먼거부터 그려야, 가려질 수 있음, 앞에서부터 그리면 전부 다 나옴)
// 벡터 거리 저장
// 벡터 우선순위 초기화
	FTransform* PlayerTransform = World->GetPlayer()->GetTransform();
	Screen* GScreen = GameEngine::GetInstance()->GetScreen();

	double PlayerDirX = PlayerTransform->GetDirVec().DirX;
	double PlayerDirY = PlayerTransform->GetDirVec().DirY;

	double CameraDirX = PlayerTransform->GetCameraDirVec().DirX;
	double CameraDirY = PlayerTransform->GetCameraDirVec().DirY;

	float PlayerPosX = PlayerTransform->GetPos().X;
	float PlayerPosY = PlayerTransform->GetPos().Y;

	const int WIDTH = GScreen->SceneHorSize;
	const int HEIGHT = GScreen->SceneVerSize;
	const vector<FEnemy*>* EnemyVec = World->GetEnemyVec();

	vector<int>		EnemyOrder(EnemyVec->size());
	vector<double>	EnemyDistance(EnemyVec->size());

	for (int i = 0; i < EnemyVec->size(); i++)
	{
		EnemyOrder[i] = i;
		EnemyDistance[i] = GetSqrDist(PlayerTransform->GetPos(), (*EnemyVec)[i]->GetTransform()->GetPos());
	}

	SortSprite(&EnemyOrder, &EnemyDistance, static_cast<int>(EnemyVec->size()));

	for (int i = 0; i < EnemyVec->size(); i++)
	{
		FEnemy* CurrentEnemy = (*EnemyVec)[EnemyOrder[i]];
		FSprite* CurrentSprite = CurrentEnemy->GetSprite();
		FTransform* CurrentTransform = CurrentEnemy->GetTransform();
		int CreatureState = static_cast<int>(CurrentEnemy->GetState());

		// 카메라에서 적 까지의 상대적인 위치
		double SpriteX = CurrentTransform->Pos.X - PlayerPosX;
		double SpriteY = CurrentTransform->Pos.Y - PlayerPosY;

		double invDet = 1.0 / (CameraDirX * PlayerDirY - PlayerDirX * CameraDirY);

		// 카메라 중심에서 좌우로 얼마나 떨어져 있는가
		double transformX = invDet * (PlayerDirY * SpriteX - PlayerDirX * SpriteY); // 화면의 좌우 (양수면 오른쪽)
		// 카메라 중심에서 앞뒤로 얼마나 떨어져 있는가.
		double transformY = invDet * (-CameraDirY * SpriteX + CameraDirX * SpriteY); // 화면 안쪽으로 얼마나 들어가 있는지

		// ?????
		// 원근투영
		// 어떤 물체가 왼쪽으로 2미터 떨어진 위치에 있다.
		// 화면에 가까울때는 화면을 많이 돌려야하고, 크게 보인다.
		// 화면에서 멀때는 화면의 중앙에 가까운곳에 작게 보인다.
		// transformX / transformY 이게 화면에서 멀수록 가운데로오게 해준다.
		// 이 값은 -1 < X < 1인데 화면에는 음수 좌표계가 없으니까 +1
		// 0 < x < 2 범위의 X를 0~SceneHorSize까지의 정수로 변환
		// 스프라이트가 찍힐 X 좌표
		int SpriteScrrenX = int((GScreen->SceneHorSize / 2) * (1 + transformX / transformY));

		//--------------------------
		//Scailing					|
		//--------------------------
		
		//TODO FSprite의 값으로 고치기
		// 가로 몇배 줄일건지
		const float uDiv = 1;
		// 세로로 몇배 줄일건지
		const float vDiv = 2;
		// 위로 몇칸 갈건지 
		// 이 값들은 스프라이트마다 가지고 있으면 좋을 듯 함;
		//int vMoveScrren = int(-CurrentSprite->VMove / transformY);
		int vMoveScrren = 0;


		// 스프라이트의 높이
		// 어안 렌즈 방지를 위해 실제 거리 말고 transformY 사용
		// 스프라이트의 높이가 화면에 들어가 있을수록 작아짐( 플레이어로 부터 멀리 있을수록 작아짐)
		int SpriteHeight = (int)(abs(int(GScreen->SceneVerSize / transformY)) / vDiv);
		//세로 비율 조정

		int DrawStartY = -SpriteHeight / 2 + GScreen->SceneVerSize / 2 + vMoveScrren;
		if (DrawStartY < 0) DrawStartY = 0;
		int DrawEndY = SpriteHeight / 2 + GScreen->SceneVerSize / 2 + vMoveScrren;
		if (DrawEndY >= GScreen->SceneVerSize) DrawEndY = GScreen->SceneVerSize - 1;

		// 스프라이트의 너비
		int SpriteWidth = (int)(abs(int(GScreen->SceneVerSize / transformY)) / uDiv);

		int DrawStartX = -SpriteWidth / 2 + SpriteScrrenX;
		if (DrawStartX < 0) DrawStartX = 0;
		int DrawEndX = SpriteWidth / 2 + SpriteScrrenX;
		if (DrawEndX >= GScreen->SceneHorSize) DrawEndX = GScreen->SceneHorSize;

		for (int Stripe = DrawStartX; Stripe < DrawEndX; Stripe++)
		{
			for (int j = DrawStartY; j < DrawEndY; j++)
			{
				// 현재 픽셀이 텍스쳐의 가로에서 몇번째인지 확인
				// (현재 위치- 시작 위치) * 텍스쳐 크기 / 전체 너비
				// 256 안곱하니까 스프라이트 원래 모양이 안나옴
				int texX = int(256 * (Stripe - (-SpriteWidth / 2 + SpriteScrrenX)) * (CurrentSprite->Height) / SpriteWidth) / 256;
				//int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * texWidth / spriteWidth) / 256;

				// 경계 안으로 들어 오도록
				if (texX < 0) texX = 0;
				if (texX >= (CurrentSprite->Height)) texX = (CurrentSprite->Height) - 1;

				// 1. transformY이 0이하면 화면의 뒤쪽
				// 2. i가 화면에 있는지
				// 3. 벽보다 가까이 있는지
				if (transformY > 0 && Stripe >= 0 && Stripe < GScreen->SceneHorSize && transformY <= GScreen->Zbuffer[Stripe])
				{
					//256 and 128 factors to avoid floats 실수를 피하기 위해서 이걸 곱했다는데 잘 몰루
					int d = (j - vMoveScrren) * 256 - GScreen->SceneVerSize * 128 + SpriteHeight * 128;
					int texY = ((d * (CurrentSprite->Width)) / SpriteHeight) / 256;

					// 경계 안으로 들어 오도록
					if (texY < 0) texY = 0;
					if (texY >= (CurrentSprite->Width)) texY = (CurrentSprite->Width) - 1;

					//TODO 고치기(SpriteIndex)
					ECreatureSpriteIndex SpriteIndex = CurrentEnemy->GetSpriteIndex();
					switch (SpriteIndex)
					{
						
						case ECreatureSpriteIndex::OnAttacked:
							SpriteIndex = ECreatureSpriteIndex::Idle;
							break;
						case ECreatureSpriteIndex::AttackStart:
						case ECreatureSpriteIndex::AttackEnd:
						case ECreatureSpriteIndex::Dead:
							break;
						default:
							SpriteIndex = ECreatureSpriteIndex::Idle;
							break;
					}

					wchar_t SpriteChar = CurrentSprite->SpriteTexture[static_cast<int>(SpriteIndex)][texY][texX];

					if (CurrentEnemy->GetbBlink())
					{
						// 피격시 깜빡임
						double TotalTime = World->GetAmountTime();
						if (static_cast<int>(TotalTime * AnimBlinkSpeed) % BlinkFrequency == 0)
						{
							SpriteChar = L' ';
						}
					}
					// 스프라이트를 출력하는 적들은 시야각안에 있음
					// TODO 그냥 벡터 계산해서 하는거랑 이거랑 비교해보기
					CurrentEnemy->SetIsInSight(true);


					//GScreen.PrintChar(SpriteChar, Stripe, j);

					// 공백 처리 (텍스처 배열에서 ' ' 즉, 빈 공간은 투명화 처리하여 그리지 않음)
					if (SpriteChar != L' ')
					{
						// GScreen의 i(가로), j(세로) 좌표에 글자(spriteChar)를 그리는 함수를 호출하세요.
						// 예시: GScreen.Buffer[j][i] = spriteChar;
						int Attribute = CurrentSprite->BaseAttribute;
						if (SpriteChar != L'▒')
							Attribute = SCREEN_TEXT_COLOR_WHITE;

						GScreen->PrintChar(SpriteChar, Stripe+1, j+1, Attribute);
					}
				}
			}
		}
	}
}

void Renderer::DrawInfo(const WorldManager* World)
{

	DrawGameStatus(World);
	DrawPlayerStatus(World);
}

void Renderer::DrawGameStatus(const WorldManager* World)
{
	Screen* GScreen = GameEngine::GetInstance()->GetScreen();
	wstringstream Wss;
	// 소수점 아래 자리 고정
	Wss.fixed;
	Wss.precision(2);
	int LeftTopX = 0;
	int LeftTopY = 0;
	int SceneRightBottomX = GScreen->SceneHorSize + 2;
	int SceneRightBottomY = World->mapHeight + 2;

	int FPS = static_cast<int>(1 / GameEngine::GetInstance()->GetDeltaTime());
	wstring Wstr = to_wstring(FPS);
	FTransform* PlayerTransform = World->GetPlayer()->GetTransform();
	FPlayer* Player = World->GetPlayer();

	Wss << std::setfill(L'0') << fixed << setprecision(2) << std::setw(2)
		<< L"FPS : " << Wstr;
	GameEngine::GetInstance()->GetScreen()->PrintString(Wss.str(), SceneRightBottomX, SceneRightBottomY + 3);

	Wss.clear();
	Wss.str(L"");
	Wss << std::setfill(L'0') << fixed << setprecision(2) << std::setw(2)
		<< L"Pos (" << std::setw(2) << PlayerTransform->GetPos().X << L", " << std::setw(2) << PlayerTransform->GetPos().Y << L")";
	GameEngine::GetInstance()->GetScreen()->PrintString(Wss.str(), SceneRightBottomX, SceneRightBottomY + 4);


/*	FTransform* ETransForm = (*World->GetEnemyVec())[0]->GetTransform();
	Wss.clear();
	Wss.str(L"");
	Wss << std::setfill(L'0') << fixed << setprecision(2) << std::setw(2)
		<< L"EPos (" << std::setw(2) << ETransForm->GetPos().X << L", " << std::setw(2) << ETransForm->GetPos().Y << L")";
	GameEngine::GetInstance()->GetScreen()->PrintString(Wss.str(), SceneRightBottomX, SceneRightBottomY + 5)*/;


	Wss.clear();
	Wss.str(L"");
	Wss << L"Theta : " << World->GetPlayer()->GetTheta() << L"°";;

	GameEngine::GetInstance()->GetScreen()->PrintString(Wss.str(), SceneRightBottomX, SceneRightBottomY + 6);

	Wss.clear();
	Wss.str(L"");
	Wss << std::setfill(L'0') << fixed << setprecision(2) << std::setw(2)
		<< L"Score : " << Player->GetScore();

	GameEngine::GetInstance()->GetScreen()->PrintString(Wss.str(), SceneRightBottomX, SceneRightBottomY + 7);

	Wss.clear();
	Wss.str(L"");
	Wss << "Time : "
		<< World->GetAmountTime();

	GameEngine::GetInstance()->GetScreen()->PrintString(Wss.str(), SceneRightBottomX, SceneRightBottomY + 1);
}

void Renderer::DrawPlayerStatus(const WorldManager* World)
{

	Screen* GScreen = GameEngine::GetInstance()->GetScreen();
	int LeftTopX = 2;
	int LeftTopY = GScreen->SceneVerSize + 2;
	int VerIndex = (GScreen->TotalHorSize - 1) / 3;
	int RightBottomX = LeftTopX + VerIndex - 2;
	int RightBottomY = GScreen->TotalVerSize - 1;
	wstringstream Wss;
	FPlayer* Player = World->GetPlayer();

	Wss.clear();
	Wss.str(L"");
	Wss << std::setfill(L'0') << fixed << setprecision(2) << std::setw(2)
		<< L"Hp   : ";
	
	int Cnt = Player->GetHp() / 5;

	for (int i = 0; i < Cnt; i++)
		Wss << L"■";

	GameEngine::GetInstance()->GetScreen()->PrintString(Wss.str(), LeftTopX, (LeftTopY + RightBottomY)/2);

	Wss.clear();
	Wss.str(L"");
	Wss << std::setfill(L'0') << fixed << setprecision(2) << std::setw(2)
		<< L"Bullet: " << Player->GetBullet() << L" / " << Player->GetMaxBullet();

	GameEngine::GetInstance()->GetScreen()->PrintString(Wss.str(), LeftTopX + 2 * VerIndex, (LeftTopY + RightBottomY) / 2);
}

void Renderer::DrawPlayerHud(const WorldManager* World)
{

	// 가로 몇배 늘릴건지
	const float uMul = 2;
	// 세로로 몇배 늘릴건지
	const float vMul = 1;

	FSprite* WeponHudSprite = World->GetPlayer()->GetSprite();
	Screen* GScreen = GameEngine::GetInstance()->GetScreen();

	int SpriteHeight = WeponHudSprite->Height;
	int SpriteWidth = WeponHudSprite->Width;
	ECreatureState PlayerState = World->GetPlayer()->GetState();

	int DrawStartY = GScreen->SceneVerSize - SpriteHeight;
	if (DrawStartY < 0) DrawStartY = 0;
	int DrawEndY = GScreen->SceneVerSize;

	int DrawStartX = GScreen->SceneHorSize / 2 - SpriteWidth / 2;
	if (DrawStartX < 0) DrawStartX = 0;
	int DrawEndX = GScreen->SceneHorSize / 2 + SpriteWidth / 2;
	if (DrawEndX >= GScreen->SceneHorSize) DrawEndX = GScreen->SceneHorSize;

	for (int Stripe = DrawStartX; Stripe < DrawEndX; Stripe++)
	{
		for (int j = DrawStartY; j < DrawEndY; j++)
		{
			// 현재 픽셀이 텍스쳐의 가로에서 몇번째인지 확인
			// (현재 위치- 시작 위치) * 텍스쳐 크기 / 전체 너비
			int texX = (Stripe + SpriteWidth / 2 - GScreen->SceneHorSize / 2);

			// 경계 안으로 들어 오도록
			if (texX < 0) texX = 0;
			if (texX >= SpriteWidth) texX = SpriteWidth;

			// 2. i가 화면에 있는지
			if (Stripe >= 0 && Stripe < GScreen->SceneHorSize)
			{
				int d = (j + (SpriteHeight - GScreen->SceneVerSize));
				int texY = d;

				// 경계 안으로 들어 오도록
				if (texY < 0) texY = 0;
				if (texY >= SpriteHeight) texY = SpriteHeight - 1;

				int HudStateIndex = static_cast<int>(PlayerState);
				if (PlayerState != ECreatureState::Attack)
					HudStateIndex = static_cast<int>(ECreatureState::Idle);

				wchar_t SpriteChar = WeponHudSprite->SpriteTexture[HudStateIndex][texY][texX];

				//TODO 스케일 대응하기
				/*GScreen.PrintChar(SpriteChar, Stripe, j);*/

				// 공백 처리 (텍스처 배열에서 ' ' 즉, 빈 공간은 투명화 처리하여 그리지 않음)
				if (SpriteChar != L' ')
				{
					// GScreen의 i(가로), j(세로) 좌표에 글자(spriteChar)를 그리는 함수를 호출하세요.
					// 예시: GScreen.Buffer[j][i] = spriteChar;
					GScreen->PrintChar(SpriteChar, Stripe+1, j+1);
				}
			}
		}
	}
}

void Renderer::SortSprite(vector<int>* OrderVec, vector<double>* DistVec, int Amount)
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

void Renderer::DrawWallVer(wchar_t Wchar, int X, int DrawStart, int DrawEnd, const int Attribute)
{
	int Length = DrawEnd - DrawStart;
	GameEngine::GetInstance()->GetScreen()->PrintVer(Wchar, X, DrawStart, Length, Attribute);
}

void Renderer::DrawPauseMenu(const WorldManager* World)
{
	Screen* GScreen = GameEngine::GetInstance()->GetScreen();
	int DrawStartX = GScreen->TotalHorSize / 5;
	int DrawEndX = DrawStartX * 4;
	int DrawStartY = GScreen->TotalVerSize /5;
	int DrawEndY = DrawStartY * 4;

	int ResumeY = GScreen->TotalVerSize /2;
	int ResumeX = GScreen->TotalHorSize /2;
	int ExitY = ResumeY + 2;
	int ExitX = ResumeX;

	const int ArrowSpace = 5;

	// Draw OutLine
	for (int i = DrawStartY; i <= DrawEndY; i++)
	{
		if (i == DrawStartY)
		{
			GScreen->PrintChar(L'┏', DrawStartX, i);
			for (int j = DrawStartX + 1; j <= DrawEndX - 1; j++)
				GScreen->PrintChar(L'━', j, i);
			GScreen->PrintChar(L'┓', DrawEndX, i);
		}
		else if (i == DrawEndY)
		{
			GScreen->PrintChar(L'┗', DrawStartX, i);
			for (int j = DrawStartX + 1; j <= DrawEndX - 1; j++)
				GScreen->PrintChar(L'━', j, i);
			GScreen->PrintChar(L'┛', DrawEndX, i);
		}
		else if (i == ResumeY)
		{
			GScreen->PrintChar(L'┃', DrawStartX, i);
			GScreen->PrintString(L"Resume", ResumeX, ResumeY);
			GScreen->PrintChar(L'┃', DrawEndX, i);
		}
		else if (i == ExitY)
		{
			GScreen->PrintChar(L'┃', DrawStartX, i);
			GScreen->PrintString(L"Exit", ExitX, ExitY);
			GScreen->PrintChar(L'┃', DrawEndX, i);
		}
		else
		{
			GScreen->PrintChar(L'┃', DrawStartX, i);
			GScreen->PrintChar(L'┃', DrawEndX, i);
		}
	}

	// Draw Select Arrow
	switch (World->GetPauseIndex())
	{
		case EPauseMenu::EPauseMenuLen:
		case EPauseMenu::None:
			break;
		case EPauseMenu::Resume:
			GScreen->PrintChar(L'▶', ResumeX - ArrowSpace, ResumeY);
			break;
		case EPauseMenu::Exit:
			GScreen->PrintChar(L'▶', ExitX - ArrowSpace, ExitY);
			break;
	}
}

void Renderer::DrawMainMenu(const WorldManager* World)
{
	Screen* GScreen = GameEngine::GetInstance()->GetScreen();
	int DrawStartX = GScreen->TotalHorSize / 5;
	int DrawEndX = DrawStartX * 4;
	int DrawStartY = GScreen->TotalVerSize / 5;
	int DrawEndY = DrawStartY * 4;

	int ResumeY = GScreen->TotalVerSize / 2;
	int ResumeX = GScreen->TotalHorSize / 2;
	int ExitY = ResumeY + 2;
	int ExitX = ResumeX;


	// 0 30 60 90 120 150
	const int ArrowSpace = 5;

	// Draw OutLine
	for (int i = DrawStartY; i <= DrawEndY; i++)
	{
		if (i == DrawStartY)
		{
			GScreen->PrintChar(L'┏', DrawStartX, i);
			for (int j = DrawStartX + 1; j <= DrawEndX - 1; j++)
				GScreen->PrintChar(L'━', j, i);
			GScreen->PrintChar(L'┓', DrawEndX, i);
		}
		else if (i == DrawEndY)
		{
			GScreen->PrintChar(L'┗', DrawStartX, i);
			for (int j = DrawStartX + 1; j <= DrawEndX - 1; j++)
				GScreen->PrintChar(L'━', j, i);
			GScreen->PrintChar(L'┛', DrawEndX, i);
		}
		else if (i == ResumeY)
		{
			GScreen->PrintChar(L'┃', DrawStartX, i);
			GScreen->PrintString(L"StartGame", ResumeX, ResumeY);
			GScreen->PrintChar(L'┃', DrawEndX, i);
		}
		else if (i == ExitY)
		{
			GScreen->PrintChar(L'┃', DrawStartX, i);
			GScreen->PrintString(L"ExitGame", ExitX, ExitY);
			GScreen->PrintChar(L'┃', DrawEndX, i);
		}
		else
		{
			GScreen->PrintChar(L'┃', DrawStartX, i);
			GScreen->PrintChar(L'┃', DrawEndX, i);
		}
	}

	// Draw Select Arrow
	switch (World->GetMainIndex())
	{
		case EMainMenu::EMainMenuLen:
		case EMainMenu::None:
			break;
		case EMainMenu::StartGame:
			GScreen->PrintChar(L'▶', ResumeX - ArrowSpace, ResumeY);
			break;
		case EMainMenu::ExitGame:
			GScreen->PrintChar(L'▶', ExitX - ArrowSpace, ExitY);
			break;
	}
}

void Renderer::DrawBorder(const WorldManager* World)
{
	Screen* GScreen = GameEngine::GetInstance()->GetScreen();
	int LeftTopX = 0;
	int LeftTopY = 0;
	int SceneRightBottomX = GScreen->SceneHorSize + 1;
	int SceneRightBottomY = GScreen->SceneVerSize + 1;

	int TotalRightBottomX = GScreen->TotalHorSize;
	int TotalRightBottomY = GScreen->TotalVerSize;

	// Scene Border
	GScreen->PrintHor(L'━', LeftTopX, LeftTopY, TotalRightBottomX - LeftTopX, SCREEN_TEXT_COLOR_WHITE);
	GScreen->PrintVer(L'┃', SceneRightBottomX, LeftTopY, SceneRightBottomY - LeftTopY, SCREEN_TEXT_COLOR_WHITE);
	GScreen->PrintHor(L'━', LeftTopX, SceneRightBottomY, SceneRightBottomX - LeftTopX, SCREEN_TEXT_COLOR_WHITE);
	GScreen->PrintVer(L'┃', LeftTopX, LeftTopY, SceneRightBottomY - LeftTopY, SCREEN_TEXT_COLOR_WHITE);
	GScreen->PrintVer(L'┃', TotalRightBottomX - 1, LeftTopY, TotalRightBottomY - LeftTopY, SCREEN_TEXT_COLOR_WHITE);

	GScreen->PrintChar(L'┏', LeftTopX, LeftTopY);
	GScreen->PrintChar(L'┳', SceneRightBottomX, LeftTopY);
	GScreen->PrintChar(L'┠', LeftTopX, SceneRightBottomY);
	GScreen->PrintChar(L'┻', SceneRightBottomX, SceneRightBottomY);

	GScreen->PrintChar(L'┓', TotalRightBottomX - 1, LeftTopY);

	// PlayerStatusBorder
	GScreen->PrintHor(L'━', LeftTopX, TotalRightBottomY - 1, TotalRightBottomX - LeftTopX + 1, SCREEN_TEXT_COLOR_WHITE);
	GScreen->PrintHor(L'━', SceneRightBottomX + 1, SceneRightBottomY, TotalRightBottomX - SceneRightBottomX, SCREEN_TEXT_COLOR_WHITE);
	
	int VerticalIndexX = (TotalRightBottomX - 1) / 3;
	GScreen->PrintVer(L'┃', VerticalIndexX * 0, SceneRightBottomY + 1, TotalRightBottomY - SceneRightBottomY, SCREEN_TEXT_COLOR_WHITE);

	GScreen->PrintChar(L'┳', VerticalIndexX * 1, SceneRightBottomY);
	GScreen->PrintVer(L'┃', VerticalIndexX * 1, SceneRightBottomY + 1, TotalRightBottomY - SceneRightBottomY, SCREEN_TEXT_COLOR_WHITE);
	GScreen->PrintChar(L'┻', VerticalIndexX * 1, TotalRightBottomY-1);

	GScreen->PrintChar(L'┳', VerticalIndexX * 2, SceneRightBottomY);
	GScreen->PrintVer(L'┃', VerticalIndexX * 2, SceneRightBottomY + 1, TotalRightBottomY - SceneRightBottomY, SCREEN_TEXT_COLOR_WHITE);
	GScreen->PrintChar(L'┻', VerticalIndexX * 2, TotalRightBottomY-1);
	
	GScreen->PrintVer(L'┃', TotalRightBottomX - 1, SceneRightBottomY + 1, TotalRightBottomY - SceneRightBottomY, SCREEN_TEXT_COLOR_WHITE);

	GScreen->PrintChar(L'┗', LeftTopX, TotalRightBottomY-1);
	GScreen->PrintChar(L'┛', TotalRightBottomX-1, TotalRightBottomY -1);
	GScreen->PrintChar(L'┫', TotalRightBottomX-1, SceneRightBottomY);


	


}

void Renderer::DrawMiniMap(const WorldManager* World)
{
	Screen* GScreen = GameEngine::GetInstance()->GetScreen();
	FPos PlayerPos = World->GetPlayerPos();
	int Theta = static_cast<int>(World->GetPlayer()->GetTheta()) % 360;
	Theta = (Theta + 360) % 360;
	int MiniMapLeftTopX = GScreen->SceneHorSize + 2;
	int MiniMapLeftTopY = 0;
	int pivotX = 0;
	int pivotY = 1;


	
	for (int i = 0; i < World->mapWidth; i++)
	{
		for (int j = 0; j < World->mapHeight; j++)
		{
			wchar_t SpriteChar = L' ';
			if (MiniMap[i][j] == true)
				SpriteChar = L'█';

			if (i == 0 || j == 0)
				SpriteChar = L'█';

			if (i == World->mapWidth -1 || j == World->mapHeight-1 )
				SpriteChar = L'█';

			///GScreen->PrintChar(SpriteChar, j + MiniMapLeftTopX, i);
			GScreen->PrintChar(SpriteChar, i + MiniMapLeftTopX + pivotX, j + pivotY);
		}
	}
	wchar_t SpriteChar = L'⮝';

	if (0 <= Theta && Theta < 45)
	{
		SpriteChar = L'⮝';
	}
	else if (45 <= Theta && Theta < 135)
	{
		SpriteChar = L'⮞';
	}
	else if (135 <= Theta && Theta < 225)
	{
		SpriteChar = L'⮟';
	}
	else if (225 <= Theta && Theta <= 315)
	{
		SpriteChar = L'⮜';
	}

	GScreen->PrintChar(SpriteChar, static_cast<int>(PlayerPos.X) + MiniMapLeftTopX + pivotX, 
		static_cast<int>(PlayerPos.Y) + pivotY);
}

void Renderer::DrawOnAtackEffect(const WorldManager* World)
{
	FPlayer* Player = World->GetPlayer();
	Screen* GScreen = GameEngine::GetInstance()->GetScreen();
	if (Player->GetbBlink())
	{
		for (int i = 0; i <= GScreen->SceneHorSize; i++)
		{
			for (int j = 0; j <= GScreen->SceneVerSize; j++)
			{
				// 픽셀을 띄엄띄엄 칠해서 반투명처럼... (픽셀이 부족해서 실패)
				if ((j % 2 == 0 && i % 2 != 0) || (j % 2 != 0 && i % 2 == 0))
					GScreen->PrintChar(L'░', i, j, BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

				// 그냥 화면 덮어버리기
				//GScreen->PrintChar(L'░', i, j, BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
		}
	}

}

void Renderer::DrawDeathMenu(const WorldManager* World)
{
	Screen* GScreen = GameEngine::GetInstance()->GetScreen();
	int MinX = GScreen->TotalHorSize / 5;
	int MaxX = MinX * 4;
	int MinY = GScreen->TotalVerSize / 5;
	int MaxY = MinY * 4;

	int Msg1X = GScreen->TotalHorSize / 4;
	int Msg1Y = GScreen->TotalVerSize / 2;
	int Msg2X = Msg1X;
	int Msg2Y = (GScreen->TotalVerSize + Msg1Y) / 2;

	const int ArrowSpace = 5;

	// Draw OutLine
	for (int i = MinY; i <= MaxY; i++)
	{
		GScreen->PrintHor(L' ', MinX + 1, i, MaxX - MinX, 7);
		if (i == MinY)
		{
			GScreen->PrintChar(L'┏', MinX, i);
			for (int j = MinX + 1; j <= MaxX - 1; j++)
				GScreen->PrintChar(L'━', j, i);
			GScreen->PrintChar(L'┓', MaxX, i);
		}
		else if (i == MaxY)
		{
			GScreen->PrintChar(L'┗', MinX, i);
			for (int j = MinX + 1; j <= MaxX - 1; j++)
				GScreen->PrintChar(L'━', j, i);
			GScreen->PrintChar(L'┛', MaxX, i);
		}
		else if (i == Msg1Y)
		{
			GScreen->PrintChar(L'┃', MinX, i);
			
			GScreen->PrintString(L"YouDied", Msg1X, Msg1Y);
			
			GScreen->PrintChar(L'┃', MaxX, i);
		}
		else if (i == Msg2Y)
		{
			GScreen->PrintChar(L'┃', MinX, i);
			GScreen->PrintString(L"Press SpaceBar To Main", Msg2X, Msg2Y);
			GScreen->PrintChar(L'┃', MaxX, i);
		}
		else
		{
			GScreen->PrintChar(L'┃', MinX, i);
			GScreen->PrintChar(L'┃', MaxX, i);
		}
	}
}

void Renderer::DrawClearMenu(const WorldManager* world)
{
	Screen* GScreen = GameEngine::GetInstance()->GetScreen();
	int MinX = GScreen->TotalHorSize / 5;
	int MaxX = MinX * 4;
	int MinY = GScreen->TotalVerSize / 5;
	int MaxY = MinY * 4;

	int Msg1X = GScreen->TotalHorSize / 4;
	int Msg1Y = MinY + 1;
	int Msg2X = Msg1X;
	int Msg2Y = (GScreen->TotalVerSize + Msg1Y) / 2;

	const int ArrowSpace = 5;

	// Draw OutLine
	for (int i = MinY; i <= MaxY; i++)
	{
		GScreen->PrintHor(L' ', MinX + 1, i, MaxX - MinX, 7);
		if (i == MinY)
		{
			GScreen->PrintChar(L'┏', MinX, i);
			for (int j = MinX + 1; j <= MaxX - 1; j++)
				GScreen->PrintChar(L'━', j, i);
			GScreen->PrintChar(L'┓', MaxX, i);
		}
		else if (i == MaxY)
		{
			GScreen->PrintChar(L'┗', MinX, i);
			for (int j = MinX + 1; j <= MaxX - 1; j++)
				GScreen->PrintChar(L'━', j, i);
			GScreen->PrintChar(L'┛', MaxX, i);
		}
		else if (i == Msg1Y)
		{
			GScreen->PrintChar(L'┃', MinX, i);

			GScreen->PrintString(L"Congratulation! You Clear The Game", Msg1X, Msg1Y);

			GScreen->PrintChar(L'┃', MaxX, i);
		}
		else if (i == Msg2Y)
		{
			GScreen->PrintChar(L'┃', MinX, i);
			GScreen->PrintString(L"Press SpaceBar To Main", Msg2X, Msg2Y);
			GScreen->PrintChar(L'┃', MaxX, i);
		}
		else
		{
			GScreen->PrintChar(L'┃', MinX, i);
			GScreen->PrintChar(L'┃', MaxX, i);
		}
	}
}
