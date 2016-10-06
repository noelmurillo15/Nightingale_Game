#pragma once

#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_AudioManager.h"

#include <vector>

using namespace std;
class Entity;
class EntityManager;
#define DISTANCE_CONTROL 350.0f

class Analyzer {

	SGD::Point Cursor;

	bool Ready = false;
	
	bool ifCooled = true;
	bool m_bIsAlternating = false;
	bool m_bScanfailed = false;
	bool m_bInRange = false;
	bool m_bSuccess = false;

	float Analysis = 0;
	float cooldown;
	
	float m_fScanTimer = 0.6f; 
	float m_fFailTimer = 1.0f; 

	bool m_bAlreadyScanned = false; 
	float m_fAlreadyScannedTimer = 2.0f;
	float m_fSuccessTimer = 2.0f;
public:
	Analyzer();
	~Analyzer();

	

	bool inprogress = false;
	int Scanned = 0;
	float BarRenderColor = 0;
	float BarRenderLength = 0;
	float TestFloatbloat = 0;

	SGD::HAudio victoria;
	SGD::HAudio m_hFailure = SGD::INVALID_HANDLE; 

	bool GetReady() { return Ready; }
	void SetReady(bool set){ Ready = set; }
	void SwitchOn() { Ready = 1; }
	void SwitchOff() { Ready = 0; Analysis = 3.0f; }
	void Update(float elapsedTime, EntityManager* EntityList, Entity* Player);
	void Update(float elapsedTime, Entity* creature, Entity* Player);
	void Render();


	bool GetSuccess(){ return m_bSuccess; }

	Entity* Target = nullptr;

	vector<Entity*> CanTarget;
	vector<int> AnalyzedList;

	SGD::HTexture m_hAnalyzerSuccessful = SGD::INVALID_HANDLE; 
	SGD::HTexture m_hCanAnalyze = SGD::INVALID_HANDLE; 
	SGD::HTexture m_hCurrAnalyzing1 = SGD::INVALID_HANDLE; 
	SGD::HTexture m_hCurrAnalyzing2 = SGD::INVALID_HANDLE;
	SGD::HTexture m_hScanFailed = SGD::INVALID_HANDLE;


};

