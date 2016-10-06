#include "PackComposite.h"
#include "DataWereHouse.h"
#include "Senses.h"
#include "Knight.h"
#include "CreatureProfile.h"
#include "SetPlayerTargetMessage.h"

PackComposite::PackComposite()
{

	m_ptBrain = new DataWereHouse;
	m_ptBrain->SetCurrentState(STATE_PASSIVE);

}


PackComposite::~PackComposite()
{
	for (unsigned int curr = 0; curr < m_vecPack.size(); curr++)
	{
		delete m_vecPack[curr];
		m_vecPack[curr] = nullptr;
	}
}

void PackComposite::Update(float elapsedTime)
{


	if (GetPackCondition() == true/* && m_ptBrain->GetCurrentState() != STATE_CURIOUS*/)
	{
		m_ptBrain->SetCurrentState(STATE_CURIOUS);

		Target = nullptr;

	}
	else if (GetPackCondition() == false && m_ptBrain->currentState != STATE_PASSIVE)
	{
		for (unsigned int curr = 0; curr < m_vecPack.size(); curr++)
		{
			Knight* CurrMember = (Knight*)m_vecPack[curr];

			CurrMember->imoveTowards = true;
			CurrMember->SetIsRepelled(false);
			CurrMember->SetIsBaityed(false);
		}

		m_ptBrain->SetCurrentState(STATE_PASSIVE);
		m_packLeader->SetTarget(m_packLeader);

	}


	PackDecisions(elapsedTime);
	Knight* Bill;
	for (size_t curr = 0; curr < m_vecPack.size(); curr++)
	{
		Bill = (Knight*)m_vecPack[curr];
		Bill->Update(elapsedTime);
	}


}
void PackComposite::Render(SGD::Point bawlz)
{
	for (unsigned int curr = 0; curr < m_vecPack.size(); curr++)
	{
		m_vecPack[curr]->Render(bawlz);
	}
}
void PackComposite::HandleCollision(const IEntity* pOther)
{
	Knight* temp;
	for (unsigned int curr = 0; curr < m_vecPack.size(); curr++)
	{
		temp = (Knight*)m_vecPack[curr];
		temp->HandleCollision(pOther);
	}
}
void PackComposite::PreformItemBehavaior(const IEntity* pOther)
{
	for (size_t curr = 0; curr < m_vecPack.size(); curr++)
	{
		m_vecPack[curr]->PreformItemBehavaior(pOther);
	}
}
void PackComposite::DetachPackMember(Entity*)
{

}
void PackComposite::PackDisband(void)
{

}
void PackComposite::UpdateMoveSpeed(void)
{
	switch (GetMoveMode()) {

	case MOVE_STAND:
		m_moveSpeed = 200;
		break;

	case MOVE_WALK:
		m_moveSpeed = 180;
		break;

	case MOVE_TROT:
		m_moveSpeed = 200;
		break;

	case MOVE_RUN:
		m_moveSpeed = 300;
		break;

	case MOVE_EXHAUSTED:
		m_moveSpeed = 40;
		break;
	}
}
bool PackComposite::GetPackCondition(void)
{

	for (size_t curr = 0; curr < m_vecPack.size(); curr++)
	{
		Knight* TempMem = (Knight*)m_vecPack[curr];

		if (TempMem->GetIsRepelled() || TempMem->GetIsBaited())
			return true;

	}

	return false;


}
void PackComposite::PackDecisions(float elapsedTime)
{
	
	unsigned int State = m_ptBrain->currentState;

	if (Target != nullptr && Target->GetAlive() == true)
	{
		if (Target->GetType() == ENT_ITEM)
		{
			int bill = 0;
		}
		if (Target->GetType() == ENT_ANIMAL)
		{

		}
	}


	if (State == STATE_PASSIVE)
	{

		if (Target == nullptr)
		{
			if (m_packLeader->GetDataWareHouse()->m_vecLocated.size())
			{
				FormationControl.m_bActive = true;
				for (size_t curr = 0; curr < m_packLeader->GetDataWareHouse()->m_vecLocated.size(); curr++)
				{
					Entity* Found = m_packLeader->GetDataWareHouse()->m_vecLocated[curr];

					int DangerZone = GetDataWareHouse()->CreatureIndex[Found->GetName()]->DangerLevel;

					if (DangerZone <= 1)
					{
						GetDataWareHouse()->SetCurrentState(STATE_AGGRO);
						m_iCurrentFormation = FormationControl.FRM_CIRCLE;
					}
					else if (DangerZone < 3)
					{
						GetDataWareHouse()->SetCurrentState(STATE_CURIOUS);
						m_iCurrentFormation = FormationControl.FRM_SCREEN;
					}
						
					else
					{
						FormationControl.m_bActive = false;
						GetDataWareHouse()->SetCurrentState(STATE_FEAR);
					}
						

				}

			}

			else if (m_packLeader->GetDataWareHouse()->m_vecPercived.size()) // How do they Search?
			{
				FormationControl.m_bActive = true;
				for (size_t curr = 0; curr < m_packLeader->GetDataWareHouse()->m_vecPercived.size(); curr++)
				{
					Entity* Looking = m_packLeader->GetDataWareHouse()->m_vecPercived[curr];
					Target = Looking;

				}
			}
			else
			{
				FormationControl.m_bActive = false;
				for (size_t curr = 0; curr < m_vecPack.size(); curr++)
				{
					Knight* wolf = (Knight*)m_vecPack[curr];
					wolf->Wander(elapsedTime);
				}
				Target = nullptr;

			}
		}


		if (Target != nullptr && Target->GetAlive() == true/*&& Target->GetType() == ENT_PLAYER*/)
		{
			for (unsigned int curr = 0; curr < m_vecPack.size(); curr++)
			{
				Knight* CurrMember = (Knight*)m_vecPack[curr];
				if (curr == 0)
					CurrMember->Following(Target);
				else
				{
					//FormationPosition.x = -40;
					//FormationPosition.y = -40;
					//FormationPosition.Rotate(RogueLeader->DegreeBetween);
					//CurrMember->m_Destination = SGD::Point(RogueLeader->GetPosition().x + FormationPosition.x, RogueLeader->GetPosition().y + FormationPosition.y);
					CurrMember->Following(m_vecPack[curr - 1]);
				}
			}
		}
		m_statedecay = 0;

	}
	else if (State == STATE_CURIOUS)
	{


		for (unsigned int curr = 0; curr < m_vecPack.size(); curr++)
		{
			Knight* CurrMember = (Knight*)m_vecPack[curr];

			if (CurrMember->GetIsRepelled())
				CurrMember->imoveTowards = false;
			else if (CurrMember->GetIsBaited())
				CurrMember->imoveTowards = true;
		}



	}
	else if (State == STATE_FEAR)
	{

	}
	else if (State == STATE_AGGRO)
	{


	}
}