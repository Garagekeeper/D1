#pragma once
#include <list>
struct FVec;
struct FPos;
struct FIntPos;

class Creature;
struct FRaycasterResult;


double	GetSqrLen(FVec Vec);
double	GetSqrLen(FPos Pos);

double	GetSqrDist(FPos P1, FPos P2);
int		GetSqrDist(FIntPos P1, FIntPos P2);

Creature* FindClosetTargetFromList(FRaycasterResult Res, FPos From, std::list<Creature*>& CreatureList);
int GetRandInt(int Min, int Max);
int GetRandInt(int Max);
float GetRandRange();
