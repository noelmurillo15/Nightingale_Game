#include "Pawn.h"

#include "PassantTurnMessage.h"



Pawn::Pawn() {
	SetName("Pawn");
	SetSize({ 24, 24 });
	SetSightEmitSz(GetSize() * 4);
	SetSoundEmitSz(GetSize() * 5);
	LoadData({ 500, 500 }, { 375, 375 }, { 525, 525 }, Camera::GetInstance()->GetMapLayer()->FindSpawnPoint(GetName()));

	SetHealth(50);
	m_soundTimer = 3;
	baitsneededforlovemode = rand() % 2 + 2;

	heart = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/Heart.png");
	AnimationSystem::GetInstance()->Load("Pawn", "Pawn");
	m_aSound = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/PawnChirp.wav");
}

Pawn::~Pawn() {
	SGD::AudioManager::GetInstance()->UnloadAudio(m_aSound);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(heart);
}

void Pawn::Update(float elapsedTime) {

	SGD::Vector TempVec = { 0, 0 };
	SGD::Point NextPos = GetPosition();
	
	if (m_soundTimer < -0.5)
		m_soundTimer = 3;

	if (stuffedtimer >= 0)
		stuffedtimer -= elapsedTime;
	else if (isstuffed && stuffedtimer <= 0)
		isstuffed = false;
	
#pragma region Fear & LoveMode
	if (GetSenses()->CheckProximity(this) == Senses::CAN_HEAR && GetDataWareHouse()->GetCurrentState() != STATE_CURIOUS)
		GetDataWareHouse()->SetCurrentState(STATE_FEAR);		
	
	if (GetTarget() != NULL && baitsneededforlovemode == 0){
		if (GetTarget()->GetType() == ENT_PLAYER)
			GetDataWareHouse()->SetCurrentState(STATE_PAWN_LOVE_MODE);
	}
#pragma endregion

	switch (GetDataWareHouse()->GetCurrentState())  {

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

	case STATE_CURIOUS:
#pragma region Curious
		if (GetDestination() != SGD::Point(-10.0f, -10.0f)){
			TempVec = GetDestination() - GetPosition();

			TempVec = TempVec.ComputeNormalized();
			TempVec = TempVec * (float)(GetMoveSpeed())* elapsedTime;

			if (abs(GetPosition().ComputeDifference(GetDestination()).x) < 48 &&
				abs(GetPosition().ComputeDifference(GetDestination()).y) < 48) {

				if (GetTarget() != NULL){
					if (GetTarget()->GetType() != ENT_ANIMAL){
						baitsneededforlovemode--;
						isstuffed = true;
						stuffedtimer = 10.0f;
					}
				}

				SetDestination({ -10.0f, -10.0f });
				GetDataWareHouse()->SetCurrentState(STATE_PASSIVE);
			}
		}
		else{
			if (GetTarget() != NULL)
				SetDestination(GetTarget()->GetPosition());
		}
		if (GetTarget() != NULL && GetTarget()->GetType() == ENT_ANIMAL)
			SetMoveMode(MOVE_WALK);
		else
			SetMoveMode(MOVE_TROT);
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

	case STATE_PAWN_LOVE_MODE:
#pragma region Love Mode
		if (GetTarget() != NULL){
			if (abs(GetTarget()->GetPosition().ComputeDifference(GetPosition()).x) > 72 ||
				abs(GetTarget()->GetPosition().ComputeDifference(GetPosition()).y) > 72 ){
				TempVec = GetTarget()->GetPosition() - GetPosition();
				TempVec = TempVec.ComputeNormalized();
				TempVec = TempVec * (float)(GetMoveSpeed())* elapsedTime;
			}
		}

		SetMoveMode(MOVE_TROT);
#pragma endregion
		break;

	default:
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

void Pawn::Render(SGD::Point bawlz) {

	if (GetDataWareHouse()->GetCurrentState() == STATE_PAWN_LOVE_MODE && GetAlive()){
		SGD::Point pos = GetPosition();
		pos.x -= bawlz.x;
		pos.y -= bawlz.y + 48;
		SGD::GraphicsManager::GetInstance()->DrawTexture(heart, pos, 0.0f, {}, {}, { .65f, .65f });
	}
	Entity::Render(bawlz);
}

void Pawn::HandleCollision(const IEntity* pOther) {

	if (pOther->GetType() == ENT_ANIMAL) {
		Entity* tmpE = (Entity*)pOther;
		switch (tmpE->GetCreature()) {

		case CRT_BISHOP:
			GetDataWareHouse()->SetCurrentState(STATE_FEAR);
			break;

		case CRT_BUMBLES:
			GetDataWareHouse()->SetCurrentState(STATE_FEAR);
			break;

		case CRT_CASTLE:
			SetTarget(tmpE);
			GetDataWareHouse()->SetCurrentState(STATE_CURIOUS);
			break;

		case CRT_KING:
			GetDataWareHouse()->SetCurrentState(STATE_FEAR);
			break;

		case CRT_PASSANT:
			SetTarget(tmpE);
			GetDataWareHouse()->SetCurrentState(STATE_CURIOUS);
			break;

		case CRT_PAWN:
			break;

		case CRT_PRANCER:
			break;

		case CRT_QUEEN:
			GetDataWareHouse()->SetCurrentState(STATE_FEAR);
			break;

		case CRT_ROOK:
			break;

		default:
			break;
		}
	}
	else if (pOther->GetType() == ENT_ITEM)
		PreformItemBehavaior(pOther);

	else if (pOther->GetType() == ENT_PLAYER) {
		if (GetDataWareHouse()->GetCurrentState() == STATE_PASSIVE){
			Entity* p = (Entity*)pOther;
			SetTarget(p);
			if (GetRect().IsIntersecting(pOther->GetRect())){
				baitsneededforlovemode = rand() % 3 + 2;
				GetDataWareHouse()->SetCurrentState(STATE_FEAR);
			}
		}
	}
}

void Pawn::PreformItemBehavaior(const IEntity* pOther) {

	Items* tmpItem = (Items*)pOther;

	if (tmpItem->GetItemType() == Items::INT_BAIT)
	{
		if (!isstuffed){
			Entity* tmp = (Entity*)pOther;
			SetTarget(tmp);
			SetDestination(tmpItem->GetPosition());
			GetDataWareHouse()->SetCurrentState(STATE_CURIOUS);
		}
	}
	else if (tmpItem->GetItemType() == Items::INT_REPELLENT && GetDataWareHouse()->GetCurrentState() != STATE_FEAR){
		SetDestination(Camera::GetInstance()->GetMapLayer()->FindRandomBush());
		baitsneededforlovemode = rand() % 3 + 1;
		GetDataWareHouse()->SetCurrentState(STATE_FEAR);
	}
}

void Pawn::UpdateMoveSpeed(){
	switch (GetMoveMode()) {

	case MOVE_STAND:
		m_moveSpeed = 0;
		break;

	case MOVE_WALK:
		m_moveSpeed = 80;
		break;

	case MOVE_TROT:
		m_moveSpeed = 145;
		break;

	case MOVE_RUN:
		m_moveSpeed = 240;
		break;

	case MOVE_EXHAUSTED:
		m_moveSpeed = 50;
		break;
	}
}