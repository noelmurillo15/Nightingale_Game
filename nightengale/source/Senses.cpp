#include "Senses.h"

#include "Entity.h"
#include "EntityManager.h"
#include "GameplayState.h"
#include "Player.h"
#include "../SGD Wrappers/SGD_InputManager.h"


Senses::Senses()
{

}

Senses::Senses(unsigned int _Type)
{

	switch (_Type)
	{

	case Entity::ENT_PLAYER:

		break;

	}
}


Senses::~Senses()
{

}

void Senses::CheckAllSenses(Entity* self)
{
	CheckListenCollision(self);
}

bool Senses::CheckListenCollision(Entity* self)
{
	m_pLiasion = GameplayState::GetInstance()->GetManager();


	EntityManager* CreatureTable = GameplayState::GetInstance()->GetManager();
	for (unsigned int i = 0; i < CreatureTable->m_tEntities[Entity::ENT_ANIMAL].size(); i++) {

		Entity* Animal = (Entity*)CreatureTable->m_tEntities[Entity::ENT_ANIMAL][i];

		if (self->GetSoundSenseRect().IsIntersecting(Animal->GetSoundEmitRect()) && Animal->GetCreature() != self->GetCreature())
		{
			self->SetTarget(Animal);
			if (self->GetSightSenseRect().IsIntersecting(Animal->GetSightEmitRect()))
			{
				return true;
			}
			return true;
		}
	}
	Entity* _TmpEntity = (Entity*)m_pLiasion->m_tEntities[Entity::ENT_PLAYER][0];//Is wrong need flexable index

	if (_TmpEntity != NULL){
		SGD::Vector m_Distance = self->GetPosition().ComputeDifference(_TmpEntity->GetPosition());

		if (self->GetSightSenseRect().IsIntersecting(_TmpEntity->GetSightEmitRect()))
		{
			self->SetTarget(_TmpEntity);
			if (self->GetSoundSenseRect().IsIntersecting(_TmpEntity->GetSoundEmitRect()))
			{
				return true;
			}
			SenseTimer = 5.0f;
			return true;
		}
	}
	return false;
}
int Senses::CheckSmellTrail(Entity* self) //returns a index to the spacific stank rect
{
	m_pLiasion = GameplayState::GetInstance()->GetManager();

	if (m_pLiasion->m_tEntities[Entity::ENT_PLAYER].size())
	{
		Player* _Player = (Player*)m_pLiasion->m_tEntities[Entity::ENT_PLAYER][0];

		if (_Player->m_ListSmellTrail.size())
		for (size_t curr = 0; curr < _Player->m_ListSmellTrail.size(); curr++)
		if (_Player->m_ListSmellTrail[curr]->m_recStankVolume.IsIntersecting(self->GetSmellSenseRect()))
			return curr;
	}
		
	return -1;
}

int Senses::CheckProximity(Entity* self)
{
	m_pLiasion = GameplayState::GetInstance()->GetManager();
	Entity* _TempPlayer = (Entity*)m_pLiasion->m_tEntities[Entity::ENT_PLAYER][0];//Is wrong need flexable index

	if (_TempPlayer != NULL){
		SGD::Vector m_Distance = self->GetPosition().ComputeDifference(_TempPlayer->GetPosition());

		if (self->GetSoundSenseRect().IsIntersecting(_TempPlayer->GetSoundEmitRect()))
		{
			SenseTimer = 10.0f;
			self->SetTarget(_TempPlayer);
			if (self->GetSightSenseRect().IsIntersecting(_TempPlayer->GetSightEmitRect())){
				self->SetTarget(_TempPlayer);
				return VISIBLE;
			}
			return CAN_HEAR;
		}
	}
	return NOT_SENSED;
}

