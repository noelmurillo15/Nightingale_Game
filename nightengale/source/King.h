#pragma once

#include "Entity.h"


class King : public Entity {

	SGD::HAudio m_hPassiveNoise = SGD::INVALID_HANDLE; 
	SGD::HAudio m_hCuriousNoise = SGD::INVALID_HANDLE; 
	SGD::HAudio m_hAggroNoise = SGD::INVALID_HANDLE; 

	float m_fItemTimer; 
	float m_fAttentionSpan; 
	float attacktime = 4.0f;
	bool Sit = true;
public:

	King();
	~King();
	void Update(float elapsedTime);
	void Render(SGD::Point bawlz);
	void HandleCollision(const IEntity* pOther);
	int GetType(void) const { return ENT_ANIMAL; }
	int GetCreature(void)const { return CRT_KING; }
	int m_iDamage = 90;
	int GetDamage() const  { return m_iDamage; }
	void PerformItemBehavior(const IEntity* pOther);
	void UpdateMoveSpeed();
	SGD::Point wanderDestination; 
	float m_fHuntTimer; 
	float m_fPassiveTimer; 
};