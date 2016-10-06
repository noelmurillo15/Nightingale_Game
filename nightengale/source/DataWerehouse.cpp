#include "DataWereHouse.h"
#include "Entity.h"
#include "CreatureProfile.h"

DataWereHouse::DataWereHouse()
{
}


DataWereHouse::~DataWereHouse()
{
}

void DataWereHouse::CreateProfile(Entity* Self, CreatureProfile* Profile)
{

	switch (Self->GetType())
	{
	case Entity::CRT_BISHOP :
		
			break;
	case Entity::CRT_BUMBLES:

		Profile->isPredator = false;
		Profile->isPrey = false;
		Profile->DangerLevel = 1;

		break;
	case Entity::CRT_CASTLE:
		break;
	case Entity::CRT_KING:
		break;
	case Entity::CRT_PACK:
		break;
	case Entity::CRT_PASSANT:
		break;
	case Entity::CRT_PAWN:
		break;
	case Entity::CRT_QUEEN:
		break;
	case Entity::CRT_ROOK:
		break;

	}

}

void DataWereHouse::AddNewIndex(string CreatureName, CreatureProfile* NewProfile)
{
	CreatureIndex[CreatureName] = NewProfile;

}