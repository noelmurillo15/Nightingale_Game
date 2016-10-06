#pragma once

#include "IGameState.h"
class Controls;

class ControllerState : public IGameState {

	Controls* m_Controls = nullptr;

public:
	//	Singleton:
	static ControllerState* GetInstance(void);

	//  Interface:
	virtual bool Initialize(void)  override;
	virtual void Terminate(void) override;
	virtual void Enter(void) override;	// load resources
	virtual void Exit(void) override;	// unload resources
	virtual bool Update(float elapsedTime)	override;	// handle input & update entites
	virtual void Render(float elapsedTime)	override;	// render entities / menu

private:
	// SINGLETON!
	ControllerState(void) = default;
	virtual ~ControllerState(void) = default;
	ControllerState(const ControllerState&) = delete;
	ControllerState& operator= (const ControllerState&) = delete;
};