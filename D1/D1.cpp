#include "D1.h"
#include "GameEngine.h"


int main()
{
	GameEngine::StartEngine();
	GameEngine* Engine = GameEngine::GetInstance();
	Engine->Init();
	Engine->Run();

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
	적 피격 스프라이트 (현재 피격시 내부에서 체력까지는 깎이는 상태)
	적 움직이게
	맵디자인을 최대한 광장이 없게

*/


}
