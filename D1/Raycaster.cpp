#include "Raycaster.h"
#include "GameEngine.h"

FRaycasterResult DDA(const int X, ERayCastLayer TargetLayer, const WorldManager* World)
{
	FRaycasterResult Res;

	Screen* GScreen = GameEngine::GetInstance()->GetScreen();
	const int WIDTH = GScreen->SceneHorSize;
	const int HEIGHT = GScreen->SceneVerSize;
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
		if (MapPosX <= 0 || MapPosX >= World->mapWidth) break;
		if (MapPosY <= 0 || MapPosY >= World->mapHeight) break;
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

		if (MapPosX <= 0 || MapPosX >= World->mapWidth) break;
		if (MapPosY <= 0 || MapPosY >= World->mapHeight) break;

		if (TargetLayer == ERayCastLayer::WALL)
		{
			// 해당 격자에 벽, 오브젝트, 적이 있는지 확인
			if (World->GetWorldMap()[MapPosY][MapPosX] == static_cast<int>(Env::WALL)) Res.bHit = true;
		}
		else if (TargetLayer == ERayCastLayer::Creature)
		{
			if (!World->GetCreatureMap()[MapPosY][MapPosX].empty()) Res.bHit = true;
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

	Res.MapPos.X = static_cast<float>(MapPosX);
	Res.MapPos.Y = static_cast<float>(MapPosY);

	if (Res.Side == 0)
		Res.PerpDist = (SideDistX - DeltaDistX);
	else
		Res.PerpDist = (SideDistY - DeltaDistY);

	if (Res.PerpDist < 0.001)
		Res.PerpDist = 0.001;

	return Res;
}
