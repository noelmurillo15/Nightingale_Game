#pragma once
#include "Entity.h"


#define ACTION_DELAY 0.2f
#define BITE_CONTROL 1.1f

class Knight : public Entity
{
public:
	Knight();
	~Knight();


	void	Update(float elapsedTime);
	void	Render(SGD::Point);

	void	UpdateMoveSpeed(void);
	void	SetIsRepelled(bool Rep) { isRepelled = Rep; }
	void	SetIsBaityed(bool bait) { isBaited = bait; }

	int		GetType(void)	const { return Entity::ENT_ANIMAL; }
	int		GetCreature(void)const { return Entity::CRT_KNIGHT; }
	int		GetDamage() const { return m_damage; }
	void	HandleCollision(const IEntity* pOther);
	void	PreformItemBehavaior(const IEntity* pOther);
	bool	imoveTowards = true;
	bool	Following(Entity*);

	bool	GetIsRepelled(void)const { return isRepelled; }
	bool	GetIsBaited(void)const { return isBaited; }
	void Wander(float elapsedTime);
	void Decision(float elapsedTime);
	float DegreeBetween = 0;
	int m_damage = 15;



	SGD::Vector OffsetVector;
	SGD::Vector	FormationPosition;

private:

	bool isRepelled = false;
	bool isBaited = false;
	float m_fAttackCooldown = 0;
	float currentRotaion = 0;
	float RotaionDiffrence = 0;
	float RandDelay = 0;
	float UpdateControl = 0;
	float itemTimer = 0;
	float WanderControl = 0;
	float WanderTimer = 0;






};

