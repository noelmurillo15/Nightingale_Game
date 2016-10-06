#include "Tile.h"


Tile::Tile(){
	m_type = -1;
	m_explored = false;
}

Tile::Tile(SGD::Point worldPosition, unsigned int type){
	m_type = type;
	m_explored = false;
	m_worldPosition = worldPosition;	
}

Tile::~Tile(){

}