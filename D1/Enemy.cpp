#include "Enemy.h"
#include <algorithm>

void FEnemy::MoveToWithArray(FPos NextPos, std::vector<std::vector<std::list<Creature*>>>* CreatureMap)
{
	FPos CurrPos = Transform.Pos;
	std::list<Creature*>* CurrList = &(*CreatureMap)[static_cast<int>(CurrPos.Y)][static_cast<int>(CurrPos.X)];
	auto itr = std::find(CurrList->begin(), CurrList->end(), this);
	if (itr != CurrList->end())
	{
		CurrList->erase(itr);
	}
	
	std::list<Creature*>* NextList = &(*CreatureMap)[static_cast<int>(NextPos.Y)][static_cast<int>(NextPos.X)];
	NextList->push_back(this);
	MoveTo(NextPos);
}

void FEnemy::Update()
{

}


