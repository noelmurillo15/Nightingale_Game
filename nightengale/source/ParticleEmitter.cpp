//***********************************************************************
//	File:		ParticleEmitter.cpp
//	Author:		Theodore Kalloo
//	Course:		SGP 1412
//	Purpose:	ParticleEmitter class handles the particle emitters used as
//				the base for the flyweight pattern. Also contains flyweight struct.
//***********************************************************************
#include "ParticleEmitter.h"
#include "Camera.h"
#include <cassert>

#include "../SGD Wrappers/SGD_GraphicsManager.h"

//TESTING 123
ParticleEmitter::ParticleEmitter()
{
	//this->SetCurrentSpawnTimer(0.0f); 
	//this->m_iMaxParticleNum = MAX_PARTICLES; 
	//this->SetEmitterLifetime(75.0f);
	//this->SetEmitterSize(SGD::Size(15.0f, 15.0f)); 
	//this->SetIsActive(true); 
	//this->SetIsLooping(true); 
	//this->SetParticleSpawnRate(0.35f); 
	//this->SetEmitterPos(SGD::Point(1000.0, 1000.0)); 
	//m_fwFlyWeights.m_tParticleImage = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/particle.bmp", SGD::Color{ 0, 0, 0 });
	
	//Set up the flyweight with the loaded in info 

//Test Flyweight Info
#pragma region TestFlyweight

	////Position
	//m_fwFlyWeights.SetStartLocation(SGD::Point(175 + m_fwFlyWeights.GetStartingSize().width, 175 + m_fwFlyWeights.GetStartingSize().height));
	//
	////Size
	//m_fwFlyWeights.SetStartingSize(SGD::Size(6.0f, 6.0f));
	//m_fwFlyWeights.SetSizeRateOfChange(-0.9f);
	//
	////Colors
	//m_fwFlyWeights.SetStartingColor(SGD::Color(255,255, 255, 255)); 
	//m_fwFlyWeights.SetAlphaRateOfChange(1.0f);
	//m_fwFlyWeights.SetRedRateOfChange(-2.1f); 
	//m_fwFlyWeights.SetGreenRateOFChange(-2.1f);
	//m_fwFlyWeights.SetBlueRateOfChange(-2.1f);
	//
	////LifeSpan
	//m_fwFlyWeights.SetMinLife(1); 
	//m_fwFlyWeights.SetMaxLife(2); 
	//
	////Velocity
	//m_fwFlyWeights.SetStartVelocity(SGD::Vector(0.0f, -100.55f)); 
	//m_fwFlyWeights.SetVelRateOfChange(.999f); 
	//
	////Rotation
	//m_fwFlyWeights.SetStartingRotation(1.0f); 
	//m_fwFlyWeights.SetRotationRateOfChange(5.0f); 

#pragma endregion
}


ParticleEmitter::~ParticleEmitter()
{
	ClearParticleVectors(); 
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_fwFlyWeights.m_tParticleImage);
	m_fwFlyWeights.m_tParticleImage = SGD::INVALID_HANDLE; 
}

