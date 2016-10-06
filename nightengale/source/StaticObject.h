#pragma once

#include "IEntity.h"
#include <string>

class AnimationTimestamp;
class Entity;


class StaticObject : public IEntity  {

	SGD::Size m_sSize = { 32.0f, 32.0f };
	SGD::Point m_ptPosition = {0.0f, 0.0f};
	SGD::Rectangle m_Rect;
	static SGD::HAudio m_aSound;
	AnimationTimestamp* m_TimeStamp = nullptr;
	float m_ShakeTimer = 0;
	bool m_Occupied = false;

public:

	StaticObject(SGD::Point Pos);
	~StaticObject();

		//	Interface
	void Update(float elapsedTime);
	void Render(SGD::Point);

		//	Virtual Methods
	virtual int	GetType()const;
	virtual int	GetDamage() const{ return 0; }
	virtual void PreformItemBehavaior(const IEntity*){}
	virtual float CheckLoS(IEntity* ToCheck){ return 0.0f; }
	virtual SGD::Rectangle GetRect(void)	const{ return m_Rect; }

	void HandleCollision(const IEntity* pOther);
	void SetRect(SGD::Rectangle rect){ m_Rect = rect; }
};