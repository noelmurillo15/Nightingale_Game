#include "AddDialogMsg.h"

#include "MessageID.h"


AddDialogMsg::AddDialogMsg(std::string text, std::string sentby) :Message(MessageID::MSG_ADD_DIALOG) {
	dialog = text;
	sender = sentby;
}