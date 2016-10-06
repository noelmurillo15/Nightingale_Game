////***********************************************************************
//	File:		ParticleManager.cpp
//	Author:		Theodore Kalloo
//	Course:		SGP 1412
//	Purpose:	ParticleManager class handles all aspects of the particle
//				system including loading from XML. 
//***********************************************************************

#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "Layer.h"
#include "tinyxml.h"
#include <cstdlib> 
#include "../SGD Wrappers/SGD_GraphicsManager.h"


ParticleManager::ParticleManager()
{
}


ParticleManager::~ParticleManager()
{
	for (unsigned int i = 0; i < testVec.size(); i++)
	{
		testVec[i]->ClearParticleVectors();
		delete testVec[i]; 
	}
	testVec.clear(); 
	testVec.resize(0); 

}

bool ParticleManager::LoadEmitter(const char* _filename)
{
	ParticleEmitter* pe = new ParticleEmitter;

	TiXmlDocument doc;
	if (doc.LoadFile(_filename) == false)
		return false; 

	TiXmlHandle handle(&doc); 

	//Root of the XML document
	TiXmlElement* root = doc.FirstChildElement();//handle.ChildElement(0).Element(); 
	if (nullptr == root)
		return false; 

	//Root's child node
	TiXmlElement* element = root->FirstChildElement();

	//Get the texture
	string textureName;
	textureName = element->Attribute("Name");

	//Load the texture
	Flyweight* tempFW = new Flyweight;
	textureName = "resource/graphics/" + textureName; 
	tempFW->m_tParticleImage = SGD::GraphicsManager::GetInstance()->LoadTexture(textureName.c_str());
	
	
	element = element->NextSiblingElement("Emit_values");

	int maxNumParts, emitShape, emitX, emitY, emitWidth, 
		emitHeight, emitLifeSpan,emitLooping;

	double emitSpawnRate; 
	//string ; 

	//Get the emitter properties
	element->ToElement()->Attribute("maxnumparticles", &maxNumParts); 
	element->ToElement()->Attribute("shape", &emitShape); 
	element->ToElement()->Attribute("emitx", &emitX);
	element->ToElement()->Attribute("emity", &emitY); 
	element->ToElement()->Attribute("emitwidth", &emitWidth); 
	element->ToElement()->Attribute("emitheight", &emitHeight);
	element->ToElement()->Attribute("emitlifespan", &emitLifeSpan);
	element->ToElement()->Attribute("emitlooping", &emitLooping);
	element->ToElement()->Attribute("emitspawnrate", &emitSpawnRate);

	//Set the emitter properties
	pe->SetMaxParticleNumber(maxNumParts); 
	pe->SetEmitterShape(emitShape); 
	pe->SetEmitterPos(SGD::Point((float)emitX, (float)emitY));
	pe->SetEmitterWidth((float)emitWidth); 
	pe->SetEmitterHeight((float)emitHeight); 
	pe->SetEmitterSize(SGD::Size((float)emitWidth, (float)emitHeight)); 
	pe->SetEmitterLifetime((float)emitLifeSpan);
	//pe->SetIsLooping((string)emitLooping);
	pe->SetParticleSpawnRate((float)emitSpawnRate); 

	element = element->NextSiblingElement("particle_values");

	int maxLifeSpan, minLifeSpan, startRotation, RotationRateOfChange, startVelX,
	startVelY, changeVel, width, height, sizeROC;

	//Get the flyweight properties
	element->ToElement()->Attribute("maxlifespan", &maxLifeSpan);
	element->ToElement()->Attribute("minlifespan", &minLifeSpan);
	element->ToElement()->Attribute("startrotation", &startRotation);
	element->ToElement()->Attribute("rotationrateofchange", &RotationRateOfChange);
	element->ToElement()->Attribute("startvelx", &startVelX);
	element->ToElement()->Attribute("startvely", &startVelY);
	element->ToElement()->Attribute("changevel", &changeVel);
	element->ToElement()->Attribute("width", &width);
	element->ToElement()->Attribute("height", &height);
	element->ToElement()->Attribute("sizerateofchange", &sizeROC);

	//Set the flyweight properties
	tempFW->SetMaxLife(maxLifeSpan); 
	tempFW->SetMinLife(minLifeSpan); 
	tempFW->SetStartingRotation((float)startRotation); 
	tempFW->SetRotationRateOfChange((float)RotationRateOfChange); 
	tempFW->SetStartVelocity(SGD::Vector((float)startVelX, (float)startVelY));
	tempFW->SetVelRateOfChange((float)changeVel); 
	tempFW->SetStartingSize(SGD::Size((float)width, (float)height));
	tempFW->SetSizeRateOfChange((float)sizeROC);
	element = element->NextSiblingElement("particle_color_values");

	int startAlpha, startRed, startGreen, startBlue, endAlpha, endRed, endGreen, endBlue; 

	//Get the color values
	element->ToElement()->Attribute("startalpha", &startAlpha);
	element->ToElement()->Attribute("startred", &startRed);
	element->ToElement()->Attribute("startgreen", &startGreen);
	element->ToElement()->Attribute("startblue", &startBlue);
	element->ToElement()->Attribute("endalpha", &endAlpha);
	element->ToElement()->Attribute("endred", &endRed);
	element->ToElement()->Attribute("endgreen", &endGreen);
	element->ToElement()->Attribute("endblue", &endBlue);

	//Set the color values
	tempFW->SetStartingColor(SGD::Color((unsigned char)startAlpha, (unsigned char)startRed, (unsigned char)startGreen, (unsigned char)startBlue));
	tempFW->SetEndingColor(SGD::Color((unsigned char)endAlpha, (unsigned char)endRed, (unsigned char)endGreen, (unsigned char)endBlue));
	
	pe->SetFlyWeights(*tempFW);
	pe->SetIsActive(true);
	testVec.push_back(pe); 
	delete tempFW;
	return true; 
}

