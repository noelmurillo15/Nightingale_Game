#pragma once

#include "IEntity.h"


class Items : public IEntity {

	int m_itype;
	float m_fLifetime;
	SGD::Size m_Size;
	SGD::Size m_SmellEmitSz;
	SGD::Point m_Position;
	SGD::Rectangle m_Rect;
	SGD::Rectangle m_SmellEmitRect;

public:

	enum{ INT_BAIT, INT_REPELLENT };

	Items(){}
	Items(int type, SGD::Point Pos);
	~Items();

	virtual void Update(float elapsedTime);
	virtual void Render(SGD::Point);

	virtual void UpdateMoveSpeed(void){}
	int GetItemType(){ return m_itype; }
	virtual int	GetType(void)	const{ return 4; }
	virtual int	GetDamage() const{ return 0; }
	virtual void HandleCollision(const IEntity* pOther){}
	virtual void PreformItemBehavaior(const IEntity*){}
	virtual float CheckLoS(IEntity* ToCheck){ return 0.0f; }
	SGD::Rectangle GetSmellEmitRect(){ return m_SmellEmitRect; }
	virtual SGD::Rectangle GetRect(void)	const{ return m_Rect; }
	virtual void SetRect(SGD::Rectangle rect){}
	void Items::UpdateBaseRect(SGD::Point bawlz);

	SGD::Point GetPosition(void)const { return m_Position; }

	bool Age(float elapsedTime);
	bool isDying();
};