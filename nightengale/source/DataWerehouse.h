#pragma once

#include "../SGD Wrappers/SGD_Geometry.h"

#include <map>
#include <string>
#include <vector>

using namespace std;

class Entity;
class CreatureProfile;


class DataWereHouse
{
public:
	DataWereHouse();
	~DataWereHouse();

	typedef pair <string, Entity*> IntrestKey;
	typedef pair <string, CreatureProfile*> IndexKey;

	map<string, Entity* >::iterator IntrestObj_iter;
	map<string, CreatureProfile*>::iterator CreatureIndex_iter;

	map <string, Entity*> DataPointObjects;
	map <string, CreatureProfile*> CreatureIndex;

	void CreateProfile(Entity* Self, CreatureProfile* Profile);
	void AddNewIndex(string CreatureName, CreatureProfile* NewProfile);

	int GetCurrentState(void) { return currentState; }
	int GetHunger(void) { return m_iHunger; }

	void SetCurrentState(int NewState){ currentState = NewState; }
	void SetHunger(int iHunger){ m_iHunger = iHunger; }


	vector<Entity*> m_vecPercived;
	vector<Entity*> m_vecLocated;
	vector<SGD::Point*> m_vecWayPoint;

	float SearchTimer = 0;
	float MemTransitionTimer = 0;
	int m_iHunger = 0;
	int currentState = 0;

private:

};