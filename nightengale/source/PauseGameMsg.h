#pragma once

#include "../SGD Wrappers/SGD_Message.h"


class PauseGameMsg : public SGD::Message {

public:

	PauseGameMsg();
	~PauseGameMsg() = default;
};