#pragma once

#include "Entity.h"


class Kao : public Entity {

public:

	bool MsgSent = false;

	Kao();
	~Kao();

	//	Overrides
	void Update(float elapsedtime);
	void Render(SGD::Point);

	void PreformItemBehavaior(const IEntity*) override;
	void HandleCollision(const IEntity* pOther) override;

	int GetType() const override { return ENT_BASE; }

	// Mutators:
	void SetPosition(SGD::Point	pos) 	{ Entity::SetPosition(pos); }
	void SetSize(SGD::Size size)		{ m_szSize = size; }
};