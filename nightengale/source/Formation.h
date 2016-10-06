#pragma once
#include "../SGD Wrappers/SGD_Geometry.h"
#include <vector>

class Entity;
using namespace std;
class Formation
{



public:
	enum FormationTitle{ FRM_SCREEN, FRM_DIAMOND, FRM_CIRCLE };

	Formation();
	Formation(Entity* Creature);
	~Formation();
	void Update(float ElapsedTime, int CurrentForm);
	SGD::Point OffSetPosition(Entity* Creature, int CurrentForm);
	Entity* FormationCore = nullptr;
	bool m_bActive;
private:
	vector<bool> PackPosition;
};

