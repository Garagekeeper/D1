#include "Player.h"

//void FPlayer::SetDirVec()
//{
//	int NumForDecision = static_cast<int>(PlayerTheta);
//	if (NumForDecision < 0)
//		NumForDecision += 360;
//	if (NumForDecision >= 360)
//		NumForDecision -= 360;
//	//  0.0, -1.0, 0.66, 0.0
//	switch (NumForDecision)
//	{
//		case 0:
//			DirX = 0.0;		DirY = -1.0;
//			PlaneX = 0.66, PlaneY = 0.0;
//			RightX = -1.0, RightY = 0.0;
//			break;
//
//		case 45:
//			DirX = 1.0;	DirY = -1.0;
//			PlaneX = 0.4667, PlaneY = 0.4667;
//			RightX = -1.0, RightY = -1.0;
//			break;
//
//		case 90:
//			DirX = 1.0;	DirY = 0.0;
//			PlaneX = 0.0, PlaneY = 0.66;
//			RightX = 0.0, RightY = -1.0;
//			break;
//
//		case 135:
//			DirX = 1.0;	DirY = 1.0;
//			PlaneX = -0.4667, PlaneY = 0.4667;
//			RightX = 1.0, RightY = -1.0;
//			break;
//
//		case 180:
//			DirX = 0.0;		DirY = 1.0;
//			PlaneX = -0.66, PlaneY = 0.0;
//			RightX = 1.0, RightY = 0.0;
//			break;
//
//		case 225:
//			DirX = -1.0;	DirY = 1.0;
//			PlaneX = -0.4667, PlaneY = -0.4667;
//			RightX = 1.0, RightY = 1.0;
//			break;
//
//		case 270:
//			DirX = -1.0;	DirY = 0.0;
//			PlaneX = 0.0, PlaneY = -0.66;
//			RightX = 0.0, RightY = 1.0;
//			break;
//
//		case 315:
//			DirX = -1.0;	DirY = -1.0;
//			PlaneX = 0.4667, PlaneY = -0.4667;
//			RightX = -1.0, RightY = 1.0;
//			break;
//	}
//}

FPlayer::FPlayer(FCreatureBaseStat _Stat, FPos _Pos, FVec _DirVec, double PlaneSize) 
	: Creature(_Stat, _Pos, _DirVec, PlaneSize)
{

}
