#pragma once

#include "Tile.h"

#include <vector>

class Player;
class Entity;

class Layer {	

	struct SpawnPts{		
		std::string m_name;
		std::vector<SGD::Point> m_worldPos;
	};

	struct Events{
		std::string m_name;
		std::vector<SGD::Rectangle> m_EventTriggers;
	};

	unsigned int DEBUG_MODE = DEBUG_OFF;

	std::vector<SpawnPts> m_SpawnPoints;
	std::vector<Events> m_Events;

	std::vector<SGD::Point> m_CollisionPoints;
	std::vector<SGD::Point> m_BushPoints;	

public:
	
	Layer();
	~Layer();

	bool CheckCollision(Entity*, SGD::Point, SGD::Point );
	bool CheckWater(Entity*, SGD::Point);

	SGD::Point FindSpawnPoint(std::string CreatureName);	
	SGD::Point FindOffScreenSpawnPt(std::string CreatureName);
	SGD::Point FindRandomBush();
	SGD::Point FindCloseBush(SGD::Rectangle);

	std::vector<SGD::Rectangle> FindEventTiles(std::string EventName);	
	std::vector<SGD::Point> GetBushPoints(){ return m_BushPoints; }
	std::vector<SGD::Point> GetCollisionPoints(){ return m_CollisionPoints; }

	unsigned int DebugMode() { return DEBUG_MODE; }
	
	void ResetLayer();
	void SetDebugMode(unsigned int mode);
	void AddCollisionPoints(SGD::Point);
	void AddBushPoints(SGD::Point);
	void AddSpawnPoints(SGD::Point wpos, std::string name);	
	void AddEvents(std::string name, std::vector<SGD::Rectangle>);	

	enum Mode{
		DEBUG_OFF,
		DEBUG_BASE,
		DEBUG_SENSERECTS,
		DEBUG_EMITRECTS,
		DEBUG_ENVIRONEMNT,
		DEBUG_ALL		
	};
};