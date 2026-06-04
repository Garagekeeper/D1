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

	double GEtSqrLen()
	{
		return (DirX * DirX) + (DirY * DirY);
	}
};

class PObject
{
protected:

	// Pos and Vector
	FPos					Pos;
	FVec					CameraDirVec;
	FVec					DirVec;
	FVec					RightVec;

	double					PlaneSize;
	double					Theta;


public:
							PObject() = default;
							PObject(FPos _Pos, FVec _DirVec, double _PlaneSize, double _Theta);
	virtual					~PObject() = default;

	// Pos and Vector
	inline FPos				GetPos() const { return Pos; }
	inline FVec				GetDirVec() const { return DirVec; }
	inline FVec				GetCameraDirVec() const { return CameraDirVec; }
	inline FVec				GetRightDirVec() const { return RightVec; }
	inline double			GetTheta() const { return Theta; }

};

