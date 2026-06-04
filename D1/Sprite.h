#pragma once
#include <vector>
#include <string>

struct FSprite
{
public:
	double X = 0.0;
	double Y = 0.0 ;
	double VMove = 0.0;
	int Width = 0;
	int Height = 0;

	std::vector<std::vector<std::wstring>> SpriteTexture;

public:
	FSprite() = default;
	FSprite(double X, double Y, double VMove, std::vector<std::vector<std::wstring>> SpriteTexture);

	virtual ~FSprite();

};
