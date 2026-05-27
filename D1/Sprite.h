#pragma once
class FSprite
{
public:
	double X;
	double Y;

	const wchar_t** SpriteTexture;

public:
	FSprite() :X(0.0), Y(0.0), SpriteTexture(nullptr) {}
	FSprite(double X, double Y, const wchar_t** SpriteTexture);

	virtual ~FSprite();

};
