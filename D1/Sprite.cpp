#include "Sprite.h"

FSprite::FSprite(double X, double Y, double VMove, std::vector<std::vector<std::wstring>> SpriteTexture)
{
	this->X = X;
	this->Y = Y;
	this->VMove = VMove;
	this->SpriteTexture = SpriteTexture;
	Width = static_cast<int>(SpriteTexture[0][0].length() + 1);
	Height = static_cast<int>(SpriteTexture[0].size());
}

FSprite::~FSprite()
{

}
