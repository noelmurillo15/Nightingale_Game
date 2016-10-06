#pragma once

#include "Entity.h"


class Rook : public Entity {

public:

	Rook();
	~Rook();	

	void Update(float elapsedTime);
	void Render(SGD::Point bawlz);

	void UpdateMoveSpeed();

	void PreformItemBehavaior(const IEntity* pOther);
	void HandleCollision(const IEntity* pOther);

	int GetType(void) const { return ENT_ANIMAL; }
	int GetCreature(void)const { return CRT_ROOK; }
	int GetDamage() const  { return m_iDamage; }	

private:

	int m_iDamage = 20;

	bool Scared = false;
	bool Asleep = false;
	bool IsStuffed = false;

	float WanderTimer;
	float SuperBait;
};