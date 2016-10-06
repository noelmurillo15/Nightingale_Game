#include "Bumbles.h"
#include "CreatureProfile.h"
#include "AnimationSystem.h"
#include "SetPlayerTargetMessage.h"

Bumbles::Bumbles() {
	SetName("Bumbles");
	SetSize({ 64, 64 });
	SetSightEmitSz(GetSize() * 4);
	SetSoundEmitSz(GetSize() * 5);
	LoadData({ 100, 100 }, { 400, 400 }, { 200, 200 }, Camera::GetInstance()->GetMapLayer()->FindSpawnPoint(GetName()));

	SetHealth(200);	
	m_soundTimer = 10;
	SetMoveMode(MOVE_TROT);

	m_aSound = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/Belch.wav");
	AnimationSystem::GetInstance()->Load("Bumbles", "Bumbles");
	AnimationSystem::GetInstance()->Load("BumblesBack", "BumblesWaterTrans");
	AnimationSystem::GetInstance()->Load("BumblesBack", "BumblesInWater");
	InWaterTimeStamp = new AnimationTimestamp("BumblesInWater", "South");
	TransitioningTimeStamp = new AnimationTimestamp("BumblesWaterTrans", "South");
}

Bumbles::~Bumbles()
{
	SGD::AudioManager::GetInstance()->UnloadAudio(m_aSound);
	delete InWaterTimeStamp;
	delete TransitioningTimeStamp;
}

void Bumbles::Update(float elapsedTime)
{

	if (m_soundTimer < -10.0)
		m_soundTimer = 5;


	if (TransitioningTimeStamp->m_isPlaying == false)
		m_bTransitioning = false;


	if (m_bTransitioning)
	{
		AnimationSystem::GetInstance()->Update(TransitioningTimeStamp, elapsedTime);

	}
	else
	{

		LocalSenses->ProjectSight(this);
		Decision(elapsedTime);

		//if (Target != nullptr)
		//	SetDestination(Target->GetPosition());

		if (GetPosition() != GetDestination())
		{
			SGD::Point bal = GetPosition();
			SGD::Vector Direct = GetDestination() - this->GetPosition();
			Direct = Direct.ComputeNormalized();
			Direct = Direct * (float)GetMoveSpeed() * elapsedTime;
			bal.x += Direct.x;
			bal.y += Direct.y;
			Direct = Zoolander(Direct);

			SetPosition(bal);
		}
	}
	unsigned int State = this->GetDataWareHouse()->GetCurrentState();


	if (InWater)
		AnimationSystem::GetInstance()->Update(InWaterTimeStamp, elapsedTime);
	else
		AnimationSystem::GetInstance()->Update(TimeStamp, elapsedTime); 





		Entity::Update(elapsedTime);
}

void Bumbles::Render(SGD::Point bawlz)
{
	Entity::Render(bawlz);

	if (m_bTransitioning)
		AnimationSystem::GetInstance()->Render(TransitioningTimeStamp, m_currPosition.x - bawlz.x, m_currPosition.y - bawlz.y, { 1.25f, 1.25f }, { (unsigned char)GetAlpha(), 255, 255, 255 });
	else if (InWater == false)
		AnimationSystem::GetInstance()->Render(TimeStamp, m_currPosition.x - bawlz.x, m_currPosition.y - bawlz.y, { 1.25f, 1.25f }, { (unsigned char)GetAlpha(), 255, 255, 255 });
	else if (InWater == true)
		AnimationSystem::GetInstance()->Render(InWaterTimeStamp, m_currPosition.x - bawlz.x, m_currPosition.y - bawlz.y, { 1.25f, 1.25f }, { (unsigned char)GetAlpha(), 255, 255, 255 });
}

