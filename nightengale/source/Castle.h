#pragma once

#include "Entity.h"


class Castle : public Entity {


public:
	Castle();
	~Castle();
	void	Update(float elapsedTime);
	void	Render(SGD::Point bawlz);
	void HandleCollision(const IEntity* pOther);
	int GetType(void) const { return ENT_ANIMAL; }
	int GetCreature(void)const { return CRT_CASTLE; }
	void UpdateMoveSpeed();
	void PreformItemBehavaior(const IEntity* pOther);
	int m_iDamage = 75;
	int GetDamage() const  { return m_iDamage; }
	SGD::HAudio m_hRevealScream = SGD::INVALID_HANDLE; 
	int GetDevouredCount(void) const { return m_iDevourCount;  }
private: 
	bool m_bIsRevealed = true; 
	bool m_bCanGrow = false; 
	float m_ReavealedTimer;
	int m_iDevourCount; 
	AnimationTimestamp* SecTimeStamp;
};