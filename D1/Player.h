#pragma once

typedef struct FPlayer
{
public:
	// player pos
	double X;
	double Y;

	// player dir vec
	double DirX;
	double DirY;
	double RightX;
	double RightY;

	// camera plane
	double PlaneX;
	double PlaneY;

	// double theta (rotation degree)
	double PlayerTheta;

public:
	void SetDirVec();
	double GetTheta();
}FPlayer;