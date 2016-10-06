#pragma once

#include "Entity.h"


class Prancer : public Entity {

	bool m_isInfected = false;
	int m_iDamage = 35;
	float attacktime = 4.0f;

public:
	Prancer();
	~Prancer();

	bool ATTACK = false;
	bool RUNNIGGA = false;
	bool TAUNT = false;

	

	void Update(float elapsedTime);
	void Render(SGD::Point bawlz);
	void HandleCollision(const IEntity* pOther);
	int GetType(void) const { return ENT_ANIMAL; }
	int GetCreature(void)const { return CRT_PRANCER; }
	int GetDamage() const  { return m_iDamage; }
	void PreformItemBehavaior(const IEntity* pOther);
	void UpdateMoveSpeed();
};