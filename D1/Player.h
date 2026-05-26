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

	// camera plane for wall casting
	double PlaneX;
	double PlaneY;

	// camera plane for Florr, Ceiling castting
	double VPlaneX;
	double VPlaneY;

	// double theta (rotation degree)
	double PlayerTheta;

public:
	void SetDirVec();
	double GetTheta();
}FPlayer;