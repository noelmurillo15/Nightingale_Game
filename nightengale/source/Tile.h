#pragma once

#include "../SGD Wrappers/SGD_Geometry.h"

class Tile {

		//	Tile data	
	bool m_explored = false;
	unsigned int m_type;
	SGD::Point m_worldPosition;

public:

	Tile();
	Tile(SGD::Point worldPosition, unsigned int type);
	~Tile();

		//	Accessors	
	bool GetHasExplored()			{ return m_explored; }
	unsigned int GetTileType()		{ return m_type; }
	SGD::Point GetWorldPosition()	{ return m_worldPosition; }

	void ToggleHasExplored()	{ m_explored = true; }
};