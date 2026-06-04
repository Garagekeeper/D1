#pragma once
struct FPos
{
	float X = 0;
	float Y = 0;
};

struct FVec
{
	double DirX = 0;
	double DirY = -1;
};

struct FTransform
{
	FPos					Pos;
	FVec					CameraDirVec;
	FVec					DirVec;
	FVec					RightVec;

	double					PlaneSize;
	double					Theta;

	FTransform(FPos _Pos, FVec _DirVec, double _PlaneSize)
	{
		Pos = _Pos;
		DirVec = _DirVec;
		PlaneSize = _PlaneSize;
		Theta = 0.0;

		// Right 벡터 
		RightVec.DirX = DirVec.DirY;
		RightVec.DirY = DirVec.DirX;

		// 카메라 벡터
		CameraDirVec.DirX = RightVec.DirX * PlaneSize;
		CameraDirVec.DirY = RightVec.DirY * PlaneSize;
	}

	void					MoveTo(FPos NextPos);
	void					RotateTo(FVec NextVec, double _Theta);


	inline FPos				GetPos() const { return Pos; }
	inline FVec				GetDirVec() const { return DirVec; }
	inline FVec				GetCameraDirVec() const { return CameraDirVec; }
	inline FVec				GetRightDirVec() const { return RightVec; }
};