void Bumbles::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_ANIMAL) {
		Entity* tmpE = (Entity*)pOther;
		switch (tmpE->GetCreature()) {

		case CRT_BISHOP:
			break;

		case CRT_BUMBLES:
			break;

		case CRT_CASTLE:
			break;

		case CRT_KING:
			break;

		case CRT_PASSANT:
			break;

		case CRT_PAWN:
			break;

		case CRT_PRANCER:
			break;

		case CRT_QUEEN:
			break;

		case CRT_ROOK:
			break;

		default:
			break;
		}
	}
	else if (pOther->GetType() == ENT_ITEM){
		this->PreformItemBehavaior(pOther); // do this if i sense an item
	}
}

void Bumbles::PreformItemBehavaior(const IEntity* pOther)
{
	Items* tmpItem = (Items*)pOther;

	if (tmpItem->GetItemType() == Items::INT_BAIT){
		//	If i come near a bait -- do this
	}
	else if (tmpItem->GetItemType() == Items::INT_REPELLENT){
		//	If i come near a repellant -- do this
	}
}

void Bumbles::Decision(float elaspedTime)
{
	if (Target != nullptr && Target->GetType() == ENT_ANIMAL)
	{
		if (m_ptBrain->CreatureIndex[Target->GetName()]->DangerLevel <= 2)
		{
#pragma region // if the creature is not threatening
			if (m_ptBrain->CreatureIndex[Target->GetName()]->isPredator == false && m_ptBrain->CreatureIndex[Target->GetName()]->isPrey == false)
				m_ptBrain->currentState = STATE_CURIOUS;
			else if (m_ptBrain->CreatureIndex[Target->GetName()]->isPredator == false && m_ptBrain->CreatureIndex[Target->GetName()]->isPrey == true)
				m_ptBrain->currentState = STATE_AGGRO;
			else
				m_ptBrain->currentState = STATE_CURIOUS;
#pragma endregion
		}
		else
		{
#pragma region //If the Creature is Threatening

			if (m_ptBrain->CreatureIndex[Target->GetName()]->isPredator == false && m_ptBrain->CreatureIndex[Target->GetName()]->isPrey == false)
				m_ptBrain->currentState = STATE_CURIOUS;
			else if (m_ptBrain->CreatureIndex[Target->GetName()]->isPredator == false && m_ptBrain->CreatureIndex[Target->GetName()]->isPrey == true)
				m_ptBrain->currentState = STATE_AGGRO;
			else
				m_ptBrain->currentState = STATE_CURIOUS;
		}

#pragma endregion

	}

	unsigned int State = m_ptBrain->GetCurrentState();

	if (State == STATE_PASSIVE)
	{

		Wander(elaspedTime);

	}
	else if (State == STATE_CURIOUS)
	{

	}
	else if (State == STATE_FEAR)
	{

	}
	else if (State == STATE_AGGRO)
	{


	}


}

void Bumbles::Wander(float elaspedtime)
{

	int PosNeg = rand() % 100;

	WanderTimer += elaspedtime;
	if (WanderTimer >= WanderControl)
	{

		WanderTimer = 0;
		WanderControl = rand() % 5 + 5.0f;

#pragma region //where he is heading
		if (PosNeg > 50)
		{
			SGD::Point m_Dest = GetDestination();
			m_Dest.x = (rand() % 50) * -1.0f;
			m_Dest.y = (rand() % 50) * -1.0f;
			SetDestination(m_Dest);
		}
		else
		{
			SGD::Point m_Dest = GetDestination();
			m_Dest.x = (rand() % 50) * 1.0f;
			m_Dest.y = (rand() % 50) * 1.0f;
			SetDestination(m_Dest);
		}

		SGD::Point m_Dest = GetDestination();
		m_Dest.x += GetPosition().x;
		m_Dest.y += GetPosition().y;
		SetDestination(m_Dest);
#pragma endregion
	}

}

void Bumbles::UpdateMoveSpeed(){
	switch (this->GetMoveMode()) {

	case MOVE_STAND:
		m_moveSpeed = 0;
		break;

	case MOVE_WALK:
		m_moveSpeed = 90;
		break;

	case MOVE_TROT:
		m_moveSpeed = 120;
		break;

	case MOVE_RUN:
		m_moveSpeed = 500;
		break;

	case MOVE_EXHAUSTED:
		m_moveSpeed = 20;
		break;
	}
}