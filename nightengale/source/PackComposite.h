#pragma once

#include "Entity.h"
#include "Formation.h"
#include <vector>

using namespace std;

class PackComposite : public Entity
{
public:
	PackComposite();
	~PackComposite();

	void	Update(float elapsedTime);
	void	Render(SGD::Point);
	void	UpdateMoveSpeed(void);
	void HandleCollision(const IEntity* pOther);
	int GetType(void) const { return ENT_PACK; }
	int GetCreature(void)const { return CRT_PACK; }
	void PreformItemBehavaior(const IEntity* pOther);

	void PackDecisions(float elapsedTime);
	void DetachPackMember(Entity*);
	void PackDisband(void);
	bool GetPackCondition(void);
	vector<Entity*> m_vecPack;

	Entity* m_packLeader = nullptr;
	float m_statedecay = 0;

	int m_iCurrentFormation = 0;
private:
	Formation FormationControl = Formation();


};

