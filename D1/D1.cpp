#include <time.h>

#include "D1.h"
#include "GameEngine.h"


int main()
{
	srand(static_cast<unsigned int>(time(0)));

	GameEngine::StartEngine();
	GameEngine* Engine = GameEngine::GetInstance();
	Engine->Init();
	Engine->Run();
	Engine->ClearEngine();

	/*

	최적화
	움직임 + 회전 시 프레임 드랍 보완 방법
	2. 회전 연산(삼각함수)의 오버헤드 체크
	3. DeltaTime 독립성 및 입력 루프 분리

	//TODO

	4. 게임 방향성 정하기
		- 슈팅

	스타 형식의 HUD, 왼쪽은 체력, 오른쪽은 탄약
	적 class 만들기(완)
	히트스캔 판정 연결(완)
	// 스프라이트 변경은 죽을때만, 나머지 경우는 깜빡이는 것만
	적 피격 스프라이트 (현재 피격시 내부에서 체력까지는 깎이는 상태, 완)
	적이 죽었을때 격자, 벡터에서 삭제 (완)
	적 움직이게 (현재 패트롤 완,)
	맵디자인을 최대한 광장이 없게
	벽쪽으로가면 스프라이트가 짤리는 현상 (벽에서 1 떨어지는 것으로 해결)

*/

}
