#pragma once

#include "BitmapFont.h"

#include "../SGD Wrappers/SGD_Handle.h"			// uses HTexture & HAudio
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"

class IGameState;


class Game {

public:

		//	Singleton
	static Game*	GetInstance(void);
	static void		DeleteInstance(void);

		//	Interface
	bool Initialize(float width, float height);
	void Terminate(void);
	int	 Update(void);
	
		//	Screen Resolution
	void ChangeScreenRes(float width, float height);
	SGD::Size GetScreenRes(){ return m_ScreenRes; }

	unsigned int GetCurrentRes()const { return m_CurrScreenRes; }
	void SetCurrentRes(unsigned int res);

		//	Input
	unsigned int GetCurrentInput()const { return m_CurrInput; }
	void SetCurrentInput();

		//	Bitmap font
	BitmapFont*	GetFont() const	{ return m_pFont; }

		//	States
	void ChangeState(IGameState* pNewState);

	enum ScreenRes{
		RES_BASE,
		RES_FULLSCREEN,
		RES_ARCADE,
	};

	enum Input{
		CTRL_KEYBOARD,
		CTRL_CONTROLLER,
		CTRL_ARCADE,
	};

private:
		// Singleton Object:
	static Game*	s_pInstance;
	Game(void) = default;		// default constructor
	~Game(void) = default;		// destructor
	Game(const Game&) = delete;	// copy constructor
	Game& operator= (const Game&) = delete;	// assignment operator	

		// Screen Size
	SGD::Size m_ScreenRes;
		// Game Font
	BitmapFont*	m_pFont = nullptr;
		// Current Game State
	IGameState*	m_pCurrState = nullptr;
		// Game Time
	unsigned long m_ulGameTime = 0;
		//	Screen Res
	unsigned int m_CurrScreenRes = RES_BASE;
		//	Input
	unsigned int m_CurrInput = CTRL_KEYBOARD;
		//	Background
	SGD::HAudio m_backgroundtheme = SGD::INVALID_HANDLE;
};