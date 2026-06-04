#include "PObject.h"

PObject::PObject(FPos _Pos, FVec _DirVec, double _PlaneSize, double _Theta)
	: Pos(_Pos), DirVec(_DirVec), PlaneSize(_PlaneSize), Theta(_Theta)
{
	// Right 벡터 
	RightVec.DirX = -DirVec.DirY;
	RightVec.DirY = DirVec.DirX;

	// 카메라 벡터
	CameraDirVec.DirX = RightVec.DirX * PlaneSize;
	CameraDirVec.DirY = RightVec.DirY * PlaneSize;
}
