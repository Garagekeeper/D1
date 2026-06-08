#pragma once
#include <list>
struct FVec;
struct FPos;

class Creature;
struct FRaycasterResult;


double	GetSqrLen(FVec Vec);
double	GetSqrLen(FPos Pos);

double	GetSqrDist(FPos P1, FPos P2);

Creature* FindClosetTargetFromList(FRaycasterResult Res, FPos From, std::list<Creature*>& CreatureList);
