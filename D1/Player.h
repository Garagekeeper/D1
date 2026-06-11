#pragma once
#include "Creature.h"

class WorldManager;

class FPlayer : public Creature
{
private:
	const int		BulletMax = 6;
	int				CurrentBullet = BulletMax;
	double			BlinkDurationMax = 0.2;
	bool			bBlink = false;
	double			CurrenBlinkDuration = BlinkDurationMax;
public:
								//constructor
					FPlayer(FCreatureBaseStat _Stat, FTransform _Transform, FSprite _Sprites)
						: Creature(_Stat, _Transform, _Sprites) {}
					FPlayer(FTransform _Transform, FSprite _Sprites)
						: Creature({ 100,100,100 }, _Transform, _Sprites) {}

	void			Update(const WorldManager* World);
	void			Roate();
	void			Move(const WorldManager* World);
	void			UpdateState();
	void			UpdateStateBehavior(const WorldManager* World);
	inline bool		GetbBlink() const { return bBlink; }
	virtual void	Attack(Creature* Target) override;
	inline int		GetBullet() { return CurrentBullet; }
	inline int		GetMaxBullet() { return BulletMax; }

};