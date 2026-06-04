#pragma once
#include <vector>
#include <string>

struct FSprite
{
public:
	double X;
	double Y;
	double VMove;
	int Width;
	int Height;

	std::vector<std::vector<std::wstring>> SpriteTexture;

public:
	FSprite() = default;
	FSprite(double X, double Y, double VMove, std::vector<std::vector<std::wstring>> SpriteTexture);

	virtual ~FSprite();

};
