#include "Game.h"

#include "MainMenuState.h"
#include "Camera.h"

#include <ctime>		//	time
#include <cstdlib>		//	srand + rand
#include <Windows.h>	//	GetTickCount
#define WIN32_LEAN_AND_MEAN


Game* Game::s_pInstance = nullptr;

Game* Game::GetInstance( void ) {
	if( s_pInstance == nullptr )
		s_pInstance = new Game;

	return s_pInstance;
}

void Game::DeleteInstance( void ) {
	delete s_pInstance;
	s_pInstance = nullptr;
}

bool Game::Initialize( float width, float height ) {

		// Seed First!
	srand( (unsigned int)time( nullptr ) );

	if( SGD::AudioManager::GetInstance()->Initialize() == false 
		|| SGD::GraphicsManager::GetInstance()->Initialize( false ) == false
		|| SGD::InputManager::GetInstance()->Initialize() == false
		|| MainMenuState::GetInstance()->Initialize() == false)
		return false;

	m_ScreenRes.width = width;
	m_ScreenRes.height = height;

		// Allocate & initialize the font
	m_pFont = new BitmapFont;
	m_pFont->Initialize();

		// Start the game in the Main Menu state
	ChangeState(MainMenuState::GetInstance());
	
		// Store the current time (in milliseconds)
	m_ulGameTime = GetTickCount();

	if (m_backgroundtheme != SGD::INVALID_HANDLE)
		SGD::AudioManager::GetInstance()->PlayAudio(m_backgroundtheme, true);

	return true;	// success!
}

int Game::Update(void) {

	// Update the wrappers
	if (SGD::AudioManager::GetInstance()->Update() == false
		|| SGD::GraphicsManager::GetInstance()->Update() == false
		|| SGD::InputManager::GetInstance()->Update() == false)
		return -10;		// exit FAILURE!	

	// Calculate the elapsed time between frames
	unsigned long now = GetTickCount();					// current time in milliseconds
	float elapsedTime = (now - m_ulGameTime) / 1000.0f;	// convert to fraction of a second
	m_ulGameTime = now;									// store the current time

	//	Detect current input
	SetCurrentInput();

#pragma region Change ScreenRes
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	if (pInput->IsKeyDown(SGD::Key::Alt) == true && pInput->IsKeyPressed(SGD::Key::Enter) == true && Game::GetInstance()->GetCurrentRes() != RES_FULLSCREEN)
		Game::GetInstance()->SetCurrentRes(RES_FULLSCREEN);

	else if (pInput->IsKeyDown(SGD::Key::Alt) == true && pInput->IsKeyPressed(SGD::Key::Enter) == true && Game::GetInstance()->GetCurrentRes() == RES_FULLSCREEN)
		Game::GetInstance()->SetCurrentRes(RES_BASE);

	if (pInput->IsKeyDown(SGD::Key::Alt) == true && pInput->IsKeyPressed(SGD::Key::A) == true && Game::GetInstance()->GetCurrentRes() != RES_ARCADE)
		Game::GetInstance()->SetCurrentRes(RES_ARCADE);

	else if (pInput->IsKeyDown(SGD::Key::Alt) == true && pInput->IsKeyPressed(SGD::Key::A) == true && Game::GetInstance()->GetCurrentRes() == RES_ARCADE)
		Game::GetInstance()->SetCurrentRes(RES_BASE);
#pragma endregion

	// Cap the elapsed time to 1/8th of a second
	if (elapsedTime > 0.125f)
		elapsedTime = 0.125f;

	// Let the current state handle input
	if (m_pCurrState->Update(elapsedTime) == false)
		return 1;	// exit success!	

	// Let the current state render

	m_pCurrState->Render(elapsedTime);

		//	Cap frame rate to around 60 fps
	while ((GetTickCount() - now) < 15);

	return 0;		// keep playing!
}

void Game::Terminate( void ) {
		//	Terminate Main menu
	MainMenuState::GetInstance()->Terminate();

		// Exit the current state
	ChangeState( nullptr );

		// Terminate & deallocate the font
	m_pFont->Terminate();
	delete m_pFont;
	m_pFont = nullptr;

		//	Unload background music
	SGD::AudioManager::GetInstance()->UnloadAudio(m_backgroundtheme);

		// Terminate the core SGD wrappers
	SGD::AudioManager::GetInstance()->Terminate();
	SGD::AudioManager::DeleteInstance();

	SGD::GraphicsManager::GetInstance()->Terminate();
	SGD::GraphicsManager::DeleteInstance();

	SGD::InputManager::GetInstance()->Terminate();
	SGD::InputManager::DeleteInstance();		
}

void Game::ChangeState( IGameState* pNewState ) {
		// Exit the old state
	if( m_pCurrState != nullptr )
		m_pCurrState->Exit();

		// Store the new state
	m_pCurrState = pNewState;

		// Enter the new state
	if( m_pCurrState != nullptr )
		m_pCurrState->Enter();
}

void Game::SetCurrentInput(){
	SGD::InputManager* input = SGD::InputManager::GetInstance();

	if (GetCurrentRes() == RES_ARCADE){
		m_CurrInput = CTRL_ARCADE;
		return;
	}
	else{
		if (input->IsControllerConnected(0))
			m_CurrInput = CTRL_CONTROLLER;
		else
			m_CurrInput = CTRL_KEYBOARD;
	}
}

void Game::ChangeScreenRes(float width, float height){
	m_ScreenRes.width = width;
	m_ScreenRes.height = height;

	if (m_CurrScreenRes == RES_BASE)
		SGD::GraphicsManager::GetInstance()->Resize({ width, height });
	else
		SGD::GraphicsManager::GetInstance()->Resize({ width, height }, false);

	Camera::GetInstance()->ResetCamera();
}

void Game::SetCurrentRes(unsigned int res) {

	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);

	m_CurrScreenRes = res;

	switch (m_CurrScreenRes){

	case RES_BASE:
		ChangeScreenRes((float)desktop.right - 256, (float)desktop.bottom - 256);
		break;

	case RES_FULLSCREEN:
		ChangeScreenRes((float)desktop.right, (float)desktop.bottom);
		break;

	case RES_ARCADE:
		ChangeScreenRes(800.0f, 600.0f);
		break;

	default:
		ChangeScreenRes(1024, 768);
		break;
	}
}