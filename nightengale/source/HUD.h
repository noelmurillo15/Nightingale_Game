#pragma once

#include "../SGD Wrappers/SGD_GraphicsManager.h"

class Player;


class HUD {

	Player* tmpPlayer = nullptr;

public:

	HUD();
	~HUD();

	int m_icurrSel;

	void Render(); 

	Player* GetCurrentPlayer(){ return tmpPlayer; }
	void SetCurrentPlayer(Player* player){ tmpPlayer = player; }

private:

	SGD::HTexture topHUD = SGD::INVALID_HANDLE;
	SGD::HTexture bottomHUD = SGD::INVALID_HANDLE;
	SGD::HTexture baitImg = SGD::INVALID_HANDLE;
	SGD::HTexture repellentImg = SGD::INVALID_HANDLE;
	SGD::HTexture analyzerImg = SGD::INVALID_HANDLE;
};