#pragma once

#include "EntityManager.h"
#include "Analyzer.h"

#define  SEARCH_TIMER 40
#define PERCIEVED_TIMER 4.5f


class DirectorAI{

public:
	DirectorAI() ;
	~DirectorAI();

	void Update(float elapsedTime);

	EntityManager* m_pLiasion = nullptr;

	void CleanEntityMemory(void);

private:

};
