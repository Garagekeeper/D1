#include "PObject.h"

PObject::~PObject()
{

}

void PObject::MoveTo(FPos NextPos)
{
	Transform.MoveTo(NextPos);
}

void PObject::RotateTo(FVec NextVec, double _Theta)
{
	Transform.RotateTo(NextVec, _Theta);
}

