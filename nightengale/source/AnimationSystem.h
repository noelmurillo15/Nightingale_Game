#pragma once

#include <vector>
#include <map>

#include "..\SGD Wrappers\SGD_Geometry.h"
#include "..\SGD Wrappers\SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_Handle.h"


class AnimationTimestamp {

public:
	std::string m_SCurrentObject;
	std::string m_SCurrentAnimation;
	std::string m_sLastAnimation;
	unsigned int m_uICurrentFrame;
	float m_FFrameDuration;
	int m_IdleFrame;
	AnimationTimestamp(std::string obj, std::string anim);
	AnimationTimestamp(AnimationTimestamp* const& obj);

	bool m_isPlaying = true;

	void SetAnimation(std::string newanim)
	{
		if (m_SCurrentAnimation != "Idle")
			m_sLastAnimation = m_SCurrentAnimation;

		if (m_SCurrentAnimation != newanim)
			m_SCurrentAnimation = newanim;
	}
};

class AnimationSystem
{
	struct Frame
	{
		SGD::Rectangle DrawRect;
		SGD::Rectangle CollisionRect;
		SGD::Point Anchor;
		SGD::Rectangle ActiveRect;
		std::string Event;
		float frameDuration;
		int sound;
	};
	struct Animation
	{
		std::vector<Frame*> frames;
		std::string Name;
		std::string IdleAnimation = "Idle";
		bool looping = true;
		int ImageID;
	};
	static AnimationSystem*	s_pInstance;

	bool Rects = 0;
	std::map<std::string, SGD::HAudio> m_vASounds;
	std::map<std::string, SGD::HTexture> m_vTbitmaps;
	std::map<std::string, std::map<std::string, Animation*>> m_mOLoaded; //Animation tree -> Object will have a map of Animations, Animation will have a vector of frames
public:
	/**********************************************************/
	// Singleton Accessors:
	static AnimationSystem*	GetInstance(void);
	static void		DeleteInstance(void);

	AnimationSystem(void) = default;
	~AnimationSystem();
	AnimationSystem(const AnimationSystem&) = delete;	// c
	AnimationSystem& operator= (const AnimationSystem&) = delete;

	std::map<std::string, std::map<std::string, Animation*>> GetLoaded() { return m_mOLoaded; }
	void Render(AnimationTimestamp* Info, float posX, float posY,  SGD::Size scale, SGD::Color color);
	void Update(AnimationTimestamp* Info, float elapsedTime);
	void Load(std::string name, std::string objName);
	
	void CheckCollision(AnimationTimestamp* a, SGD::Point aP, AnimationTimestamp* b, SGD::Point bP);
	void Unload(std::string Objname);
};

