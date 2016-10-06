#include "AnimationSystem.h"
#include "..\SGD Wrappers\SGD_GraphicsManager.h"
#include "..\SGD Wrappers\SGD_Geometry.h"
#include "..\SGD Wrappers\SGD_AudioManager.h"

#include "..\SGD Wrappers\SGD_InputManager.h"

#include "Camera.h"
#include "Layer.h"

#include "tinyxml.h"

#include <cassert>

AnimationSystem* AnimationSystem::s_pInstance = nullptr;

AnimationSystem* AnimationSystem::GetInstance(void)
{
	if (s_pInstance == nullptr)
		s_pInstance = new AnimationSystem;
	return s_pInstance;
}

AnimationTimestamp::AnimationTimestamp(AnimationTimestamp* const& obj)
{
	m_SCurrentObject = obj->m_SCurrentObject;
	m_SCurrentAnimation = obj->m_SCurrentAnimation;
	m_sLastAnimation = obj->m_sLastAnimation;
	m_uICurrentFrame = obj->m_uICurrentFrame;
	m_FFrameDuration = obj->m_FFrameDuration;
	m_IdleFrame = obj->m_IdleFrame;

}

void AnimationSystem::DeleteInstance(void)
{
	delete s_pInstance;
	s_pInstance = nullptr;
}

//Creates an Animation timestamp; takes in the object name and starting animation name
AnimationTimestamp::AnimationTimestamp(std::string obj, std::string anim)
{
	m_SCurrentObject = obj;
	m_SCurrentAnimation = anim;
	m_uICurrentFrame = 0;
}

AnimationSystem::~AnimationSystem() {

	//An Object's animations were not unloaded

	{
		std::map<std::string, SGD::HAudio>::iterator iter = m_vASounds.begin();

		for (size_t i = 0; i < m_vASounds.size(); i++)
		{
			SGD::AudioManager::GetInstance()->UnloadAudio(iter->second);
			m_vASounds.erase(iter->first);
			iter = m_vASounds.begin();
		}
	}

	auto liter = m_mOLoaded.begin();
	for (size_t i = 0; liter != m_mOLoaded.end(); i++)
	{
		Unload(liter->first);
		liter = m_mOLoaded.begin();
	}

	assert(m_mOLoaded.size() == 0
		&& "An animation was not unloaded");

}

