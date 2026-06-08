#include "Utils.h"
#include "Transform.h"
#include "Define.h"
#include "Renderer.h"
#include "WorldManager.h"

double GetSqrLen(FVec Vec)
{
    return (Vec.DirX * Vec.DirX)+(Vec.DirY * Vec.DirY);
}

double GetSqrLen(FPos Pos)
{
    return (Pos.X * Pos.X) + (Pos.Y * Pos.Y);;
}

double GetSqrDist(FPos P1, FPos P2)
{
    return ((P2.X - P1.X) * (P2.X - P1.X)) + ((P2.Y - P1.Y) * (P2.Y - P1.Y));
}

