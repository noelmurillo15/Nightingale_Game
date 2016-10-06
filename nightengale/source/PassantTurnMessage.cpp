#include "PassantTurnMessage.h"
#include "MessageID.h"
#include "Entity.h"
#include "AnimationSystem.h"
#include <cassert>

PassantTurnMessage::PassantTurnMessage(Entity* me, AnimationTimestamp* other)
: Message(MessageID::MSG_PASSANT_TURN)
{
	// Validate the parameter
	assert(me != nullptr
		&& "PassantTurnMessage - cannot turn null");

	// Store the parameter
	m_pEntity = me;
	m_pother = other;
}


PassantTurnMessage::~PassantTurnMessage()
{
	m_pEntity = nullptr;
}


