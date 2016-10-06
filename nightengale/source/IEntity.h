#pragma once

#include "..\SGD Wrappers\SGD_Geometry.h"	// Rectangle type
#include "..\SGD Wrappers\SGD_Handle.h"
using namespace std;


class IEntity {

protected:

	IEntity(void) = default;

public:
	virtual ~IEntity(void) = default;

		//	Interface
	virtual void	Update(float elapsedTime) = 0;
	virtual void	Render(SGD::Point) = 0;

		//	Updates
	virtual void	HandleCollision(const IEntity* pOther) = 0;
	virtual void	PreformItemBehavaior(const IEntity* pOther) = 0;	
	virtual float	CheckLoS(IEntity* ToCheck) = 0;

		//	Accesors
	virtual int		GetType()	const = 0;	
	virtual int		GetDamage() const = 0;
	virtual SGD::Rectangle GetRect()	const = 0;
};