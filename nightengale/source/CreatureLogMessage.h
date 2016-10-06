#pragma once
#include "../SGD Wrappers/SGD_Message.h"

class Entity; 
class CreatureLogMessage : public SGD::Message
{
public:
	CreatureLogMessage(Entity* creatureToAdd);
	~CreatureLogMessage();

	Entity* GetCreatureToAdd(void) const { return m_eCreature;  }
private:
	Entity* m_eCreature = nullptr; 
};

