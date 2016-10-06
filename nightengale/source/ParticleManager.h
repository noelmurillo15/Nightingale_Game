#pragma once

//***********************************************************************
//	File:		ParticleManager.h
//	Author:		Theodore Kalloo
//	Course:		SGP 1412
//	Purpose:	ParticleManager class handles all aspects of the particle
//				system including loading from XML. 
//***********************************************************************
using namespace std; 

#include "Particle.h"
#include "ParticleEmitter.h"

#include "../SGD Wrappers/SGD_String.h"

//Need to include the tinyXML info


class ParticleManager
{
public:
	ParticleManager();
	~ParticleManager();

	bool LoadEmitter(const char* _filename);
	void RenderEmitters(float dt, SGD::Point offset); 
	void UpdateEmitters(float dt, SGD::Point offset);
	bool DisableEmitters(int _sub); 
	bool EnableEmitters(int _sub); 
	bool SaveEmitterXML(void); 

	ParticleEmitter* CreateEmitter(SGD::Point pos); 
	ParticleEmitter* CreateEmitter(SGD::Rectangle rekt);
	ParticleEmitter* CreateEmitter(SGD::Point pos, SGD::Color color);
	ParticleEmitter* CreateItemEmitter(SGD::Point pos, float spawnrate, float lifeSpan, SGD::Color _color);
	//ParticleEmitter* CreateEmitter(SGD::Point pos, SGD::String filename);
	ParticleEmitter* CreateSaveEmitter(SGD::Point pos, float spawnrate, float lifeSpan, SGD::Color _color); 
	ParticleEmitter* CreatePoisonEmitter(SGD::Point pos, float spawnrate, float lifeSpan, SGD::Color _color);

	vector<ParticleEmitter*> testVec;

private: 
	//std::map<SGD::String, ParticleEmitter> testMap;   //<SGD::String filename, ParticleEmitter pe> testEmitter; 
	SGD::String m_strFilename; 

};

