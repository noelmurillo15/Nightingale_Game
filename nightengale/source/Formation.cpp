#include "Formation.h"
#include "Entity.h"
#include "Knight.h"

Formation::Formation()
{
	//PackPosition.resize(5, true);
}

Formation::Formation(Entity* Core)
{

	FormationCore = Core;
}


Formation::~Formation()
{
}

void Formation::Update(float ElapsedTime, int CurrentForm)
{

	
		OffSetPosition(FormationCore, CurrentForm);




}

SGD::Point Formation::OffSetPosition(Entity* Creature, int CurrentForm)
{

	SGD::Point NewDestination;

	if (Creature != nullptr)
	{
		Knight* PackMember = (Knight*)Creature;

		if (CurrentForm == FRM_CIRCLE)
		{

		}
		else if (CurrentForm == FRM_DIAMOND)
		{

		}
		else if (CurrentForm == FRM_SCREEN)
		{

		}
	}
	else
	{

	}

	return NewDestination;
}
