#pragma once

#include "IGameState.h"


class LoadMenuState : public IGameState {

public:
		//	Singleton:
	static LoadMenuState* GetInstance(void);

		//	Interface:
	virtual bool	Initialize(void) override;
	virtual void	Terminate(void)  override;
	virtual void	Enter(void)				override;	// load resources
	virtual void	Exit(void)				override;	// unload resources
	virtual bool	Update(float elapsedTime)	override;	// handle input & update entites
	virtual void	Render(float elapsedTime)	override;	// render entities / menu
	
	bool Input();
	bool Selection();
	
	struct SaveSlotInfo{
		int elapsedTime;
		int Worldnum;
		std::string charName;
	};
	
private:
		// SINGLETON!
	LoadMenuState(void) = default;
	virtual ~LoadMenuState(void) = default;
	LoadMenuState(const LoadMenuState&) = delete;
	LoadMenuState& operator= (const LoadMenuState&) = delete;

	int	 m_nCursor = 0;
	int CurrSaveSlot = 0;
	float m_fScreenWidth = 0;
	float m_fScreenHeight = 0;

	SGD::Rectangle m_svSlot1, m_svSlot2, m_svSlot3, m_svExit;

	SGD::HAudio m_hLoadScreenBGM = SGD::INVALID_HANDLE; 
	SGD::HAudio m_hLoadOptionSwitchSFX = SGD::INVALID_HANDLE; 
	SGD::HAudio m_hLoadOptionSelectionSFX = SGD::INVALID_HANDLE; 

	SaveSlotInfo* save1;
	SaveSlotInfo* save2;
	SaveSlotInfo* save3;

	float arcadebuffer = 0.0f;
};