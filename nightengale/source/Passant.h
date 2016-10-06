#pragma once

#include "Entity.h"


class Passant : public Entity {

	SGD::HAudio m_sAttack;
	int m_iDamage = 23;

public:
	Passant();
	~Passant();

	AnimationTimestamp* SecTimeStamp;

	void Update(float elapsedTime);
	void Render(SGD::Point bawlz);
	void HandleCollision(const IEntity* pOther);
	int GetType(void) const { return ENT_ANIMAL; }
	int GetCreature(void)const { return CRT_PASSANT; }
	int GetDamage() const  { return m_iDamage; }
	void PreformItemBehavaior(const IEntity* pOther);
	void UpdateMoveSpeed();
};