#pragma once
#include <functional>
enum class EDir
{
	Up,
	Down,
	Left,
	Right,
	EDirLen,
};

struct FPos
{
	float X = 0;
	float Y = 0;
};

struct FIntPos
{
	int X = 0;
	int Y = 0;

	bool operator==(const FIntPos& Others) const
	{
		return (X == Others.X) && (Y == Others.Y);
	}

	bool operator!=(const FIntPos& Others) const 
	{
		return !(*this == Others);
	}

	bool operator<(const FIntPos& others) const 
	{
		if (X == others.X)
			return (Y < others.Y);

		return (X < others.X);
	}
};

namespace std
{
	template <>
	struct hash<FIntPos>
	{
		size_t operator()(const FIntPos& pos) const noexcept
		{
			// X와 Y의 해시값을 섞어서 고유한 값을 만듭니다 (비트 연산 활용)
			size_t h1 = hash<int>{}(pos.X);
			size_t h2 = hash<int>{}(pos.Y);
			return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
		}
	};
}


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
		RightVec.DirX = -DirVec.DirY;
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
