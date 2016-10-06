#pragma once

#include "IGameState.h"

class CreatureFactory;
class Entity;
class Player;


class CharacterSelect :public IGameState {

	bool CharacterInstantiated = false;

public:
		//	Singleton:
	static CharacterSelect* GetInstance(void);
	
		//	Interface:
	virtual bool	Initialize(void)  override;
	virtual void	Terminate(void)	 override;
	virtual void	Enter(void)				override;	// load resources
	virtual void	Exit(void)				override;	// unload resources
	virtual bool	Update(float elapsedTime)	override;	// handle input & update entites
	virtual void	Render(float elapsedTime)	override;	// render entities / menu

	CreatureFactory* GetFactory(void){ return PlayerMaker; }

	void ResetCharacterInstantiation(){ CharacterInstantiated = false; }
	void CharacterIsInstantiated(){ CharacterInstantiated = true; }
	bool GetIsCharacterInstantiated(){ return CharacterInstantiated; }
	
	bool Input();

	bool AshLife = false;
	bool BrockLife = false;
	bool JennyLife = false;
	bool ToddLife = true;

private:
		// SINGLETON!
	CharacterSelect(void) = default;
	virtual ~CharacterSelect(void) = default;
	CharacterSelect(const CharacterSelect&) = delete;
	CharacterSelect& operator= (const CharacterSelect&) = delete;

		//	Char Select Variables
	int m_selection = 0;
	int _previous = 0;
	
	CreatureFactory* PlayerMaker = nullptr;

		//	Textures
	SGD::HTexture m_Ash = SGD::INVALID_HANDLE;
	SGD::HTexture m_Brock = SGD::INVALID_HANDLE;
	SGD::HTexture m_Jenny = SGD::INVALID_HANDLE;
	SGD::HTexture m_Todd = SGD::INVALID_HANDLE;

		//	Audio
	SGD::HAudio m_hCharacterSwitch = SGD::INVALID_HANDLE; 
	SGD::HAudio m_hOptionSelect = SGD::INVALID_HANDLE; 
	SGD::HAudio m_hCharacterSelectBGM = SGD::INVALID_HANDLE; 

		//	Enum
	enum CharacterSelected{ 
		ENT_PROTAG,
		ENT_WILDCARD,
		ENT_POPO,
		ENT_BONES,
		ENT_QUIT 
	};

	bool m_bGameOver = false;
	float arcadebuffer = 0.0f;
};