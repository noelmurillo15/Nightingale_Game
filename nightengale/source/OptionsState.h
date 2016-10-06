#pragma once

#include "IGameState.h"


class OptionsState : public IGameState {

	int	m_nCursor = 0;
	
public:
		//	Singleton
	static OptionsState* GetInstance(void);

		//	Interface:
	virtual bool	Initialize(void)  override;
	virtual void	Terminate(void)	 override;
	virtual void	Enter(void)				override;	// load resources
	virtual void	Exit(void)				override;	// unload resources
	virtual bool	Update(float elapsedTime)	override;	// handle input & update entites
	virtual void	Render(float elapsedTime)	override;	// render entities / menu

		//	Resize
	void ReSizeRects();
	bool Input();

private:
		// SINGLETON!
	OptionsState(void) = default;
	virtual ~OptionsState(void) = default;
	OptionsState(const OptionsState&) = delete;
	OptionsState& operator= (const OptionsState&) = delete;

	float arcadebuffer = 0.0f;

		//	Rects
	SGD::Rectangle m_optionRect;
	SGD::Rectangle m_MastervolRect;
	SGD::Rectangle m_SFXvolRect;

		//	Audio
	SGD::HAudio m_background = SGD::INVALID_HANDLE;
	SGD::HAudio m_hOptionsMenuOptionSwitchSFX = SGD::INVALID_HANDLE;
	SGD::HAudio m_hOptionsMenuOptionSelectedSFX = SGD::INVALID_HANDLE;			
};