void ParticleManager::RenderEmitters(float dt, SGD::Point offset)
{
	if (testVec.size() > 0)
	{
		for (unsigned int i = 0; i < testVec.size(); i++)
			testVec[i]->Render(dt, offset);
	}
}

void ParticleManager::UpdateEmitters(float dt, SGD::Point offset)
{
	if (testVec.size() > 0)
	{
		for (unsigned int i = 0; i < testVec.size(); i++)
			testVec[i]->Update(dt,offset);
	}
}

bool ParticleManager::DisableEmitters(int _sub)
{
	if (true == testVec[_sub]->GetIsActive())
	{
		testVec[_sub]->SetIsActive(false); 
		return true;
	}
	else
		return false; 
}

bool ParticleManager::EnableEmitters(int _sub)
{
	if (testVec.size() > 0)
	{
		if (false == testVec[_sub]->GetIsActive())
		{
			testVec[_sub]->SetIsActive(true);
			return true;
		}
		else
			return false;
	}
	else
		return false; 
}

ParticleEmitter* ParticleManager::CreateEmitter(SGD::Point pos)
{
	ParticleEmitter* tempEmit = new ParticleEmitter;
	tempEmit->SetIsLooping(true); 
	tempEmit->SetIsActive(true); 
	tempEmit->SetEmitterPos(pos);
	//TODO something something emitter shape
	testVec.push_back(tempEmit);
	return tempEmit; 
}