//Displays current frame of current animation of current object in position parameter, flipped x = 1; flipped y = 2
void AnimationSystem::Render(AnimationTimestamp* Info, float posX, float posY, SGD::Size scale, SGD::Color color)
{
	if (!Info)
		return;
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();


	assert(m_vTbitmaps[Info->m_SCurrentObject] != SGD::INVALID_HANDLE
		&& "No Bitmap loaded for animation");

	if (!m_mOLoaded[Info->m_SCurrentObject][Info->m_SCurrentAnimation])
		Info->m_SCurrentAnimation = "Idle";

	Frame* Fr;
	if (Info->m_SCurrentAnimation == "Idle" && !m_mOLoaded[Info->m_SCurrentObject]["Idle"] ||
		(m_mOLoaded[Info->m_SCurrentObject][Info->m_SCurrentAnimation]->frames.size() - 1) < Info->m_uICurrentFrame)
	{
		if (Info->m_sLastAnimation == "" || Info->m_sLastAnimation == "Idle")
		{
			Fr = m_mOLoaded[Info->m_SCurrentObject]["South"]->frames[0];
		}
		else
			Fr = m_mOLoaded[Info->m_SCurrentObject][Info->m_sLastAnimation]->frames[0];
	}
	else
		Fr = m_mOLoaded[Info->m_SCurrentObject][Info->m_SCurrentAnimation]->frames[Info->m_uICurrentFrame];

	SGD::Point anchor = Fr->Anchor;
	SGD::Size Scale = scale;

	//DRAW TEXTURE
	SGD::Point poi(posX, posY);
	//pGraphics->DrawTextureSection(m_vTbitmaps[Info->m_SCurrentObject], { poi.x - anchor.x, poi.y - anchor.y },

	if (Info->m_SCurrentObject == "Queen")
	{
		SGD::Rectangle aC = { Fr->DrawRect };



		//float awidth = aC.ComputeWidth();
		//float aheight = aC.ComputeHeight();
		//aC.left = (posX + (anchor.x + Fr->DrawRect.left));
		//aC.right = (posX + (anchor.x + Fr->DrawRect.right));
		//aC.top = (posY + (anchor.y + Fr->DrawRect.top) - 24);
		//aC.bottom = (posY + (anchor.y + Fr->DrawRect.bottom) - 24);
		if (aC.IsEmpty() == false)
			pGraphics->DrawTextureSection(m_vTbitmaps[Info->m_SCurrentObject], { posX + (Fr->DrawRect.left - anchor.x), posY + (Fr->DrawRect.top - anchor.y) },
			Fr->DrawRect, 0.0, {}, color, Scale);
	}
	else
		pGraphics->DrawTextureSection(m_vTbitmaps[Info->m_SCurrentObject], { posX, posY - 24 },
		Fr->DrawRect, 0.0, {}, color, Scale);



	SGD::Rectangle aD = { Fr->DrawRect };





	std::string s = Fr->Event;



	if (Rects)
	{
		if (Fr->CollisionRect.top > 0)
		{

			SGD::Rectangle aC = { Fr->CollisionRect };



			float awidth = aC.ComputeWidth();
			float aheight = aC.ComputeHeight();
			aC.left = (posX + (Fr->CollisionRect.left - aD.left));
			aC.right = (posX + (Fr->CollisionRect.right - aD.left));
			aC.top = (posY + (Fr->CollisionRect.top - aD.top) - 24);
			aC.bottom = (posY + (Fr->CollisionRect.bottom - aD.top) - 24);
			if (aC.IsEmpty() == false)
				pGraphics->DrawRectangle(aC, { 0, 0, 0, 0 }, { 255, 0, 0 }, 1);
		}

		if (Fr->ActiveRect.top > 0)
		{

			std::vector<SGD::Rectangle> biorects = Camera::GetInstance()->GetMapLayer()->FindEventTiles("Save");
			SGD::Rectangle aC = { Fr->ActiveRect };


			float awidth = aC.ComputeWidth();
			float aheight = aC.ComputeHeight();
			aC.left = (posX + (Fr->ActiveRect.left - aD.left));
			aC.right = (posX + (Fr->ActiveRect.right - aD.left));
			aC.top = (posY + (Fr->ActiveRect.top - aD.top) - 24);
			aC.bottom = (posY + (Fr->ActiveRect.bottom - aD.top) - 24);
			if (aC.IsEmpty() == false)
				pGraphics->DrawRectangle(aC, { 0, 0, 0, 0 }, { 0, 255, 0 }, 1);
			for (size_t i = 0; i < biorects.size(); i++)
			{
				if (aC.IsIntersecting(biorects[i]))
				{
					if (s != "N/A" && s != "")
						pAudio->PlayAudio(m_vASounds[s]);
				}
			}
		}
	}

}

void AnimationSystem::Update(AnimationTimestamp* Info, float elapsedTime)
{

	if (!Info)
		return;
	//decriment FrameDuration by elapsed Time
	Info->m_FFrameDuration -= .5f * elapsedTime;

	Animation* Anim;
	if (Info->m_SCurrentAnimation == "Idle" && !m_mOLoaded[Info->m_SCurrentObject]["Idle"])
	{
		if (Info->m_sLastAnimation == "" || Info->m_sLastAnimation == "Idle")
		{
			Anim = m_mOLoaded[Info->m_SCurrentObject]["South"];
		}
		else
			Anim = m_mOLoaded[Info->m_SCurrentObject][Info->m_sLastAnimation];
	}
	else
		Anim = m_mOLoaded[Info->m_SCurrentObject][Info->m_SCurrentAnimation];



	if (Info->m_FFrameDuration < 0)
	{


		Info->m_uICurrentFrame++;

		if (Info->m_SCurrentObject == "BumblesWaterTrans")
			Info->m_isPlaying = true;


		if (Info->m_uICurrentFrame >= Anim->frames.size()) //If CurrentFrame is at the end
		{
			Info->m_uICurrentFrame = 0;


			if (Anim->looping == 0) //If looping is true reset Animation
				Info->m_SCurrentAnimation = Anim->IdleAnimation;

			if (Info->m_SCurrentObject == "BumblesWaterTrans")
				Info->m_isPlaying = false;

		}


		//Set Frame duration to new frame duration
		Info->m_FFrameDuration = Anim->frames[Info->m_uICurrentFrame]->frameDuration;


	}


}

