/***************************************************************
|	File:		DestroyEntityMessage.cpp
|	Author:		
|	Course:		
|	Purpose:	DestroyEntityMessage class stores the entity
|				to be removed from the Entity Manager
|				when the message is processed
***************************************************************/

#include "DestroyEntityMessage.h"
#include "MessageID.h"
#include "Entity.h"
#include <cassert>


/**************************************************************/
// CONSTRUCTOR
//	- hold a reference to the pointer
DestroyEntityMessage::DestroyEntityMessage( Entity* ptr )
	: Message( MessageID::MSG_DESTROY_ENTITY )
{
	// Validate the parameter
	assert( ptr != nullptr 
			&& "DestroyEntityMessage - cannot destroy null" );

	// Store the parameter
	m_pEntity = ptr;
}

/**************************************************************/
// DESTRUCTOR
//	- release the reference
DestroyEntityMessage::~DestroyEntityMessage()
{
	//delete m_pEntity;
	m_pEntity = nullptr;
}
