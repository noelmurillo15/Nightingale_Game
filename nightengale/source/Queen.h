#pragma once

#include "Entity.h"


class Queen : public Entity {
	bool Landing;
	short currAction;
	float LandTimer;
	enum actions {CON_FLY,CON_LAND,CON_REST,CON_TAKEOFF};
	int m_iDamage = 100;
	SGD::Vector direction = { -1, 0 };
	SGD::Vector SecDir = { -1, 0 };
	SGD::HAudio m_sAttack;
public:
	Queen();
	~Queen();
	void	Update(float elapsedTime);
	void	Render(SGD::Point bawlz);
	void HandleCollision(const IEntity* pOther);
	int GetType(void) const { return ENT_ANIMAL; }
	int GetCreature(void)const { return CRT_QUEEN; }
	int GetDamage() const  { return m_iDamage; }
	void PreformItemBehavaior(const IEntity* pOther);
	void UpdateMoveSpeed();
};

