#pragma once

#include "Entity.h"
#include <map>

class Items;
class Layer;
class AnimationTimestamp;
class HUD;


#define SMELL_CONTROL 0.5f


class Player : public Entity {

	std::map<unsigned int, std::vector<std::string>> m_StringTable;		

	bool m_canSave = false;
	bool m_levelTransition = false;
	bool m_succesfultransition = false;	
	bool Exhausted = false;
	bool m_bCanHarvestBait = false; 
	bool m_bCanHarvestRepellent = false; 
	bool m_NighttimeDeath = false;

public:
	vector<StankNode*> m_ListSmellTrail;
	Player();
	Player(std::string charName);
	~Player();

		//	Interface
	void Update(float elapsedtime) override;
	void Render(SGD::Point) override;	

		//	Dialog
	void LoadDialog();
	void SendDialog(unsigned int DialogType);

	void Input();

		//	Updates
	void Trans();	
	void UpdateMoveSpeed();
	void MoveSpeedManip(float elapsedTime);
	void PreformItemBehavaior(const IEntity*) override;

		//	Collisions
	void HandleCollision(const IEntity* pOther) override;	
	bool EventHandleCollision(std::vector<SGD::Rectangle>, SGD::Point, bool poison);
	
		//	Overrides
	int GetType() const override { return ENT_PLAYER; }
	
		// Accessors:
	float	GetStamina()					{ return m_Stamina; }
	bool	GetCanSave()					{ return m_canSave; }
	bool	GetLevelTransition()			{ return m_levelTransition; }
	int		GetNumBaits(void)				{ return m_Baits; }
	int		GetNumRepellents(void)			{ return m_Repellents; }
	int		GetNumSaves(void)				{ return m_NumSaves; }
	bool	GetCanHarvestBait(void)			{ return m_bCanHarvestBait;  }
	bool	GetCanHarvestRepellent(void)	{ return m_bCanHarvestRepellent;  }
	bool	GetNightimeDeath()				{ return m_NighttimeDeath; }

		// Mutators:
	void SetStamina(float s)			{ m_Stamina = s; };
	void SetSize(SGD::Size size)		{ m_szSize = size; }	
	void SetCanSave(bool boo)			{ m_canSave = boo; }
	void SetLevelTransition(bool boo)	{ m_levelTransition = boo; }
	void SetNumBaits(int _baits)		{ m_Baits = _baits;} 
	void SetNumRepellents(int _repels)	{ m_Repellents = _repels; }
	void SetCanHarvestBait(bool _can)	{ m_bCanHarvestBait = _can;  }
	void SetCanHarvestRepellent(bool _can) { m_bCanHarvestRepellent = _can;  }

	enum DialogType {
		MSG_BASE,
		MSG_GREETING,
		MSG_ENVIRONMENT
	};

private:

	SGD::HTexture m_nighttime = SGD::INVALID_HANDLE;
	SGD::HTexture m_bHurt = SGD::INVALID_HANDLE;
	SGD::HAudio m_sHurt = SGD::INVALID_HANDLE;
	SGD::HAudio m_sDeath = SGD::INVALID_HANDLE;


	float m_Stamina = 0.0f;
	float hurtOp = 0.0f;

	float m_fSmellTimer = 0.0f;
	float m_ExhaustTimer = 0.0f;
	float m_NighttimeTimer = 0.0f;
	float m_hurtTimer = 0.0f;
	float arcadebuffer = 0.0f;

	int m_Repellents = 0;
	int m_Baits = 0;
	int m_NumSaves = 0;

	HUD* m_Hud = nullptr;
};