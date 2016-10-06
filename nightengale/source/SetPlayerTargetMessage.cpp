#include "SetPlayerTargetMessage.h"
#include "MessageID.h"

SetPlayerTargetMessage::SetPlayerTargetMessage(Entity* customer) :Message(MessageID::MSG_SET_TARGET_PLAYER)
{
	m_pCustomer = customer;

}