ParticleEmitter* ParticleManager::CreateItemEmitter(SGD::Point pos, float spawnrate, float lifeSpan, SGD::Color _color)
{
	ParticleEmitter* tempEmit = new ParticleEmitter; 

	tempEmit->SetMaxParticleNumber(300); 
	tempEmit->SetIsLooping(false);
	tempEmit->SetEmitterLifetime(lifeSpan); 
	tempEmit->SetEmitterHeight(1.0f); 
	tempEmit->SetParticleSpawnRate(spawnrate); 
	tempEmit->SetEmitterWidth(1.0f); 
	tempEmit->SetEmitterSize(SGD::Size(tempEmit->GetEmitterWidth(), tempEmit->GetEmitterHeight())); 
	tempEmit->GetFlyWeights()->SetMaxLife(3); 
	tempEmit->GetFlyWeights()->SetMinLife(1); 
	tempEmit->GetFlyWeights()->SetStartVelocity(SGD::Vector(0, -100)); 
	//tempEmit->GetFlyWeights()->SetEndingColor(SGD::Color(255, 0, 0)); 
	tempEmit->GetFlyWeights()->SetVelRateOfChange(1.0f); 
	tempEmit->GetFlyWeights()->SetStartingSize(SGD::Size(5, 5)); 
	tempEmit->GetFlyWeights()->m_tParticleImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/ParticleImage.png"); 
//	SGD::Point randPoint = SGD::Point(static_cast<float>(rand() % static_cast<int>(pos.x + pos.x + 200)), static_cast<float>(rand() % static_cast<int>(pos.y + pos.y + 200)));
	tempEmit->GetFlyWeights()->SetStartLocation(pos);
	//For single color particles. Hardcoded for items
	tempEmit->GetFlyWeights()->SetStartingColor(_color); 
	tempEmit->GetFlyWeights()->SetEndingColor(_color); 
	tempEmit->SetIsActive(true);
	tempEmit->SetEmitterPos(pos);
	//TODO something something emitter shape
	testVec.push_back(tempEmit);
	return tempEmit;
}

ParticleEmitter* ParticleManager::CreateSaveEmitter(SGD::Point pos, float spawnrate, float lifeSpan, SGD::Color _color)
{
	ParticleEmitter* tempEmit = new ParticleEmitter;

	tempEmit->SetMaxParticleNumber(300);
	tempEmit->SetIsLooping(true);
	tempEmit->SetEmitterLifetime(lifeSpan);
	tempEmit->SetEmitterHeight(1.0f);
	tempEmit->SetParticleSpawnRate(spawnrate);
	tempEmit->SetEmitterWidth(1.0f);
	tempEmit->SetEmitterSize(SGD::Size(tempEmit->GetEmitterWidth(), tempEmit->GetEmitterHeight()));
	tempEmit->GetFlyWeights()->SetMaxLife(2);
   tempEmit->GetFlyWeights()->SetMinLife(1);
	tempEmit->GetFlyWeights()->SetStartVelocity(SGD::Vector(0, -10));
	//tempEmit->GetFlyWeights()->SetEndingColor(SGD::Color(255, 0, 0)); 
	tempEmit->GetFlyWeights()->SetVelRateOfChange(1.0f);
	tempEmit->GetFlyWeights()->SetStartingSize(SGD::Size(5, 5));
	tempEmit->GetFlyWeights()->m_tParticleImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/Star.png");
	//	SGD::Point randPoint = SGD::Point(static_cast<float>(rand() % static_cast<int>(pos.x + pos.x + 200)), static_cast<float>(rand() % static_cast<int>(pos.y + pos.y + 200)));
	tempEmit->GetFlyWeights()->SetStartLocation(pos);
	//For single color particles. Hardcoded for items
	tempEmit->GetFlyWeights()->SetStartingColor(_color);
	tempEmit->GetFlyWeights()->SetEndingColor(_color);
	tempEmit->SetIsActive(true);
	tempEmit->SetEmitterPos(pos);
	//TODO something something emitter shape
	testVec.push_back(tempEmit);
	return tempEmit;
}

