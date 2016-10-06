#include "Rook.h"


Rook::Rook() {
	SetName("Rook");
	SetSize({ 48, 48 });
	SetSightEmitSz(GetSize() * 4);
	SetSoundEmitSz(GetSize() * 5);
	LoadData({ 200, 200 }, { 500, 500 }, { 1000, 1000 }, Camera::GetInstance()->GetMapLayer()->FindSpawnPoint(GetName()));

	SetHealth(120);
	
	m_soundTimer = 3;
	m_aSound = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/Rook.wav");

	AnimationSystem::GetInstance()->Load("Rook", "Rook");
}

Rook::~Rook() {
	SGD::AudioManager::GetInstance()->UnloadAudio(m_aSound);
}

void Rook::Update(float elapsedTime) {

	SGD::Vector TempVec = { 0, 0 };
	SGD::Point NextPos = GetPosition();

	if (WanderTimer >= 0)
		WanderTimer -= elapsedTime;

	if (m_soundTimer >= 0)
		m_soundTimer -= elapsedTime;

	if (SuperBait >= 0)
		SuperBait -= elapsedTime;
	else if (IsStuffed && SuperBait <= 0){
		IsStuffed = false;
		Asleep = true;
		GetDataWareHouse()->SetCurrentState(STATE_ASLEEP);
	}
	


	switch (GetDataWareHouse()->GetCurrentState()){

	case STATE_PASSIVE:
#pragma region Passive
		if (GetDestination() != SGD::Point(-10.0f, -10.0f)) {
			TempVec = GetDestination() - GetPosition();

			TempVec = TempVec.ComputeNormalized();
			TempVec = TempVec * (float)(GetMoveSpeed())* elapsedTime;

			if (abs(GetPosition().ComputeDifference(GetDestination()).x) < 10 &&
				abs(GetPosition().ComputeDifference(GetDestination()).y) < 10){
				SetDestination({ -10.0f, -10.0f });
				GetDataWareHouse()->SetCurrentState(STATE_PASSIVE);
				break;
			}
		}
		else
			SetDestination(Camera::GetInstance()->GetMapLayer()->FindCloseBush(Camera::GetInstance()->GetCameraBounds()));

		SetMoveMode(MOVE_WALK);
#pragma endregion
		break;

	case STATE_FEAR:
#pragma region Fear
		if (GetDestination() == SGD::Point(-10.0f, -10.0f))
			SetDestination(Camera::GetInstance()->GetMapLayer()->FindRandomBush());

		if (abs(GetPosition().ComputeDifference(GetDestination()).x) < 8 &&
			abs(GetPosition().ComputeDifference(GetDestination()).y) < 8) {
			SetDestination({ -10.0f, -10.0f });
			GetDataWareHouse()->SetCurrentState(STATE_PASSIVE);
			break;
		}

		TempVec = GetDestination() - GetPosition();
		TempVec = (TempVec.ComputeNormalized());
		TempVec *= (float)(GetMoveSpeed()) * elapsedTime;

		SetMoveMode(MOVE_RUN);
#pragma endregion
		break;

	case STATE_CURIOUS:
#pragma region Curious
		if (GetDestination() != SGD::Point(-10.0f, -10.0f)){
			TempVec = GetDestination() - GetPosition();

			TempVec = TempVec.ComputeNormalized();
			TempVec = TempVec * (float)(GetMoveSpeed())* elapsedTime;

			if (abs(GetPosition().ComputeDifference(GetDestination()).x) < 32 &&
				abs(GetPosition().ComputeDifference(GetDestination()).y) < 32) {

				SuperBait = 12.0f;
				IsStuffed = true;

				SetDestination({ -10.0f, -10.0f });
				GetDataWareHouse()->SetCurrentState(STATE_PASSIVE);
			}
		}
		else{
			if (GetTarget() != NULL)
				SetDestination(GetTarget()->GetPosition());
		}

		SetMoveMode(MOVE_TROT);
#pragma endregion
		break;

	case STATE_ALERT:
		SetMoveMode(MOVE_STAND);
		break;

	case STATE_ASLEEP:
		if (TimeStamp->m_SCurrentAnimation != "Sleep")
			TimeStamp->SetAnimation("Sleep");

		SetMoveMode(MOVE_STAND);
		break;
	}


#pragma region Update Position
	TempVec = Zoolander(TempVec);
	NextPos.x += TempVec.x;
	NextPos.y += TempVec.y;
	SetPosition(NextPos);
#pragma endregion

	Entity::Update(elapsedTime);	
}

