#include "Knight.h"
#include "DataWereHouse.h"
#include "AnimationSystem.h"
#include "Senses.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "SetPlayerTargetMessage.h"
#include "../SGD Wrappers/SGD_AudioManager.h"


Knight::Knight()
{
	SetName("Knight");
	SetSize({ 32, 32 });
	SetSightEmitSz(GetSize() * 4);
	SetSoundEmitSz(GetSize() * 5);
	LoadData({ 150, 150 }, { 150, 150 }, { 400, 400 }, Camera::GetInstance()->GetMapLayer()->FindSpawnPoint(GetName()));
	
	m_damage = 35;
	m_soundTimer = 10;
	SetHealth(90);

	AnimationSystem::GetInstance()->Load("Knight", "Knight");
}


Knight::~Knight()
{

}

void Knight::Update(float elapsedTime)
{
	UpdateControl += elapsedTime;
	if (isBaited == true || isRepelled == true)
		itemTimer += elapsedTime;


	LocalSenses->ProjectSight(this);

	if (UpdateControl >= RandDelay)
	{
		RandDelay = rand() % 5 + 1.0f;
		RandDelay *= 0.17f;
		UpdateControl = 0;
		Decision(elapsedTime);
	}


	if ((isBaited == true || isRepelled == true) && itemTimer > 5)
	{
		isBaited = false;
		isRepelled = false;
		itemTimer = 0;
	}


	if (GetPosition() != GetDestination())
	{
		SGD::Point bal = GetPosition();
		SGD::Vector Direct = GetDestination() - GetPosition();
		if (imoveTowards)
			Direct = Direct.ComputeNormalized();
		else
			Direct = -Direct.ComputeNormalized();

		Direct = Direct * (float)GetMoveSpeed() * elapsedTime;


		Direct = Zoolander(Direct);

		DegreeBetween = acos(Direct.ComputeDotProduct(m_vecDirection));

		SetPosition(bal);
	}
	Entity::Update(elapsedTime);
}
void	Knight::Render(SGD::Point bawlz)
{
	Entity::Render(bawlz);
}


void	Knight::HandleCollision(const IEntity* pOther)
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

		case CRT_PLAYER:

			break;
		default:
			break;
		}
	}
	else if (pOther->GetType() == ENT_ITEM){
		this->PreformItemBehavaior(pOther); // do this if i sense an item
	}

}
void	Knight::PreformItemBehavaior(const IEntity* pOther)
{
	Items* tmpItem = (Items*)pOther;

	if (tmpItem->GetItemType() == Items::INT_BAIT){
		Target = (Entity*)tmpItem;
		m_destination = tmpItem->GetPosition();
		isBaited = true;
	}
	else if (tmpItem->GetItemType() == Items::INT_REPELLENT){
		Target = (Entity*)tmpItem;
		m_destination = tmpItem->GetPosition();
		m_ptBrain->currentState = STATE_CURIOUS;
		isRepelled = true;
	}
}
void Knight::Decision(float elaspedTime)
{

}
void Knight::Wander(float elaspedtime)
{






}
bool Knight::Following(Entity* tracking)
{
	Target = tracking;

	SGD::Vector distance = GetPosition().ComputeDifference(Target->GetPosition());
	if (distance.ComputeLength() <= 20)
	{
		SetMoveMode(MOVE_STAND);
		SetDestination(GetPosition());
		return false;
	}
	else
	{
		SetMoveMode(MOVE_TROT);
		SetDestination(Target->GetPosition());
		return true;
	}



}
void Knight::UpdateMoveSpeed()
{
	switch (GetMoveMode()) {

	case MOVE_STAND:
		m_moveSpeed = 0;
		break;

	case MOVE_WALK:
		m_moveSpeed = 90;
		break;

	case MOVE_TROT:
		m_moveSpeed = 180;
		break;

	case MOVE_RUN:
		m_moveSpeed = 320;
		break;

	case MOVE_EXHAUSTED:
		m_moveSpeed = 40;
		break;
	}
}
