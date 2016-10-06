#pragma once

#include "Game.h"


class IGameState {

protected:

	IGameState( void ) = default;
	virtual ~IGameState( void )	= default;

public:

	// IGameState Interface:
	virtual bool	Initialize(void)			= 0;	// Startup
	virtual void	Terminate(void)				= 0;	// Unpack
	virtual void	Enter( void )				= 0;	// load resources
	virtual void	Exit ( void )				= 0;	// unload resources
	virtual bool	Update( float elapsedTime )	= 0;	// handle input & update entites
	virtual void	Render( float elapsedTime )	= 0;	// render entities / menu
};