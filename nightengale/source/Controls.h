#pragma once

#include "../SGD Wrappers/SGD_Handle.h"


class Controls {

public:

	Controls();
	~Controls();

	void Update(float elapsedTime); 
	void Render(unsigned int); 

private: 

	SGD::HTexture m_hArcadeControls = SGD::INVALID_HANDLE;
	SGD::HTexture m_hKeyboardControls = SGD::INVALID_HANDLE;
	SGD::HTexture m_hControllerControls = SGD::INVALID_HANDLE;
};