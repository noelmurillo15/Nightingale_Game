/***************************************************************
|	File:		EntityManager.cpp
|	Author: YoMomma
|	Course: 
|	Purpose:	EntityManager class stores & maintains all game entities
***************************************************************/

#include "EntityManager.h"

#include "Camera.h"
#include "Player.h"
#include "PackComposite.h"

#include <cassert>


EntityManager::EntityManager()
{

}

/**************************************************************/
// AddEntity
//	- store the entity into the specified bucket
//	- the Entity Manager holds a reference to the entity
void EntityManager::AddEntity(IEntity* pEntity, unsigned int bucket)
{

	// Validate the parameter
	assert(pEntity != nullptr && "EntityManager::AddEntity - parameter cannot be null");

	// Expand the table?
	if (bucket >= m_tEntities.size())
		m_tEntities.resize(bucket + 1);

	// Append the entity into the specified vector
	m_tEntities[bucket].push_back(pEntity);

	// Hold a reference to keep the entity in memory
}


/**************************************************************/
// RemoveEntity
//	- remove the entity from the specified bucket
//	- release the reference to the entity
void EntityManager::RemoveEntity(IEntity* pEntity, unsigned int bucket)
{
	// Validate the iteration state
	assert(m_bIterating == false && "EntityManager::RemoveEntity - cannot remove while iterating");

	// Validate the parameters
	assert(pEntity != nullptr && "EntityManager::RemoveEntity - cannot remove NULL");
	assert(bucket < m_tEntities.size() && "EntityManager::RemoveEntity - invalid bucket");


	// Try to find the entity
	EntityVector& vec = m_tEntities[bucket];
	for (unsigned int i = 0; i < vec.size(); i++)
	{
		if (vec[i] == pEntity)
		{

			// Remove the entity
			vec.erase(vec.begin() + i);
			break;
		}
	}
}

void EntityManager::RemoveAll(void)
{
	// Validate the iteration state
	assert(m_bIterating == false && "EntityManager::RemoveAll - cannot remove while iterating");

	// Lock the iterator
	m_bIterating = true;
	{
		// Release every entity
		for (unsigned int bucket = 0; bucket < m_tEntities.size(); bucket++)
		{
			EntityVector& vec = m_tEntities[bucket];
			for (unsigned int i = 0; i < vec.size(); i++)
			{
				if (bucket == Entity::ENT_PLAYER){
					//SaveSlot tmp;
					//Entity* tmpPlayer = (Entity*)vec[i];
					//tmp.SaveToXml(tmpPlayer);
				}
				else
					delete vec[i];
				vec[i] = nullptr;

			}
		}
	}
	// Unlock the iterator
	m_bIterating = false;


	// Collapse the table
	m_tEntities.clear();
}

void EntityManager::RemoveACertainBucket(unsigned int bucket)
{
	// Validate the iteration state
	assert(m_bIterating == false && "EntityManager::RemoveAll - cannot remove while iterating");

	if (m_tEntities.size() < bucket)
		return;

	// Lock the iterator
	m_bIterating = true;
	{

		EntityVector& vec = m_tEntities[bucket];
		for (unsigned int i = 0; i < vec.size(); i++)
		{
			delete vec[i];
			vec[i] = nullptr;
		}
		vec.clear();
	}
	// Unlock the iterator
	m_bIterating = false;
}

void EntityManager::UpdateAll(float elapsedTime)
{
	// Validate the iteration state
	assert(m_bIterating == false && "EntityManager::UpdateAll - cannot update while iterating");
	//CheckLoS(Entity::ENT_ANIMAL, (Entity*)m_tEntities[Entity::ENT_PLAYER][0]);
	// Lock the iterator
	m_bIterating = true;
	{
		// Update every entity
		for (unsigned int bucket = 0; bucket < m_tEntities.size(); bucket++)
		{
			EntityVector& vec = m_tEntities[bucket];

			for (unsigned int i = 0; i < vec.size(); i++){
				if (bucket == Entity::ENT_PLAYER)
				{
					Player* tempPlayer = (Player*)vec[i];

					tempPlayer->Update(elapsedTime);

					tempPlayer->SetCanHarvestBait(tempPlayer->EventHandleCollision(Camera::GetInstance()->GetMapLayer()->FindEventTiles("HarvestBait"), Camera::GetInstance()->GetCameraOffset(), false));					
					tempPlayer->SetCanHarvestRepellent(tempPlayer->EventHandleCollision(Camera::GetInstance()->GetMapLayer()->FindEventTiles("HarvestRepellent"), Camera::GetInstance()->GetCameraOffset(), false));					
					tempPlayer->SetCanSave(tempPlayer->EventHandleCollision(Camera::GetInstance()->GetMapLayer()->FindEventTiles("Save"), Camera::GetInstance()->GetCameraOffset(), false));
					tempPlayer->SetLevelTransition(tempPlayer->EventHandleCollision(Camera::GetInstance()->GetMapLayer()->FindEventTiles("End"), Camera::GetInstance()->GetCameraOffset(), false));		
					tempPlayer->EventHandleCollision(Camera::GetInstance()->GetMapLayer()->FindEventTiles("Poison"), Camera::GetInstance()->GetCameraOffset(), true);
				}
				else if (bucket == Entity::ENT_ITEM){


					vec[i]->Update(elapsedTime);

					auto iter = vec.begin();
					for (size_t x = 0; x < vec.size(); x++){
						Items* tmpItem = (Items*)vec[x];
						if (tmpItem->Age(elapsedTime) == 1){

							for (size_t i = 0; i < this->m_tEntities[Entity::ENT_ANIMAL].size(); i++)
							{
								Entity* temp = (Entity*)this->m_tEntities[Entity::ENT_ANIMAL][i];
								if (temp->GetTarget() == vec[x])
									temp->SetTarget(nullptr);
							}

							delete vec[x];
							vec.erase(iter);
							iter = vec.begin();
							x = 0;
						}
					}
				}
				else{
					vec[i]->Update(elapsedTime);
				}
			}
		}
	}
	// Unlock the iterator
	m_bIterating = false;
}

