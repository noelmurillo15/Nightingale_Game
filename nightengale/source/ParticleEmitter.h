#pragma once
//***********************************************************************
//	File:		ParticleEmitter.h
//	Author:		Theodore Kalloo
//	Course:		SGP 1412
//	Purpose:	ParticleEmitter class handles the particle emitters used as
//				the base for the flyweight pattern. Also contains flyweight struct.
//***********************************************************************
#include "Particle.h"

#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_Handle.h"

#include <vector>

using namespace std; 

#define MAX_PARTICLES 2500


struct Flyweight {

public:
	//Accessors
	int GetMinLife() { return m_fMinLife; }
	int GetMaxLife() { return m_fMaxLife; }

	SGD::Vector GetStartVelocity(void) { return m_vcStartingVel; }
	float GetVelocityRateOfChange(void) { return m_vcVelocityRateOfChange; }

	float GetStartingRotation(void) { return m_fStartingRotation; }
	float GetRotationRateOfChange(void) { return m_fRotationRateOfChange; }

	SGD::Size GetStartingSize(void) { return m_szStartingSize; }
	float GetSizeRateOfChange(void) { return m_szSizeRateOfChange; }

	SGD::Color GetStartingColor(void) { return m_cStartingColor; }
	SGD::Color GetEndColor(void) { return m_cEndColor; }
	float GetAlphaRateOfChange(void) { return m_alphaRateOfChange; }
	float GetRedRateOfChange(void) { return  m_fRedRateOfChange; }
	float GetGreenRateOfChange(void) { return m_fGreenRateOfChange; }
	float GetBlueRateOfChange(void) { return  m_fBlueRateOfChange; }

	SGD::Point GetStartLocation() {return m_ptStartLocation;}

	//Mutators
	void SetMinLife(int _min) { m_fMinLife = _min; } 
	void SetMaxLife(int _max) { m_fMaxLife = _max; }

	void SetStartVelocity(SGD::Vector _startvel) { m_vcStartingVel = _startvel; }
	void SetVelRateOfChange(float _velroc) { m_vcVelocityRateOfChange = _velroc; }

	void SetStartingRotation(float _startrot) { m_fStartingRotation = _startrot; }
	void SetRotationRateOfChange(float _rotroc) { m_fRotationRateOfChange = _rotroc; }

	void SetStartingSize(SGD::Size _startsize) { m_szStartingSize = _startsize; }
	void SetSizeRateOfChange(float _sizeroc) { m_szSizeRateOfChange = _sizeroc; }

	void SetStartingColor(SGD::Color _colorstart) { m_cStartingColor = _colorstart; }
	void SetEndingColor(SGD::Color _colorend) { m_cEndColor = _colorend;  }
	void SetAlphaRateOfChange(float _aroc) { m_alphaRateOfChange = _aroc; }
	void SetRedRateOfChange(float _rroc) { m_fRedRateOfChange = _rroc; }
	void SetGreenRateOFChange(float _groc) { m_fGreenRateOfChange = _groc; }
	void SetBlueRateOfChange(float _broc) { m_fBlueRateOfChange = _broc; }

	SGD::HTexture m_tParticleImage = SGD::INVALID_HANDLE;

	void SetStartLocation(SGD::Point _point) {m_ptStartLocation = _point; }

private:
	int m_fMinLife;
	int m_fMaxLife;

	SGD::Vector m_vcStartingVel;
	float m_vcVelocityRateOfChange;

	float m_fStartingRotation;
	float m_fRotationRateOfChange;

	SGD::Size m_szStartingSize;
	float m_szSizeRateOfChange;

	SGD::Color m_cStartingColor;
	SGD::Color m_cEndColor; 
	float m_alphaRateOfChange;
	float m_fRedRateOfChange;
	float m_fGreenRateOfChange;
	float m_fBlueRateOfChange;

	SGD::Point m_ptStartLocation; 


};

class ParticleEmitter
{
public:
	ParticleEmitter();
	~ParticleEmitter();

	//Update
	void Update(float dt, SGD::Point offset);

	//Render
	void Render(float dt, SGD::Point offset); 

	//Render particles while a level runs
	void RenderSaveParticles(float dt, SGD::Point offset); 

	//Accessors
	bool GetIsActive(void) { return m_bIsActive; }
	bool GetIsLooping(void) { return m_bIsLooping; }
	SGD::Size GetEmitterSize(void) { return m_szEmitterSize; }
	int GetMaxParticleNumber(void) { return m_iMaxParticleNum; }
	Flyweight* GetFlyWeights(void) { return &m_fwFlyWeights; }
	float GetEmitterLifetime(void) { return m_fEmitterLifetime; }
	float GetParticleSpawnRate(void) { return m_fParticleSpawnRate; }
	SGD::Point GetEmitterPos(void){ return m_ptEmitterPos; }
	float GetCurrentSpawnTimer(void) { return m_fCurrentSpawnTimer; }
	int GetEmitterShape(void) { return m_iEmitterShape; }
	float GetEmitterWidth(void ) { return m_fWidth; }
	float GetEmitterHeight(void) { return m_fHeight; }


	//Mutators
	void SetIsActive(bool _active) { m_bIsActive = _active; }
	void SetIsLooping(bool _loop) { m_bIsLooping = _loop; }
	void SetEmitterSize(SGD::Size _emitsize) { m_szEmitterSize = _emitsize; }
	void SetMaxParticleNumber(int _partnum) { m_iMaxParticleNum = _partnum; }
	void SetFlyWeights(Flyweight _fw) { m_fwFlyWeights = _fw; }
	void SetEmitterLifetime(float _emitlifetime) { m_fEmitterLifetime = _emitlifetime; }
	void SetParticleSpawnRate(float _spawnrate) { m_fParticleSpawnRate = _spawnrate; }
	void SetEmitterPos(SGD::Point _pos) { m_ptEmitterPos = _pos; }
	void SetEmitterPos(SGD::Rectangle rekt);

	void SetCurrentSpawnTimer(float _time) { m_fCurrentSpawnTimer = _time;  }
	vector<Particle*> activeParticles;

	void ClearParticleVectors(void);
	Particle* SpawnParticle(void);
	void SetEmitterShape(int _shape) { m_iEmitterShape = _shape; }
	void SetEmitterWidth(float _width) { m_fWidth = _width;  }
	void SetEmitterHeight(float _height) { m_fHeight = _height;  }

	


private: 
	bool m_bIsActive; 
	bool m_bIsLooping; 
	SGD::Size m_szEmitterSize; 
	int m_iMaxParticleNum; 
	Flyweight m_fwFlyWeights; 
	float m_fEmitterLifetime; 
	float m_fParticleSpawnRate; 
	float m_fCurrentSpawnTimer; 
	SGD::Point m_ptEmitterPos;
	int m_iEmitterShape; 
	float m_fWidth, m_fHeight; 
};

