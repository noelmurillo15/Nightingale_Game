#pragma once

#include "Entity.h"

struct BishopDirt; 

class Bishop : public Entity {

	bool m_bAttacking; 
public:

	Bishop();
	~Bishop();

	void	Update(float elapsedTime) override;
	void	Render(SGD::Point bawlz);

	int m_iDamage = 20;
	int GetDamage() const  { return m_iDamage; }

	void HandleCollision(const IEntity* pOther);
	int GetType(void) const { return ENT_ANIMAL; }
	int GetCreature(void)const { return CRT_BISHOP; }
	void UpdateMoveSpeed();
	void PerformItemBehavior(const IEntity* pOther);

	SGD::HAudio m_hBishopSFX = SGD::INVALID_HANDLE; 
	SGD::HTexture m_hBishopDirtTrail = SGD::INVALID_HANDLE; 
	SGD::Point wanderDestination;
	float wanderTimer;
	float m_fFearTimer; 
	float m_fDirtTimer; 
	vector<BishopDirt> DirtPath; 
};