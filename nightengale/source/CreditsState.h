#pragma once

#include "IGameState.h"


class CreditsState : public IGameState {

public:
		//	Singleton:
	static CreditsState* GetInstance(void);

		//  Interface:
	virtual bool Initialize(void)  override;
	virtual void Terminate(void) override;
	virtual void Enter(void) override;	// load resources
	virtual void Exit(void) override;	// unload resources
	virtual bool Update(float elapsedTime)	override;	// handle input & update entites
	virtual void Render(float elapsedTime)	override;	// render entities / menu

private:
		// SINGLETON!
	CreditsState(void) = default;
	virtual ~CreditsState(void) = default;
	CreditsState(const CreditsState&) = delete;
	CreditsState& operator= (const CreditsState&) = delete;

		//	Audio
	SGD::HAudio m_hCreditsBGM = SGD::INVALID_HANDLE; 
	SGD::HAudio m_hLeaveCreditsSFX = SGD::INVALID_HANDLE; 
};