#pragma once

#include "Entity.h"


class Pawn : public Entity {	
	
public:

	Pawn();
	~Pawn();

	void Update(float elapsedTime);
	void Render(SGD::Point bawlz);
	void UpdateMoveSpeed();	

	int GetType(void) const { return ENT_ANIMAL; }
	int GetCreature(void)const { return CRT_PAWN; }
	int GetDamage() const  { return m_iDamage; }

	void HandleCollision(const IEntity* pOther);
	void PreformItemBehavaior(const IEntity* pOther);	

private:
	int m_iDamage = 10;
	int baitsneededforlovemode = 0;

	float stuffedtimer = 0.0f;

	bool isstuffed = false;

	SGD::HTexture heart = SGD::INVALID_HANDLE;
};