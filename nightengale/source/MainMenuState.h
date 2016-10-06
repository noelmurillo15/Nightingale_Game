#pragma once

#include "IGameState.h"


class MainMenuState : public IGameState {	

public:

		// Singleton Accessor
	static MainMenuState* GetInstance( void );

		// IGameState Interface:
	virtual bool	Initialize(void) override;
	virtual void	Terminate(void)  override;
	virtual void	Enter	( void )				override;	// load resources
	virtual void	Exit	( void )				override;	// unload resources
	virtual bool	Update	( float elapsedTime )	override;	// handle input & update entites
	virtual void	Render	( float elapsedTime )	override;	// render entities / menu

	bool Input();

private:
		
		// SINGLETON!
	MainMenuState( void )			= default;
	virtual ~MainMenuState( void )	= default;
	MainMenuState( const MainMenuState& )				= delete;	
	MainMenuState& operator= ( const MainMenuState& )	= delete;

		// Cursor Index
	int		m_nCursor = 0;
	float arcadebuffer = 0.0f;

	SGD::HAudio m_hMainMenuMusic = SGD::INVALID_HANDLE; 
	SGD::HAudio m_hMenuSelectionSFX = SGD::INVALID_HANDLE; 
	SGD::HAudio m_hMenuChangeSFX = SGD::INVALID_HANDLE; 	

	SGD::HTexture m_MainScreen = SGD::INVALID_HANDLE;
};