void Rook::Render(SGD::Point bawlz) 
{
	Entity::Render(bawlz);
}

void Rook::HandleCollision(const IEntity* pOther) {

	Entity* tmpE = (Entity*)pOther;

	if (tmpE->GetType() == ENT_ANIMAL){
		switch (tmpE->GetCreature()){

		case CRT_BUMBLES:
			if (GetDataWareHouse()->GetCurrentState() != STATE_FEAR &&
				GetDataWareHouse()->GetCurrentState() != STATE_ASLEEP)
				GetDataWareHouse()->SetCurrentState(STATE_FEAR);
			break;

		case CRT_KING:
			if (GetDataWareHouse()->GetCurrentState() != STATE_FEAR &&
				GetDataWareHouse()->GetCurrentState() != STATE_ASLEEP)
				GetDataWareHouse()->SetCurrentState(STATE_FEAR);
			break;

		case CRT_PRANCER:
			if (GetDataWareHouse()->GetCurrentState() != STATE_FEAR &&
				GetDataWareHouse()->GetCurrentState() != STATE_ASLEEP)
				GetDataWareHouse()->SetCurrentState(STATE_FEAR);
			break;

		case CRT_QUEEN:
			if (GetDataWareHouse()->GetCurrentState() != STATE_FEAR &&
				GetDataWareHouse()->GetCurrentState() != STATE_ASLEEP)
				GetDataWareHouse()->SetCurrentState(STATE_FEAR);
			break;

		default:
			if (GetDataWareHouse()->GetCurrentState() != STATE_FEAR &&
				GetDataWareHouse()->GetCurrentState() != STATE_ASLEEP && GetDataWareHouse()->GetCurrentState() != STATE_PASSIVE)
				GetDataWareHouse()->SetCurrentState(STATE_PASSIVE);
			break;
		}
	}
	else if (pOther->GetType() == ENT_ITEM)
		PreformItemBehavaior(pOther);

	else if (pOther->GetType() == ENT_PLAYER) {
		Entity* p = (Entity*)pOther;
		if (GetSightSenseRect().IsIntersecting(p->GetSightEmitRect()) && GetDataWareHouse()->GetCurrentState() != STATE_CURIOUS && GetDataWareHouse()->GetCurrentState() != STATE_ASLEEP)
			GetDataWareHouse()->SetCurrentState(STATE_FEAR);
		else if (GetDataWareHouse()->GetCurrentState() != STATE_CURIOUS && GetDataWareHouse()->GetCurrentState() != STATE_ASLEEP)
			GetDataWareHouse()->SetCurrentState(STATE_ALERT);
		else if (GetDataWareHouse()->GetCurrentState() == STATE_ASLEEP && GetSoundSenseRect().IsIntersecting(pOther->GetRect()))
			GetDataWareHouse()->SetCurrentState(STATE_FEAR);
	}
}


void Rook::PreformItemBehavaior(const IEntity* pOther)
{
	Items* tmpItem = (Items*)pOther;

	if (tmpItem->GetItemType() == Items::INT_BAIT && SuperBait <= 0 && !Asleep){
		Entity* tmp = (Entity*)pOther;
		SetTarget(tmp);
		SetDestination(tmpItem->GetPosition());
		GetDataWareHouse()->SetCurrentState(STATE_CURIOUS);
	}
	else if (tmpItem->GetItemType() == Items::INT_REPELLENT){
		
	}
}

void Rook::UpdateMoveSpeed(){
	switch (this->GetMoveMode()) {

	case MOVE_STAND:
		m_moveSpeed = 0;
		break;

	case MOVE_WALK:
		m_moveSpeed = 80;
		break;

	case MOVE_TROT:
		m_moveSpeed = 180;
		break;

	case MOVE_RUN:
		m_moveSpeed = 325;
		break;

	case MOVE_EXHAUSTED:
		m_moveSpeed = 50;
		break;
	}
}