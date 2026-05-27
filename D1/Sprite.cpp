#include "Sprite.h"

FSprite::FSprite(double X, double Y, const wchar_t** SpriteTexture)
{
	this->X = X;
	this->Y = Y;
	this->SpriteTexture = SpriteTexture;
}

FSprite::~FSprite()
{
	delete[] SpriteTexture;
	SpriteTexture = nullptr;
}
