#pragma once
#include <vector>
#include <string>

struct FSprite
{
public:
	double									X = 0.0;
	double									Y = 0.0 ;
	double									VMove = 0.0;
	int										Width = 0;
	int										Height = 0;
	int										BaseAttribute;

	std::vector<std::vector<std::wstring>>	SpriteTexture;

public:
											FSprite() = default;
											FSprite(double InX, double InY, double InVMove, int InBaseAttribute, std::vector<std::vector<std::wstring>> InSpriteTexture);

	virtual									~FSprite();

};