//Update
void ParticleEmitter::Update(float dt, SGD::Point offset)
{
	if (true == m_bIsActive)//Don't update if the emitter is not active
	{
		m_fEmitterLifetime -= dt; 

		int m_iDeadParticleCount = 0; //Counter for the amount of dead particles
		for (unsigned int i = 0; i < activeParticles.size(); i++) //Iterate through all of the active particles
		{
			if (true == activeParticles[i]->GetIsDead())//If this particle is dead
				m_iDeadParticleCount++; //Increase the dead particle counter
		}
		if (m_iDeadParticleCount == activeParticles.size() && m_iDeadParticleCount >= 1 || this->GetEmitterLifetime() <= 0) //If all particles are dead in the vector 
		{
			//if looping is true, bring back to life and reset parameters
			if (true == this->GetIsLooping())
			{
				int temp = activeParticles.size();
				ClearParticleVectors();
				this->SetCurrentSpawnTimer(0.0f);
				for (int i = 0; i < temp; i++)
				{
					if (m_fCurrentSpawnTimer >= m_fParticleSpawnRate && activeParticles.size() < (unsigned int)m_iMaxParticleNum)//Spawn a new particle if under max number
					{
						Particle* pa = SpawnParticle();
						activeParticles.push_back(pa);
						this->SetCurrentSpawnTimer(0.0f);
					}
				}
			}
			else //Else, remove from the vector 
			{
				ClearParticleVectors();
				m_bIsActive = false; 
			}
		}
	}
	if (true == m_bIsActive)//Don't update if the emitter is not active
	{
		this->SetCurrentSpawnTimer(this->GetCurrentSpawnTimer() + (dt) ); //Increment the spawn timer by the elapsed time
		if (m_fCurrentSpawnTimer >= m_fParticleSpawnRate && activeParticles.size() < (unsigned int)m_iMaxParticleNum)//Spawn a new particle if under max number of particles
		{
			Particle* pa = SpawnParticle(); 
			SGD::Point randPoint = SGD::Point(static_cast<float>(rand() % 100) + this->GetEmitterPos().x - 100,
				static_cast<float>(rand() % 100) + this->GetEmitterPos().y - 100);
			pa->SetCurrColor(this->GetFlyWeights()->GetStartingColor()); 
			pa->SetCurrPosition(randPoint); 
			activeParticles.push_back(pa);
			this->SetCurrentSpawnTimer(0.0f);
		}

		for (unsigned int i = 0; i < activeParticles.size(); i++) //For all particles in the emitter vector 
		{
			//Check if the particle is still alive
			if (activeParticles[i]->GetCurrLife() <= 0)
			{
				activeParticles[i]->SetIsDead(true);
			}

			//If all particles are alive: 
			//Updating the color
			activeParticles[i]->SetCurrColor(SGD::Color(activeParticles[i]->GetCurrColor().alpha + (unsigned char)(m_fwFlyWeights.GetAlphaRateOfChange()* dt),
				activeParticles[i]->GetCurrColor().red + (unsigned char)(m_fwFlyWeights.GetRedRateOfChange() * dt),
				activeParticles[i]->GetCurrColor().green + (unsigned char)(m_fwFlyWeights.GetGreenRateOfChange()* dt),
				activeParticles[i]->GetCurrColor().blue + (unsigned char)(m_fwFlyWeights.GetBlueRateOfChange() * dt)));

			//Set the current mid point (+ is going downward, - means going upward) 
			if (activeParticles[i]->GetCurrMidPoint().x > activeParticles[i]->GetCurrPosition().x &&
				activeParticles[i]->GetCurrMidPoint().y > activeParticles[i]->GetCurrPosition().y)//Upper left
			{
				activeParticles[i]->SetCurrMidPoint(SGD::Point(activeParticles[i]->GetCurrPosition().x + (activeParticles[i]->GetCurrSize().width / 2),
					activeParticles[i]->GetCurrPosition().y + (activeParticles[i]->GetCurrSize().height / 2)));
			}
			else if (activeParticles[i]->GetCurrMidPoint().x < activeParticles[i]->GetCurrPosition().x &&
					 activeParticles[i]->GetCurrMidPoint().y > activeParticles[i]->GetCurrPosition().y)//Upper right
			{
				activeParticles[i]->SetCurrMidPoint(SGD::Point(activeParticles[i]->GetCurrPosition().x - (activeParticles[i]->GetCurrSize().width / 2),
					activeParticles[i]->GetCurrPosition().y + (activeParticles[i]->GetCurrSize().height / 2)));
			}
			else if (activeParticles[i]->GetCurrMidPoint().x < activeParticles[i]->GetCurrPosition().x &&
					 activeParticles[i]->GetCurrMidPoint().y < activeParticles[i]->GetCurrPosition().y)//Bottom right
			{
				activeParticles[i]->SetCurrMidPoint(SGD::Point(activeParticles[i]->GetCurrPosition().x - (activeParticles[i]->GetCurrSize().width / 2),
					activeParticles[i]->GetCurrPosition().y - (activeParticles[i]->GetCurrSize().height / 2)));
			}
			else if (activeParticles[i]->GetCurrMidPoint().x > activeParticles[i]->GetCurrPosition().x &&
					 activeParticles[i]->GetCurrMidPoint().y < activeParticles[i]->GetCurrPosition().y)//Bottom left
			{
				activeParticles[i]->SetCurrMidPoint(SGD::Point(activeParticles[i]->GetCurrPosition().x + (activeParticles[i]->GetCurrSize().width / 2),
					activeParticles[i]->GetCurrPosition().y - (activeParticles[i]->GetCurrSize().height / 2)));
			}
			//Set the mid vector for scaling
			activeParticles[i]->SetMidVector(activeParticles[i]->GetCurrPosition() - activeParticles[i]->GetCurrMidPoint());

			//Updating the lifespan
			activeParticles[i]->SetCurrLife(activeParticles[i]->GetCurrLife() - dt);

			//Updating the velocity
			activeParticles[i]->SetCurrVelocity(activeParticles[i]->GetCurrVelocity() * (m_fwFlyWeights.GetVelocityRateOfChange()));

			//Update the size
			activeParticles[i]->SetCurrSize(SGD::Size(activeParticles[i]->GetCurrSize().width + (m_fwFlyWeights.GetSizeRateOfChange() * dt),
				activeParticles[i]->GetCurrSize().height + (m_fwFlyWeights.GetSizeRateOfChange() * dt)));

			//If size is less than zero, just set it to zero
			if (activeParticles[i]->GetCurrSize().width <= 0 || activeParticles[i]->GetCurrSize().height <= 0)
				activeParticles[i]->SetCurrSize(SGD::Size(0, 0));

			//Update the rotatoes
			activeParticles[i]->SetCurrRotation(activeParticles[i]->GetCurrRotation() + (m_fwFlyWeights.GetRotationRateOfChange()* dt));

			//Update the position
			activeParticles[i]->SetCurrPosition(activeParticles[i]->GetCurrPosition() + (activeParticles[i]->GetCurrVelocity() * dt));
		}
	}
}