ParticleEmitter* ParticleManager::CreatePoisonEmitter(SGD::Point pos, float spawnrate, float lifeSpan, SGD::Color _color) {
	ParticleEmitter* tempEmit = new ParticleEmitter;

	tempEmit->SetMaxParticleNumber(100);
	tempEmit->SetIsLooping(true);
	tempEmit->SetEmitterLifetime(lifeSpan);
	tempEmit->SetEmitterHeight(1.0f);
	tempEmit->SetParticleSpawnRate(spawnrate);
	tempEmit->SetEmitterWidth(1.0f);
	tempEmit->SetEmitterSize(SGD::Size(tempEmit->GetEmitterWidth(), tempEmit->GetEmitterHeight()));
	tempEmit->GetFlyWeights()->SetMaxLife(8);
	tempEmit->GetFlyWeights()->SetMinLife(5);
	tempEmit->GetFlyWeights()->SetStartVelocity(SGD::Vector(0, -1));
	tempEmit->GetFlyWeights()->SetVelRateOfChange(1.0f);
	tempEmit->GetFlyWeights()->SetStartingSize(SGD::Size(5, 5));
	tempEmit->GetFlyWeights()->m_tParticleImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/Poison.png");
	tempEmit->GetFlyWeights()->SetStartLocation(pos);
	tempEmit->GetFlyWeights()->SetStartingColor(_color);
	tempEmit->GetFlyWeights()->SetEndingColor(_color);
	tempEmit->SetIsActive(true);
	tempEmit->SetEmitterPos(pos);
	testVec.push_back(tempEmit);
	return tempEmit;
}

ParticleEmitter* ParticleManager::CreateEmitter(SGD::Rectangle rekt)
{
	ParticleEmitter* tempEmit = new ParticleEmitter;
	tempEmit->SetIsLooping(false);
	tempEmit->SetIsActive(true);
	SGD::Point tempPoint(rekt.left, rekt.top);
	tempEmit->SetEmitterPos(tempPoint);
	testVec.push_back(tempEmit);
	return tempEmit;
}

ParticleEmitter* ParticleManager::CreateEmitter(SGD::Point pos, SGD::Color color)
{
	ParticleEmitter* tempEmit = new ParticleEmitter;
	tempEmit->SetEmitterPos(pos); 
	tempEmit->SetIsActive(true); 
	tempEmit->SetIsLooping(true);
	testVec.push_back(tempEmit); 
	return tempEmit; 
}

//ParticleEmitter* ParticleManager::CreateEmitter(SGD::Point pos, SGD::String filename)
//{
//	ParticleEmitter* temp;// = new ParticleEmitter;
//	//temp->
//	return temp; 
//}

bool ParticleManager::SaveEmitterXML()
{
	TiXmlDocument doc; 
	TiXmlElement* root = new TiXmlElement("ParticleManager_info"); 

	//TiXmlElement* element = new TiXmlElement("Texture_info"); 
	TiXmlElement* data; 

	const char* textureName = "particle.bmp"; 
	data = new TiXmlElement("Texture_Name"); 
	data->SetAttribute("Name", textureName); 
	//element->LinkEndChild(data);
	root->LinkEndChild(data);

	data = new TiXmlElement("Emit_values"); 
	data->SetAttribute("maxnumparticles", 100); 
	data->SetAttribute("shape", 1); 
	data->SetAttribute("emitx", 100);
	data->SetAttribute("emity", 100);
	data->SetAttribute("emitwidth", 100);
	data->SetAttribute("emitheight", 50);
	data->SetAttribute("emitlifespan", 100);
	data->SetAttribute("emitlooping", 0);
	data->SetAttribute("emitspawnrate", 3);
	root->LinkEndChild(data);

	data = new TiXmlElement("particle_values");
	data->SetAttribute("maxlifespan", 10);
	data->SetAttribute("minlifespan", 3);
	data->SetAttribute("startrotation", 1);
	data->SetAttribute("rotationrateofchange", 1);
	data->SetAttribute("startvelx", 0);
	data->SetAttribute("startvely", 10);
	data->SetAttribute("changevel", 0);
	root->LinkEndChild(data);

	data = new TiXmlElement("particle_color_values");
	data->SetAttribute("startalpha", 255);
	data->SetAttribute("startred", 255);
	data->SetAttribute("startgreen", 255);
	data->SetAttribute("startblue", 255);
	data->SetAttribute("endalpha", 255);
	data->SetAttribute("endred", 0);
	data->SetAttribute("endgreen", 0);
	data->SetAttribute("endblue", 0);
	root->LinkEndChild(data);
	doc.LinkEndChild(root);
	doc.SaveFile("TestParticle2.xml");
	return true; 
}