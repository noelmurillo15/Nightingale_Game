#pragma once

#include "Entity.h"

class Bumbles : public Entity {


public:
	Bumbles();
	~Bumbles();
	
	void	Update(float elapsedTime);
	void	Render(SGD::Point bawlz);
	int GetType(void) const { return ENT_ANIMAL; }
	int GetCreature(void)const { return CRT_BUMBLES; }
	void HandleCollision(const IEntity* pOther);
	void PreformItemBehavaior(const IEntity* pOther);
	float attacktime = 0;
	void UpdateMoveSpeed();


	bool InWater = true;
	bool m_bTransitioning = false;
	bool m_ResetTrans = false;
private:
	AnimationTimestamp* TransitioningTimeStamp = nullptr;
	AnimationTimestamp* InWaterTimeStamp = nullptr;
	float WanderControl = 0;
	float WanderTimer = 0;
	void Decision(float elaspedtime);
	void Wander(float elaspedtime);

};