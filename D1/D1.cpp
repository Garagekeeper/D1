#include <time.h>

#include "D1.h"
#include "GameEngine.h"


int main()
{
	srand(static_cast<unsigned int>(time(0)));

	while (true)
	{
		GameEngine::StartEngine();
		GameEngine* Engine = GameEngine::GetInstance();
		Engine->Init();
		Engine->GameRun();
		if (Engine->GetIsExitComplete())
		{
			Engine->ClearEngine();
			break;
		}
		Engine->ClearEngine();
	}

	printf("게임 종료!");

	/*

	최적화
	움직임 + 회전 시 프레임 드랍 보완 방법
	2. 회전 연산(삼각함수)의 오버헤드 체크
	3. DeltaTime 독립성 및 입력 루프 분리

	//Done
		적 class 만들기(완)
		적 움직이게 (현재 패트롤 완, MoceTo Target with A* 까지 완)
		// 스프라이트 변경은 죽을때만, 나머지 경우는 깜빡이는 것만
		적 피격 스프라이트 (완)
		적 스프라이트 색 넣기(완)
		적이 죽었을때 스프라이트 변경(완)
		적이 죽었을때 격자, 벡터에서 삭제 (완)
		벽쪽으로가면 스프라이트가 짤리는 현상
		- 적이 정수 좌푤를가질때 벽에 반쯤 파묻히는 현상이었는데, 길 찾기시 0.5를 더해줘서 중심으로 다니게 조절함

		히트스캔 판정 연결(완)
		공격 스프라이트 만들기(완)
		플레이어 피격 만들기(구현하긴 했는데, 화면 전체를 덮을지 체커 무늬로 할지 .. 별로 맘에 안들어서 적용 안할듯)
		플레이어 사망 화면 만들기
			- 메인으로 돌아가기

		게임 클리어 화면 만들기
		- 메인으로

		미니맵(맵, 플레이어 완)

	//TODO


	
	
	플레이어 사망시 유다희 화면 만들기
	
	시작할 때 플레이어 이름 입력받기
	플레이어 이름 HUD에 띄우기
	HUD꾸미기
	미니맵에 오브젝트 표시하기
	게임 클리어 조건 만들기

	게임 클리어 화면 만들기
		- 최고기록 표시하기

	게임내 오브젝트 배치
		- 깃발 (클리어)
		- 열쇠 (클리어 조건)
	

	플레이어 사망 화면 꾸미기


	적절한 레벨 디자인 하기 (AI 힘 빌리기)

	문서 작성시 전체 클래스 목록과 클래스별 기능을 적어주기
*/

}
