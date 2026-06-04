#include "Utils.h";
#include "Transform.h"
#include <cmath>

void FTransform::MoveTo(FPos NextPos)
{
	Pos = NextPos;
}

void FTransform::RotateTo(FVec NextVec, double _Theta)
{
	//TODO Sqrt 안쓰고 해보기
	auto Len = sqrt(GetSqrLen(NextVec));

	// 방향벡터 수정
	DirVec.DirX = NextVec.DirX / Len;
	DirVec.DirY = NextVec.DirY / Len;

	// 오른쪽을 가리키는 벡터 수정
	RightVec.DirX = -DirVec.DirY;
	RightVec.DirY = DirVec.DirX;

	// 카메라벡터 수정 (사실 RightVec이랑 똑같은데) 길이만 다름
	CameraDirVec.DirX = RightVec.DirX * PlaneSize;
	CameraDirVec.DirY = RightVec.DirY * PlaneSize;

	// 회전각 기록
	Theta += _Theta;
}