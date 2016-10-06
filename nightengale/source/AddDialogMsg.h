#pragma once

#include "../SGD Wrappers/SGD_Message.h"
#include <string>


class AddDialogMsg : public SGD::Message {

	

public:

	AddDialogMsg(std::string text, std::string sentby);
	~AddDialogMsg() = default;

	std::string dialog;
	std::string sender;
};