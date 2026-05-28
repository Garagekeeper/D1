#include "Sprite.h"

FSprite::FSprite(double X, double Y, double VMove, const wchar_t** SpriteTexture)
{
	this->X = X;
	this->Y = Y;
	this->VMove = VMove;
	this->SpriteTexture = SpriteTexture;
}

FSprite::~FSprite()
{
	delete[] SpriteTexture;
	SpriteTexture = nullptr;
}
