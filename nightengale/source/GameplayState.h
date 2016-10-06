#pragma once

#include "IGameState.h"	
#include <vector>
#include "../SGD Wrappers/SGD_Message.h"

class Entity;
class EntityManager;
class CreatureFactory;
class DirectorAI;
class ParticleManager;
class Camera;
class Analyzer;
class CreatureLogs; 
class Knight;
class Controls; 
class SaveSlots;
class Player;
class DialogSystem;

using namespace std;

class GameplayState : public IGameState {

	struct Pause{
		bool isPaused = false;
		bool pause = false;
		SGD::Rectangle m_PauseScreen;
		int m_nCursor = 1;
	};

	struct EndGame{
		bool Losing = false;
		bool Winning = false;
		bool GameOver = false;

		void SetWinning(bool boo){ Winning = boo; }
	};

	struct Save{
		bool isPaused = false;
		bool isSaved = false;
		SGD::Rectangle m_saveScreen;
		int m_nCursor = 1;
	};

	Pause m_pause;
	EndGame* m_endgame;	
	Save m_save; 	

	Camera*			 cam = nullptr;
	Analyzer*		 AnaDex = nullptr;
	Controls*		 m_Instructions = nullptr;
	SaveSlots*		 m_SaveSlots = nullptr;
	DirectorAI*		 m_mKnight = nullptr;	
	DialogSystem*	 m_Dialog = nullptr;
	CreatureLogs*	 m_CreatureLog = nullptr;
	EntityManager*	 m_ObjectManager = nullptr;
	CreatureFactory* m_AnimalFactory = nullptr;
	ParticleManager* m_ParticleManager = nullptr;
		
	bool On = false;
	bool m_bInCreatureLog = false; 
	bool m_bInOptionsScreen = false; 
	std::string loadfile = "";
	bool m_bMessagesInit = false;

	float deathtimer = 0.0f;
	float m_fCreatureLogMenuTimer = 0.5f; 
public:
		// Singleton Accessor:
	static GameplayState* GetInstance(void);

		// IGameState Interface:
	virtual bool	Initialize(void) override;
	virtual void	Terminate(void)  override;
	virtual void	Enter(void)				override;	// load resources
	virtual void	Exit(void)				override;	// unload resources
	virtual bool	Update(float elapsedTime)	override;	// handle input & update entities
	virtual void	Render(float elapsedTime)	override;	// render game entities / menus

		//	Normals Functions
	void Input();
	void AddToManager(Entity*);
	void PlayDeathDialogue(std::string person);
	void SetPlayer(Player* NewPlayer){ m_player = NewPlayer; }
	void SetIsInCreatureLog(bool _bool){ m_bInCreatureLog = _bool; }
	void SetIsInControlsScreen(bool _bool){ m_bInOptionsScreen = _bool; }
	void SetLoadFile(std::string name){ loadfile = name; }

	void WorldTransition();

	std::string GetLoadFile(){ return loadfile; }

	bool GetTurnedOn() { return On; }
	bool AnalyzeReady = false;
	bool GetIsInCreatureLog(void) const { return m_bInCreatureLog; }
	bool GetIsInControlsScreen(void) const { return m_bInOptionsScreen; }
	bool GetAlreadyInstantiated(){ return AlreadyInstantiated; }

	std::string CalculateFps(float elapsedTime);

	EndGame* GetEndGame(){ return m_endgame; }
	EntityManager* GetManager(){ return m_ObjectManager; }	
	Player* GetPlayer(void)const { return m_player; }	
	Analyzer* GetAnna() { return AnaDex; }
	CreatureLogs* GetCreatureLogs(void) const { return m_CreatureLog; }
	DialogSystem* GetDialogSystem(){ return m_Dialog; }

private:

		// SINGLETON (not-dynamically allocated)
	GameplayState(void) = default;	// default constructor
	virtual ~GameplayState(void) = default;	// destructor
	GameplayState(const GameplayState&) = delete;	// copy constructor
	GameplayState& operator= (const GameplayState&) = delete;	// assignment operator

	static void MessageProc(const SGD::Message* pMsg);

	SGD::HTexture   _pawn = SGD::INVALID_HANDLE;
	SGD::HTexture   _dirtypawn = SGD::INVALID_HANDLE;
	SGD::HAudio		m_win = SGD::INVALID_HANDLE;
	SGD::HAudio		m_lose = SGD::INVALID_HANDLE;
	SGD::HAudio		m_repellant = SGD::INVALID_HANDLE;
	SGD::HAudio		m_bait = SGD::INVALID_HANDLE;
	SGD::HAudio		m_WindHowlPlain = SGD::INVALID_HANDLE;

	SGD::HAudio m_sSwitch;
	vector<SGD::Rectangle> BumblesTrans;

	Player* m_player = nullptr;

	float NeedToScan = 0;
	float Fade = 0;
	float TimePlayed = 0.0f;

		//FPS
	unsigned int	m_FPS = 60;
	unsigned int	m_Frames = 0;
	float			m_FpsTimer = 0.0f;

	int DeadPlayerCount = 0;
	bool CanWin = false;
	bool TansitionFade = false;
	bool Dead3PeopleDialogueBool = false; 
	bool Dead2PeopleDialogueBool = false; 
	bool Dead1PeopleDialogueBool = false; 

	bool m_bVictory = false; 

	Knight* testfollow;
	std::string DeadPlayer;
	float deadTimer = 0;
	float arcadebuffer = 0.0f;

	bool AlreadyInstantiated = false;
};