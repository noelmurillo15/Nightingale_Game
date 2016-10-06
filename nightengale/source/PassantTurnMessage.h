#pragma once
#include "..\SGD Wrappers\SGD_Message.h"
class Entity;
class AnimationTimestamp;
class PassantTurnMessage :
	public SGD::Message
{

public:
	/**********************************************************/
	// Constructor REQUIRES the entity to remove
	PassantTurnMessage(Entity* me, AnimationTimestamp* Other);
	virtual ~PassantTurnMessage();

	/**********************************************************/
	// Accessor
	Entity*		GetEntity(void) const		{ return m_pEntity; }
	AnimationTimestamp* GetOther() const { return m_pother; }
private:
	/**********************************************************/
	// entity to destroy
	Entity*		m_pEntity;
	AnimationTimestamp* m_pother;
};

