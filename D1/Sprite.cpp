#include "Sprite.h"

FSprite::FSprite(double InX, double InY, double InVMove, int InBaseAttribute, std::vector<std::vector<std::wstring>> InSpriteTexture)
{
	X = InX;
	Y = InY;
	VMove = InVMove;
	BaseAttribute = InBaseAttribute;
	SpriteTexture = InSpriteTexture;
	Width = static_cast<int>(SpriteTexture[0][0].length());
	Height = static_cast<int>(SpriteTexture[0].size());
}

FSprite::~FSprite()
{

}
