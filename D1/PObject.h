#pragma once
#include "Transform.h"
#include "Sprite.h"


class PObject
{
protected:

	// Pos and Vector
	FTransform				Transform;
	FSprite					Sprites;


public:
							PObject() = default;
							PObject(FTransform _Transform, FSprite _Sprites) : Transform(_Transform), Sprites(_Sprites) {}
	virtual					~PObject();

	// Pos and Vector
	inline double			GetTheta() const { return Transform.Theta; }
	inline FSprite*			GetSprite() { return &Sprites; }
	inline FTransform*		GetTransform() { return &Transform; }


	virtual void			MoveTo(FPos NextPos);
	void					RotateTo(FVec NextVec, double _Theta);

};