//Loads in the Animation tree and stors it into Loaded
void AnimationSystem::Load(std::string name, std::string objName)
{

	if (m_mOLoaded[objName].size() > 0)
		return;
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();
	//Load XML Doc by name of Level
	std::string xml;
	if (objName == "Jenny" || objName == "Todd" || objName == "Ash" || objName == "Brock" || objName == "Kao")
	{
		xml = "resource/XMLFiles/Players/" + objName + ".xml";
	}
	else
		xml = "resource/XMLFiles/Creatures/" + objName + ".xml";
	TiXmlDocument doc(xml.c_str());
	if (!doc.LoadFile())
		return;
	TiXmlHandle hDoc(&doc);

	TiXmlElement* AnimationFiles = hDoc.ChildElement(0).Element();
	if (!AnimationFiles) return;
	// *

	TiXmlElement* Object = AnimationFiles->FirstChildElement();
	int AnimNum;
	Object->QueryIntAttribute("AnimationCount", &AnimNum);
	Object = Object->NextSiblingElement();
	// /*


	TiXmlElement* Animation = Object->FirstChildElement();

	//forloop for animations
	//Load animation name, Store in Loaded[][x]
	for (int i = 0; i < AnimNum; i++, Animation = Animation->NextSiblingElement())
	{
		int FrameNum;
		Animation->QueryIntAttribute("FrameCount", &FrameNum);
		std::string Aname = Animation->Attribute("AnimName");

		TiXmlElement* frame = Animation->FirstChildElement();
		if (m_mOLoaded[objName][Aname] && m_mOLoaded[objName][Aname]->frames.size() < 0)
			continue;
		else
			m_mOLoaded[objName][Aname] = new AnimationSystem::Animation;
		//forloop for frames
		//Load in frames, store in Loaded[][][x]
		for (int i = 0; i < FrameNum; i++)
		{
			Frame* temp = new Frame;

			frame->QueryFloatAttribute("DrawLeft", &temp->DrawRect.left);
			frame->QueryFloatAttribute("DrawTop", &temp->DrawRect.top);
			frame->QueryFloatAttribute("DrawRight", &temp->DrawRect.right);
			frame->QueryFloatAttribute("DrawBottom", &temp->DrawRect.bottom);

			frame->QueryFloatAttribute("CollisionLeft", &temp->CollisionRect.left);
			frame->QueryFloatAttribute("CollisionTop", &temp->CollisionRect.top);
			frame->QueryFloatAttribute("CollisionRight", &temp->CollisionRect.right);
			frame->QueryFloatAttribute("CollisionBottom", &temp->CollisionRect.bottom);

			frame->QueryFloatAttribute("EventLeft", &temp->ActiveRect.left);
			frame->QueryFloatAttribute("EventTop", &temp->ActiveRect.top);
			frame->QueryFloatAttribute("EventRight", &temp->ActiveRect.right);
			frame->QueryFloatAttribute("EventBottom", &temp->ActiveRect.bottom);

			frame->QueryFloatAttribute("AnchorLeft", &temp->Anchor.x);
			frame->QueryFloatAttribute("AnchorTop", &temp->Anchor.y);

			frame->QueryFloatAttribute("Duration", &temp->frameDuration);
			temp->Event = frame->Attribute("Event");

			m_mOLoaded[objName][Aname]->frames.push_back(temp);
			frame = frame->NextSiblingElement();

		}
	}
	std::string bml;
	if (objName == "Jenny" || objName == "Todd" || objName == "Ash" || objName == "Brock" || objName == "Kao")
	{
		bml = "resource/graphics/Players/" + name + ".png";
	}
	else
		bml = "resource/graphics/Creatures/" + name + ".png";

	m_vTbitmaps[objName] = SGD::GraphicsManager::GetInstance()->LoadTexture(bml.c_str());
	if (objName == "Player")
	{
		m_vASounds["RStep"] = pAudio->LoadAudio("resource/audio/Moo.wav");
		m_vASounds["LStep"] = pAudio->LoadAudio("resource/audio/Quak.wav");

	}
}