void EntityManager::RenderAll(SGD::Point  CameraOffset)
{
	// Validate the iteration state
	assert(m_bIterating == false && "EntityManager::RenderAll - cannot render while iterating");

	Player* tempPlayer = nullptr;

	// Lock the iterator
	m_bIterating = true;
	{
		// Render every entity
		for (unsigned int bucket = 0; bucket < m_tEntities.size(); bucket++)
		{
			EntityVector& vec = m_tEntities[bucket];
			for (unsigned int i = 0; i < vec.size(); i++){
				if (bucket == Entity::ENT_PLAYER)
				{
					tempPlayer = (Player*)vec[i];
				}
				if (bucket == Entity::ENT_ITEM){
					Items* tmpItem = (Items*)vec[i];
					tmpItem->Render(CameraOffset);
				}
				else{
					vec[i]->Render(CameraOffset);
				}
			}
		}
		if (tempPlayer != nullptr)
			tempPlayer->Render(CameraOffset); // render player last
	}
	// Unlock the iterator
	m_bIterating = false;
}

void EntityManager::CheckCollisions(unsigned int bucket1, unsigned int bucket2)
{
	// Validate the iteration state
	assert(m_bIterating == false && "EntityManager::CheckCollisions - cannot collide while iterating");

	// Quietly validate the parameters
	if (bucket1 >= m_tEntities.size()
		|| bucket2 >= m_tEntities.size()
		|| m_tEntities[bucket1].size() == 0
		|| m_tEntities[bucket2].size() == 0)
		return;


	// Lock the iterator
	m_bIterating = true;
	// Are they different buckets?
	if (bucket1 != bucket2)
	{
		// Which bucket is smaller?
		//	should be the outer loop for less checks (n0)*(n1+1) + 1
		EntityVector* pVec1 = &m_tEntities[bucket1];
		EntityVector* pVec2 = &m_tEntities[bucket2];

		if (pVec2->size() < pVec1->size())
		{
			EntityVector* pTemp = pVec1;
			pVec1 = pVec2;
			pVec2 = pTemp;
		}

		EntityVector& vec1 = *pVec1;
		EntityVector& vec2 = *pVec2;

		// Iterate through the smaller bucket
		for (unsigned int i = 0; i < vec1.size(); i++)
		{
			// Iterate through the larger bucket
			for (unsigned int j = 0; j < vec2.size(); j++)
			{

				if (vec1[i]->GetType() == Entity::ENT_ITEM || vec2[j]->GetType() == Entity::ENT_PACK)
				{
					SGD::Rectangle itemSmellRect;
					Items* tmpitem;
					Entity* tmpE;

					if (vec1[i]->GetType() == Entity::ENT_ITEM)
					{
						if (vec2[j]->GetType() == Entity::ENT_PACK)
						{
							PackComposite* HandlePack = (PackComposite*)vec2[j];

							tmpitem = (Items*)vec1[i];
							tmpE = (Entity*)HandlePack->m_vecPack[0];
							itemSmellRect = tmpitem->GetSmellEmitRect();
							itemSmellRect.left -= Camera::GetInstance()->GetCameraOffset().x;
							itemSmellRect.right -= Camera::GetInstance()->GetCameraOffset().x;
							itemSmellRect.top -= Camera::GetInstance()->GetCameraOffset().y;
							itemSmellRect.bottom -= Camera::GetInstance()->GetCameraOffset().y;

						}
						else
						{

							tmpitem = (Items*)vec1[i];
							tmpE = (Entity*)vec2[j];
							itemSmellRect = tmpitem->GetSmellEmitRect();
							itemSmellRect.left -= Camera::GetInstance()->GetCameraOffset().x;
							itemSmellRect.right -= Camera::GetInstance()->GetCameraOffset().x;
							itemSmellRect.top -= Camera::GetInstance()->GetCameraOffset().y;
							itemSmellRect.bottom -= Camera::GetInstance()->GetCameraOffset().y;
						}

						if (itemSmellRect.IsIntersecting(tmpE->GetSmellSenseRect()))
							vec2[j]->HandleCollision(vec1[i]);
					}
					else
					{
						tmpitem = (Items*)vec2[j];
						tmpE = (Entity*)vec1[i];
						itemSmellRect = tmpitem->GetSmellEmitRect();
						itemSmellRect.left -= Camera::GetInstance()->GetCameraOffset().x;
						itemSmellRect.right -= Camera::GetInstance()->GetCameraOffset().x;
						itemSmellRect.top -= Camera::GetInstance()->GetCameraOffset().y;
						itemSmellRect.bottom -= Camera::GetInstance()->GetCameraOffset().y;
						if (itemSmellRect.IsIntersecting(tmpE->GetSmellSenseRect()))
							vec1[i]->HandleCollision(vec2[j]);

					}
				}
				else{

					Entity* player = nullptr;
					Entity* animal = nullptr;

					if (vec1[i]->GetType() == Entity::ENT_PLAYER){
						player = (Entity*)vec1[i];
						animal = (Entity*)vec2[j];
					} else {
						player = (Entity*)vec2[j];
						animal = (Entity*)vec1[i];
					}

					// Check for collision between the entities
					if (animal->GetSightSenseRect().IsIntersecting(player->GetSightEmitRect()) == true) {
						vec1[i]->HandleCollision(vec2[j]);
						vec2[j]->HandleCollision(vec1[i]);
					}
				}
			}

		}
	}
	else // bucket1 == bucket2
	{
		EntityVector& vec = m_tEntities[bucket1];

		// Optimized loop to ensure objects do not collide with
		// each other twice
		for (unsigned int i = 0; i < vec.size() - 1; i++)
		{
			for (unsigned int j = i + 1; j < vec.size(); j++)
			{
				// Ignore self-collision
				if (vec[i] == vec[j])
					continue;

				Entity* rEnt1 = (Entity*)vec[i];
				Entity* rEnt2 = (Entity*)vec[j];

				if (rEnt1->GetSightSenseRect().IsIntersecting(rEnt2->GetSightEmitRect()))
					vec[i]->HandleCollision(vec[j]);

				if (rEnt2->GetSightSenseRect().IsIntersecting(rEnt1->GetSightEmitRect()))
					vec[j]->HandleCollision(vec[i]);
			}
		}
	}

	// Unlock the iterator
	m_bIterating = false;

}

