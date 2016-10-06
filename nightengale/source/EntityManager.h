#pragma once
#include <vector>		// uses std::vector
#include "../SGD Wrappers/SGD_Geometry.h"

class IEntity;			// uses IEntity*
class Entity;
class Player;

using namespace std;
class EntityManager {

public:

	/**********************************************************/
	// Default constructor & destructor
	EntityManager();
	~EntityManager( void )	= default;
	

	vector<SGD::Rectangle> GvG;
	/**********************************************************/
	// Entity Storage:
	void	AddEntity	( IEntity* pEntity, unsigned int bucket );
	void	RemoveEntity( IEntity* pEntity, unsigned int bucket );
	void	RemoveEntity( IEntity* pEntity );
	void	RemoveACertainBucket	( unsigned int bucket );
	void	RemoveAll	( void );

	/**********************************************************/
	// Entity Upkeep:
	void	UpdateAll	( float elapsedTime );
	void	RenderAll(SGD::Point offset);

	void	CheckCollisions( unsigned int bucket1, unsigned int bucket2 );
	void	CheckEnvironmentCollisions(unsigned int bucket1);
		


	/**********************************************************/
	// Typedefs will simplify the templates
	typedef std::vector< IEntity* >		EntityVector;
	typedef std::vector< EntityVector >	EntityTable;

	/**********************************************************/
	// members:
	EntityTable	m_tEntities;			// vector-of-vector-of-IEntity* (2D table)
	
	EntityTable GetTable() { return m_tEntities; }
	

	
private:
	/**********************************************************/
	// Not a singleton, but still don't want the Trilogy-of-Evil
	EntityManager( const EntityManager& )				= delete;
	EntityManager& operator= ( const EntityManager& )	= delete;

	bool		m_bIterating = false;	// read/write lock
};