void Senses::ProjectSight(Entity* Self) {

	EntityManager* CreatureTable = GameplayState::GetInstance()->GetManager();

	Entity* Player = (Entity*)CreatureTable->m_tEntities[Entity::ENT_PLAYER][0];

	if (Player != NULL){
		SGD::Vector m_Distance = Self->GetPosition().ComputeDifference(Player->GetPosition());

		if (m_Distance.ComputeLength() <= 300 && Player->GetInHiding() == false)
		{
#pragma region // Can I see a Player?
			float rDegree = Self->CheckLoS(Player);

			if ((rDegree <= (sqrt(3)) / 2 && rDegree > 0))
			{

				if (Self->GetDataWareHouse()->m_vecLocated.size() == 0)
				{
					if (Player == Self->GetTarget())
						Self->GetDataWareHouse()->m_vecLocated.push_back(Player);
				}

				for (auto Find_iter = Self->GetDataWareHouse()->m_vecLocated.begin(); Find_iter != Self->GetDataWareHouse()->m_vecLocated.end(); Find_iter++)
				{

					if ((*Find_iter) == Player)
						break;

					if (Player == Self->GetTarget())
						Self->GetDataWareHouse()->m_vecLocated.push_back(Player);
				}
			}
			else
			{
				for (auto Locate_iter = Self->GetDataWareHouse()->m_vecLocated.begin(); Locate_iter != Self->GetDataWareHouse()->m_vecLocated.end(); Locate_iter++)
				{
					if ((*Locate_iter) == Player)
					{

						auto Percieved_Iter = Self->GetDataWareHouse()->m_vecPercived.begin();
						for (; Percieved_Iter != Self->GetDataWareHouse()->m_vecPercived.end(); Percieved_Iter++)
						{
							if ((*Percieved_Iter) == Player)
								break;
						}

						Self->GetDataWareHouse()->m_vecPercived.push_back((*Locate_iter));
						Self->GetDataWareHouse()->m_vecLocated.erase(Locate_iter);
						break;
					}

				}
#pragma endregion
			}

			for (unsigned int i = 0; i < CreatureTable->m_tEntities[Entity::ENT_ANIMAL].size(); i++)
			{
				Entity* Animal = (Entity*)CreatureTable->m_tEntities[Entity::ENT_ANIMAL][i];
				m_Distance = Self->GetPosition().ComputeDifference(Animal->GetPosition());

				if (m_Distance.ComputeLength() <= 300 && Animal->GetInHiding() == false)
				{
					float rDegree = Self->CheckLoS(Animal);

#pragma region //Can I See the Animal?
					if ((rDegree <= (sqrt(3)) / 2 && rDegree > 0))
					{

						//I Saw a creature 
						if (Self->GetDataWareHouse()->m_vecLocated.size() == 0)
						{
							if (Animal == Self->GetTarget())
								Self->GetDataWareHouse()->m_vecLocated.push_back(Animal);
						}

						for (auto Find_iter = Self->GetDataWareHouse()->m_vecLocated.begin(); Find_iter != Self->GetDataWareHouse()->m_vecLocated.end(); Find_iter++)
						{
							if ((*Find_iter) == Animal)
								break;
							if (Animal == Self->GetTarget())
								Self->GetDataWareHouse()->m_vecLocated.push_back(Animal);
						}

						if (Animal == Self->GetTarget())
							Self->GetDataWareHouse()->m_vecLocated.push_back(Animal);
					}
					else
					{

						for (auto Find_iter = Self->GetDataWareHouse()->m_vecLocated.begin(); Find_iter != Self->GetDataWareHouse()->m_vecLocated.end(); Find_iter++)
						{
							if ((*Find_iter) == Animal)
							{
								Self->GetDataWareHouse()->m_vecPercived.push_back((*Find_iter));
								Self->GetDataWareHouse()->m_vecLocated.erase(Find_iter);
								break;
							}
						}
					}
#pragma endregion
				}

			}
		}
	}
}

void Senses::UpdateTimer(float elapsedTime){
	if (SenseTimer > 0)
		SenseTimer -= elapsedTime;
}