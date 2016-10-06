#pragma once

#include <vector>		

#include "../SGD Wrappers/SGD_Geometry.h"

class IEntity;			
class EntityManager; 
class Entity;


class Senses {

	float SenseTimer = 0.0f;	

public:

	Senses();
	Senses(unsigned int _Type);
	~Senses();

	typedef std::vector< IEntity* >		EntityVector;
	typedef std::vector< EntityVector >	EntityTable;

	void CheckAllSenses(Entity* self);
	bool CheckListenCollision(Entity* self);
	int CheckSmellTrail(Entity* self);

	void UpdateTimer(float ElapsedTime);
	float GetSensesTimer(){ return SenseTimer; }

	int CheckProximity(Entity* self);

	EntityManager* m_pLiasion = nullptr;
	void ProjectSight(Entity* Self);

	enum Proximity{
		NOT_SENSED,
		VISIBLE,
		CAN_HEAR,
	};
};