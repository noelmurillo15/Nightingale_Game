#pragma once

#include "IGameState.h"

#include <vector>

class Kao;
class Player;
class DialogSystem;
class Pawn;
class StaticObject;
class Analyzer;


class HowtoplayState : public IGameState {	

public:

	static HowtoplayState* GetInstance(void);

		// IGameState Interface:
	virtual bool Initialize() override;
	virtual void Terminate() override;
	virtual void Enter() override;
	virtual void Exit() override;
	virtual bool Update(float elapsedTime)	override;
	virtual void Render(float elapsedTime)	override;

	void NextProgress(){ m_progress++; }
	unsigned int GetProgress(){ return m_progress; }

private:
		// SINGLETON!
	HowtoplayState(void) = default;
	virtual ~HowtoplayState(void) = default;
	HowtoplayState(const HowtoplayState&) = delete;
	HowtoplayState& operator= (const HowtoplayState&) = delete;

		//	HTP Variables
	Player* m_Player = nullptr;
	Kao* TheMan = nullptr;
	Pawn* m_Meerkat = nullptr;
	DialogSystem* m_Dialog = nullptr;
	Analyzer* m_Anadex = nullptr;
	std::vector<StaticObject*> m_Bushes;

	enum TutorialProgression{
		T_START,
		T_WASD,
		T_HIDING,
		T_PAWN,
		T_ANALYZING,
		T_BAIT,
		T_REPEL,
		T_SAVING,
		T_LOG,
		T_END
	};

	unsigned int m_progress = T_START;
	bool m_bTutorialScanFinished = false; 
};