#pragma once
//***********************************************************************
//	File:		Particle.h
//	Author:		Theodore Kalloo
//	Course:		SGP 1412
//	Purpose:	Particle class handles the particle objects' unique values
//***********************************************************************
#include "../SGD Wrappers/SGD_Color.h"
#include "../SGD Wrappers/SGD_Geometry.h"

class Particle
{
public:
	Particle();
	~Particle();

	//Accessors
	float GetCurrLife(void) { return m_fCurrLife; }
	SGD::Color GetCurrColor() { return m_cCurrColor;  }
	SGD::Size GetCurrSize(void) { return m_sCurrSize;  }
	float GetCurrRotation(void) { return m_fCurrRotation; }
	SGD::Vector GetCurrVelocity(void) { return m_vcCurrVelocity; }
	SGD::Point GetCurrPosition(void) { return m_ptCurrPosition; } 
	bool GetIsDead(void) { return m_bIsDead; }
	SGD::Point GetCurrMidPoint(void) { return m_ptCurrMidPoint; }
	SGD::Vector GetMidVector(void) { return m_vcMidVector;  }

	//Mutators 
	void SetCurrLife(float _life) {  m_fCurrLife = _life; }
	void SetCurrColor(SGD::Color _color) { m_cCurrColor = _color; }
	void SetCurrSize(SGD::Size _size) {  m_sCurrSize = _size; }
	void SetCurrRotation(float _rotation) { m_fCurrRotation = _rotation; }
	void SetCurrVelocity(SGD::Vector _vel) {  m_vcCurrVelocity = _vel; }
	void SetCurrPosition(SGD::Point _pos) { m_ptCurrPosition = _pos; }
	void SetIsDead(bool _dead) { m_bIsDead = _dead;  }
	void SetCurrMidPoint(SGD::Point _newMid) { m_ptCurrMidPoint = _newMid; }
	void SetMidVector(SGD::Vector _mv) { m_vcMidVector = _mv; }


private: 
	float m_fCurrLife; 
	SGD::Color m_cCurrColor; 
	SGD::Size m_sCurrSize; 
	float m_fCurrRotation; 
	SGD::Vector m_vcCurrVelocity; 
	SGD::Point m_ptCurrPosition; 

	SGD::Point m_ptCurrMidPoint;

	SGD::Vector m_vcMidVector;
	bool m_bIsDead; 
};

