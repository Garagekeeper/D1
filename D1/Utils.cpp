#include "Utils.h"
#include "Transform.h"

double GetSqrLen(FVec Vec)
{
    return (Vec.DirX * Vec.DirX)+(Vec.DirY * Vec.DirY);
}

double GetSqrLen(FPos Pos)
{
    return (Pos.X * Pos.X) + (Pos.Y * Pos.Y);;
}
