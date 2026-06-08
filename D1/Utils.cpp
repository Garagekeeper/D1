#include "Utils.h"
#include "Transform.h"
#include "Raycaster.h"
#include "Creature.h"
#include "GameEngine.h";

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

Creature* FindClosetTargetFromList(FRaycasterResult Res, FPos From, std::list<Creature*>& CreatureList)
{
	Creature* ClosestTarget = nullptr;
	// Ray 맞은 물체가 있고, 해당 물체가 벽보다 가까우면
	if (Res.bHit
		&& GameEngine::GetInstance()->GetScreen()->Zbuffer[TargetWidth / 2] > Res.PerpDist)
	{
		// 해당 위치에 생물을 모두 가져옴 (겹쳐 있을 수 있음)
		Creature* MinCreature = nullptr;

		// 플레이어와 가장 거리가 짧은 생물체를 선정
		double Min = INT32_MAX;
		for (auto e : CreatureList)
		{
			double Dist = GetSqrDist(e->GetTransform()->Pos, From);
			if (Dist < Min)
			{
				Min = Dist;
				MinCreature = e;
			}

		}
		ClosestTarget = MinCreature;
	}

	return ClosestTarget;
}