void EntityManager::CheckEnvironmentCollisions(unsigned int bucket1){

	// Validate the iteration state
	assert(m_bIterating == false && "EntityManager::CheckCollisions - cannot collide while iterating");

	// Quietly validate the parameters
	if (bucket1 >= m_tEntities.size()
		|| Entity::ENT_BUSH >= m_tEntities.size()
		|| m_tEntities[bucket1].size() == 0
		|| m_tEntities[Entity::ENT_BUSH].size() == 0)
		return;

	// Lock the iterator
	m_bIterating = true;

	// Which bucket is smaller?
	EntityVector* pVec1 = &m_tEntities[bucket1];
	EntityVector* pVec2 = &m_tEntities[Entity::ENT_BUSH];

	if (pVec2->size() < pVec1->size())
	{
		EntityVector* pTemp = pVec1;
		pVec1 = pVec2;
		pVec2 = pTemp;
	}

	EntityVector& vec1 = *pVec1;
	EntityVector& vec2 = *pVec2;

	Entity* entity = nullptr;
	IEntity* bush = nullptr;

	// Iterate through the smaller bucket
	for (unsigned int i = 0; i < vec1.size(); i++)
	{
		// Iterate through the larger bucket
		for (unsigned int j = 0; j < vec2.size(); j++)
		{
			if (vec1[i]->GetType() == Entity::ENT_BUSH){
				bush = (IEntity*)vec1[i];
				entity = (Entity*)vec2[j];
			}
			else {
				bush = (IEntity*)vec2[j];
				entity = (Entity*)vec1[i];
			}

			if (entity->GetInHiding())
				entity->SetInHiding(false);

			if (bush->GetRect().IsIntersecting(entity->GetRect()) == true) {
				bush->HandleCollision(entity);
				entity->SetInHiding(true);
				break;
			}
		}
	}
	m_bIterating = false;
}