void AnimationSystem::CheckCollision(AnimationTimestamp* a, SGD::Point aP, AnimationTimestamp* b, SGD::Point bP)
{
	SGD::Rectangle aC = { m_mOLoaded[a->m_SCurrentObject][a->m_SCurrentAnimation]->frames[a->m_uICurrentFrame]->CollisionRect };
	SGD::Rectangle bC = { m_mOLoaded[b->m_SCurrentObject][b->m_SCurrentAnimation]->frames[b->m_uICurrentFrame]->CollisionRect };

	aC.bottom -= aC.top; aC.top -= aC.top;
	aC.right -= aC.left; aC.left -= aC.left;

	float awidth = aC.ComputeWidth();
	float aheight = aC.ComputeHeight();
	aC.left += (aP.x - (awidth / 2));
	aC.right += (aP.x + (awidth / 2));
	aC.top += (aP.y - (aheight / 2));
	aC.bottom += (aP.y + (aheight / 2));

	bC.bottom -= bC.top; bC.top -= bC.top;
	bC.right -= bC.left; bC.left -= bC.left;

	float bwidth = bC.ComputeWidth();
	float bheight = bC.ComputeHeight();
	bC.left += (bP.x - (bwidth / 2));
	bC.right += (bP.x + (bwidth / 2));
	bC.top += (bP.y - (bheight / 2));
	bC.bottom += (bP.y + (bheight / 2));



}



void AnimationSystem::Unload(std::string Objname)
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_vTbitmaps[Objname]);
	m_vTbitmaps.erase(Objname);
#if 0
	std::map<std::string, Animation*>::iterator iter = m_mOLoaded[Objname].begin();

	std::vector<Frame*>::iterator fiter;

	for (size_t i = 0; i < m_mOLoaded[Objname].size(); i++)
	{
		fiter = m_mOLoaded[Objname][iter->first.data()]->frames.begin();
		for (size_t i = 0; i < m_mOLoaded[Objname][iter->first.data()]->frames.size(); i++)
		{
			delete *fiter; //Loaded[Objname][iter->first.data()]->frames[i]; // go into object/ each animation/ clear all frames vectors
			fiter++;
		}
		m_mOLoaded[Objname][iter->first.data()]->frames.clear();
		delete m_mOLoaded[Objname][iter->first.data()];

		iter++;
	}
	m_mOLoaded[Objname].clear(); // go int Object clear all animations

	m_mOLoaded.erase(Objname);
	/////////
#else
	std::map<std::string, Animation*>::iterator iter = m_mOLoaded[Objname].begin();

	std::vector<Frame*>::iterator fiter;

	for (size_t i = 0; i < m_mOLoaded[Objname].size(); i++)
	{
		if (iter->second)
		{

			fiter = iter->second->frames.begin();
			for (size_t i = 0; i < iter->second->frames.size(); i++)
			{
				delete *fiter; //Loaded[Objname][iter->first.data()]->frames[i]; // go into object/ each animation/ clear all frames vectors
				fiter++;
			}
			iter->second->frames.clear();
			delete iter->second;
		}

		iter++;
	}
	m_mOLoaded[Objname].clear(); // go int Object clear all animations

	m_mOLoaded.erase(Objname);
#endif
}