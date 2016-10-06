#pragma once
#include "../SGD Wrappers/SGD_Message.h"

class Entity;
class SetPlayerTargetMessage : public SGD::Message
{
public:
	SetPlayerTargetMessage(Entity* Customer);
	~SetPlayerTargetMessage() = default;

	Entity* m_pCustomer = nullptr;
};

