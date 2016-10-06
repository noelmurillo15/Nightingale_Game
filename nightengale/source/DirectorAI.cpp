#include "DirectorAI.h"

#include "GameplayState.h"
#include "Entity.h"
#include "DataWerehouse.h"
#include <vector>
using namespace std;


DirectorAI::DirectorAI() {
	m_pLiasion = GameplayState::GetInstance()->GetManager();
}

DirectorAI::~DirectorAI()
{
}

void DirectorAI::Update(float elapsedTime) {
#pragma region //Update creatures memory


		if (m_pLiasion->m_tEntities[Entity::ENT_ANIMAL].size())
		{
			for (unsigned int curr = 0; curr	< m_pLiasion->m_tEntities[Entity::ENT_ANIMAL].size(); curr++)
			{
				Entity* Creature = (Entity*)m_pLiasion->m_tEntities[Entity::ENT_ANIMAL][curr];
				DataWereHouse* CreatureBrain = Creature->GetDataWareHouse();
				if (CreatureBrain->m_vecLocated.size() != 0 || CreatureBrain->m_vecPercived.size() != 0)
				{
					CreatureBrain->SearchTimer += elapsedTime;
					CreatureBrain->MemTransitionTimer += elapsedTime;

					if (CreatureBrain->MemTransitionTimer >= PERCIEVED_TIMER)
					{
						CreatureBrain->MemTransitionTimer = 0;
						auto Iter = CreatureBrain->m_vecPercived.begin();

						if (Iter != CreatureBrain->m_vecPercived.end())
							CreatureBrain->m_vecPercived.erase(Iter);
					}
				}

		}
	}
#pragma endregion
}

void DirectorAI::CleanEntityMemory(void) {
	for (int bucket = 0; m_pLiasion->m_tEntities.size(); bucket++) {
		for (int curr = 0; m_pLiasion->m_tEntities[bucket].size(); curr++) {
			Entity* Creature = (Entity*)m_pLiasion->m_tEntities[bucket][curr];
		}
	}
}