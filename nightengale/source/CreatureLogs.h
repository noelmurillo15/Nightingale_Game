#pragma once
#include <atlstr.h>
#include "../SGD Wrappers/SGD_Handle.h"
class CreatureLogs {

public:
	CreatureLogs();
	~CreatureLogs();

	void Update(float elapsedTime);
	void Render(float elapsedTime); 

	void ScanCreature(int _creatureID); 

	bool CheckLog(int _ID);

	CString m_sLogEntry;

	bool m_bShowingLog; 

	bool IsScanned(int _creatureID); 
	bool GetLog(int _ID);

	int GetNumAnalyzed(void); 

private: 
	float m_fHardcodedTimer; 
	bool m_aScannedCreatures[10];
	int m_iCursor = 1; 

	SGD::HTexture m_hKnight = SGD::INVALID_HANDLE;
	SGD::HTexture m_hKing = SGD::INVALID_HANDLE;
	SGD::HTexture m_hQueen = SGD::INVALID_HANDLE;
	SGD::HTexture m_hPawn = SGD::INVALID_HANDLE;
	SGD::HTexture m_hPrancer = SGD::INVALID_HANDLE;
	SGD::HTexture m_hBishop = SGD::INVALID_HANDLE;
	SGD::HTexture m_hRook = SGD::INVALID_HANDLE;
	SGD::HTexture m_hCastle = SGD::INVALID_HANDLE;
	SGD::HTexture m_hPassant = SGD::INVALID_HANDLE;
	SGD::HTexture m_hBumbles = SGD::INVALID_HANDLE;
	SGD::HTexture m_hHUDFrame = SGD::INVALID_HANDLE;
	SGD::HTexture* m_hCurrImg = nullptr; 
	float m_fArcadeBuffer = 0.2f; 
};