//Render
void ParticleEmitter::Render(float dt, SGD::Point offset)
{
	if (true == m_bIsActive)
	{
		SGD::GraphicsManager::GetInstance()->DrawRectangle(SGD::Rectangle(this->GetEmitterPos().x - offset.x, this->GetEmitterPos().y - offset.y, 
			(this->GetEmitterPos().x - offset.x) + this->GetEmitterSize().width, (this->GetEmitterPos().y - offset.y) + this->GetEmitterSize().height), SGD::Color());

		for (unsigned int i = 0; i < activeParticles.size(); i++)
		{
			//Only render the particle if it is still alive
			assert(m_fwFlyWeights.m_tParticleImage != SGD::INVALID_HANDLE);
			if (activeParticles[i]->GetCurrLife() > 0.0f)
			{
				SGD::GraphicsManager::GetInstance()->DrawTexture(m_fwFlyWeights.m_tParticleImage, 
					SGD::Point(activeParticles[i]->GetCurrPosition().x - offset.x, activeParticles[i]->GetCurrPosition().y - offset.y),
					activeParticles[i]->GetCurrRotation(), SGD::Vector((activeParticles[i]->GetCurrPosition().x - activeParticles[i]->GetCurrSize().width * 0.1f) - activeParticles[i]->GetCurrMidPoint().x,
					(activeParticles[i]->GetCurrPosition().y - activeParticles[i]->GetCurrSize().width * 0.1f) - activeParticles[i]->GetCurrMidPoint().y)   /*GetMidVector()*/, activeParticles[i]->GetCurrColor(),
					SGD::Size(activeParticles[i]->GetCurrSize().width * 0.1f, activeParticles[i]->GetCurrSize().height * 0.1f));
			}
		}
	}
}

void ParticleEmitter::RenderSaveParticles(float dt, SGD::Point offset)
{
	if (true == m_bIsActive)
	{
		SGD::GraphicsManager::GetInstance()->DrawRectangle(SGD::Rectangle(this->GetEmitterPos().x - offset.x, this->GetEmitterPos().y - offset.y,
			(this->GetEmitterPos().x - offset.x) + this->GetEmitterSize().width, (this->GetEmitterPos().y - offset.y) + this->GetEmitterSize().height), SGD::Color());

		for (unsigned int i = 0; i < activeParticles.size(); i++)
		{
			//Only render the particle if it is still alive
			assert(m_fwFlyWeights.m_tParticleImage != SGD::INVALID_HANDLE);
			if (activeParticles[i]->GetCurrLife() > 0.0f)
			{
				SGD::GraphicsManager::GetInstance()->DrawTexture(m_fwFlyWeights.m_tParticleImage,
					SGD::Point(activeParticles[i]->GetCurrPosition().x - offset.x, activeParticles[i]->GetCurrPosition().y - offset.y),
					activeParticles[i]->GetCurrRotation(), SGD::Vector((activeParticles[i]->GetCurrPosition().x - activeParticles[i]->GetCurrSize().width * 0.1f) - activeParticles[i]->GetCurrMidPoint().x,
					(activeParticles[i]->GetCurrPosition().y - activeParticles[i]->GetCurrSize().width * 0.1f) - activeParticles[i]->GetCurrMidPoint().y)   /*GetMidVector()*/, activeParticles[i]->GetCurrColor(),
					SGD::Size(activeParticles[i]->GetCurrSize().width * 0.1f, activeParticles[i]->GetCurrSize().height * 0.1f));
			}
		}
	}

}

void ParticleEmitter::ClearParticleVectors()
{
	for (unsigned int i = 0; i < activeParticles.size(); i++)
	{
		delete activeParticles[i]; 
	}
	activeParticles.clear(); 
	activeParticles.resize(0); 
}

void ParticleEmitter::SetEmitterPos(SGD::Rectangle rekt)
{
	SGD::Point tempPoint(rekt.left, rekt.top);
	this->m_ptEmitterPos = tempPoint; 
}

Particle* ParticleEmitter::SpawnParticle()
{
	Particle* part = new Particle;
	part->SetCurrColor(SGD::Color(255, 255, 255, 255));
	part->SetCurrLife((float)(rand() % (m_fwFlyWeights.GetMaxLife() - m_fwFlyWeights.GetMinLife()) + m_fwFlyWeights.GetMinLife()));
	part->SetCurrPosition(this->GetEmitterPos());
	part->SetCurrRotation(m_fwFlyWeights.GetStartingRotation());
	part->SetCurrSize(m_fwFlyWeights.GetStartingSize());
	part->SetCurrVelocity(SGD::Vector{ (float)(rand() % 400 - 200), m_fwFlyWeights.GetStartVelocity().y });
	part->SetCurrMidPoint(SGD::Point(part->GetCurrPosition().x - part->GetCurrSize().width / 2,
									 part->GetCurrPosition().y - part->GetCurrSize().height / 2)); 
	part->SetIsDead(false);
	return part; 
}