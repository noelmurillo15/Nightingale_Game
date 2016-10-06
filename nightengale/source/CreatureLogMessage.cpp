#include "CreatureLogMessage.h"
#include "MessageID.h"

CreatureLogMessage::CreatureLogMessage(Entity* creatureToAdd) : Message(MessageID::MSG_ADD_TO_CREATURE_LOG)
{
	if (creatureToAdd != nullptr)
		m_eCreature = creatureToAdd;
}


CreatureLogMessage::~CreatureLogMessage()
{
}
