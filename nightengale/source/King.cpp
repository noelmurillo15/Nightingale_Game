#include "King.h"


King::King() {
	SetName("King");
	SetSize({ 80, 80 });
	SetSightEmitSz(GetSize() * 4);
	SetSoundEmitSz(GetSize() * 5);
	LoadData({ 300, 300 }, { 450, 450 }, { 550, 550 }, Camera::GetInstance()->GetMapLayer()->FindSpawnPoint(GetName()));
	
	m_fHuntTimer = 3.0f;
	m_fPassiveTimer = 4.0f;
	GetDataWareHouse()->SetCurrentState(STATE_PASSIVE);
	wanderDestination = Camera::GetInstance()->GetMapLayer()->FindRandomBush();
	m_fItemTimer = 4.0f;
	m_iDamage = 45;
	SetHealth(200);
	m_fAttentionSpan = 5.0f; 

	m_aSound = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/KingGrowl.wav");
	m_hAggroNoise = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/KingAggroSFX.wav");
	m_hCuriousNoise = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/KingCuriousSFX.wav");
	m_hPassiveNoise = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/KingPassiveSFX.wav");
	AnimationSystem::GetInstance()->Load("King", "King");
}

King::~King() 
{
	SGD::AudioManager::GetInstance()->UnloadAudio(m_aSound);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hAggroNoise); 
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hCuriousNoise); 
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hPassiveNoise); 
}

void King::Update(float elapsedTime) 
{
	if (!Target)
		this->GetDataWareHouse()->SetCurrentState(STATE_PASSIVE);

	SGD::Vector tempVec = SGD::Vector(0, 0); 
	SGD::Point nextPos = this->GetPosition(); 

	this->GetSenses()->ProjectSight(this); 
	this->GetSenses()->UpdateTimer(elapsedTime); 

	if (this->GetDataWareHouse()->GetCurrentState() == STATE_PASSIVE && wanderDestination == SGD::Point(-10.0f, -10.0f))
	{
		wanderDestination = Camera::GetInstance()->GetMapLayer()->FindRandomBush(); 
		this->SetMoveMode(MOVE_WALK);
	}
	else if (wanderDestination != SGD::Point(-10.0f, -10.0f)){
		tempVec = this->GetPosition() - wanderDestination;
		tempVec = tempVec.ComputeNormalized();
		tempVec = tempVec * (float)(GetMoveSpeed()) * elapsedTime;
	}
	if (abs(this->GetPosition().ComputeDifference(wanderDestination).x) < 5 && abs(this->GetPosition().ComputeDifference(wanderDestination).y) < 5)
	{
		wanderDestination = { -10.0f, -10.0f };
	}

	if (attacktime > 0)
		attacktime -= elapsedTime;
	else if (attacktime < 0 && GetDataWareHouse()->GetCurrentState() == STATE_AGGRO && GetSenses()->GetSensesTimer() < 0)
		GetDataWareHouse()->SetCurrentState(STATE_PASSIVE);

	if (GetSenses()->CheckProximity(this) == Senses::CAN_HEAR && GetDataWareHouse()->GetCurrentState() != STATE_AGGRO && GetDataWareHouse()->GetCurrentState() != STATE_CURIOUS)
		GetDataWareHouse()->SetCurrentState(STATE_FEAR);

	else if (GetSenses()->CheckProximity(this) == Senses::VISIBLE && attacktime <= 0 && GetDataWareHouse()->GetCurrentState() != STATE_CURIOUS)
		GetDataWareHouse()->SetCurrentState(STATE_AGGRO);

		////Check for anything within proximity to the king
		//if (this->GetSenses()->CheckProximity(this) == this->GetSenses()->VISIBLE ||
		//	this->GetSenses()->CheckProximity(this) == this->GetSenses()->CAN_HEAR)
		//{
		//	//If target is a king, queen, or plant
		//	if (GetTarget()->GetCreature() == CRT_KING ||
		//		GetTarget()->GetCreature() == CRT_QUEEN ||
		//		GetTarget()->GetCreature() == CRT_CASTLE)
		//	{
		//		//Doesn't care about it so its state is passive
		//		this->GetDataWareHouse()->SetCurrentState(STATE_PASSIVE);
		//	}
		//	else if (this->GetTarget()->GetType()  == ENT_ITEM)
		//		PerformItemBehavior(this->GetTarget()); 
		//	//Otherwise it gets curious about anything that gets too close
		//	else
		//		this->GetDataWareHouse()->SetCurrentState(STATE_CURIOUS);
		//}

		//if (true == this->GetSenses()->CheckListenCollision(this))//Check sound
		//{
		//	//If the target is a king, queen, or plant
		//	if (GetTarget()->GetCreature() == CRT_KING ||
		//		GetTarget()->GetCreature() == CRT_QUEEN ||
		//		GetTarget()->GetCreature() == CRT_CASTLE)
		//	{
		//		//Then set the target to something else
		//		this->GetDataWareHouse()->SetCurrentState(STATE_PASSIVE);
		//	}
		//	else// if (true)
		//	{
		//		this->GetDataWareHouse()->SetCurrentState(STATE_CURIOUS);
		//	}
		//}


	switch (this->GetDataWareHouse()->GetCurrentState())
	{
	case STATE_CURIOUS:
	{
		this->SetMoveMode(MOVE_WALK);
		if (m_hCuriousNoise != SGD::INVALID_HANDLE) {
			SGD::Vector H = Camera::GetInstance()->GetCameraPosition() - GetPosition();
			float g = ((abs(H.x) + abs(H.y)) / 15);
			float b = 100 - g;
			SGD::AudioManager::GetInstance()->SetAudioVolume(m_hCuriousNoise, (int)b);
			if (m_soundTimer <= 0 && b > 10)
			{
				if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hCuriousNoise))
					SGD::AudioManager::GetInstance()->PlayAudio(m_hCuriousNoise, false);
			}
		}
		tempVec = this->GetTarget()->GetPosition() - this->GetPosition(); 
		tempVec = tempVec.ComputeNormalized(); 
		tempVec = tempVec * (float)(GetMoveSpeed()) * elapsedTime; 

		//if (GetDestination() != SGD::Point(-10.0f, -10.0f)){
		//	tempVec = GetDestination() - GetPosition();
		//
		//	tempVec = tempVec.ComputeNormalized();
		//	tempVec = tempVec * (float)(GetMoveSpeed())* elapsedTime;
		//
		//
		//}
		m_fHuntTimer -= elapsedTime;
		if (abs(this->GetPosition().ComputeDifference(this->GetTarget()->GetPosition()).x) < 50 && 
			abs(this->GetPosition().ComputeDifference(this->GetTarget()->GetPosition()).y) < 50 &&
			m_fHuntTimer <= 0)
		{
			//wanderDestination = { -10.0f, -10.0f };
			this->GetDataWareHouse()->SetCurrentState(STATE_AGGRO);
			m_fHuntTimer = 3.0f;
		}
		break;
	}
	case STATE_PASSIVE:
	{
		if (m_hPassiveNoise != SGD::INVALID_HANDLE) {
			SGD::Vector H = Camera::GetInstance()->GetCameraPosition() - GetPosition();
			float g = ((abs(H.x) + abs(H.y)) / 15);
			float b = 100 - g;
			SGD::AudioManager::GetInstance()->SetAudioVolume(m_hPassiveNoise, (int)b);

			if (m_soundTimer <= 0 && b > 10)
			{
				if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hPassiveNoise))
					SGD::AudioManager::GetInstance()->PlayAudio(m_hPassiveNoise, false);
			}
		}
		this->SetMoveMode(MOVE_WALK);
		tempVec = wanderDestination - this->GetPosition();
		tempVec = tempVec.ComputeNormalized();
		tempVec = tempVec * (float)(GetMoveSpeed()) * elapsedTime;
		break;
	}
	case STATE_AGGRO:
	{
		if (m_hAggroNoise != SGD::INVALID_HANDLE) {
			SGD::Vector H = Camera::GetInstance()->GetCameraPosition() - GetPosition();
			float g = ((abs(H.x) + abs(H.y)) / 15);
			float b = 100 - g;
			SGD::AudioManager::GetInstance()->SetAudioVolume(m_hAggroNoise, (int)b);
			if (m_soundTimer <= 0 && b > 10)
			{
				if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hAggroNoise))
					SGD::AudioManager::GetInstance()->PlayAudio(m_hAggroNoise, false);
			}
		}
		if (this->GetTarget() != NULL)
			tempVec = this->GetTarget()->GetPosition() - this->GetPosition();

		tempVec = tempVec.ComputeNormalized();
		tempVec = tempVec * (float)(GetMoveSpeed())* elapsedTime;
		if (this->GetTarget()->GetRect().IsIntersecting(this->GetRect()))
		{
			attacktime = 4.0f; 
			GetDataWareHouse()->SetCurrentState(STATE_FEAR);
		}
		this->SetMoveMode(MOVE_RUN);
		break;
	}
	case STATE_FEAR:
	{
		if (abs(GetPosition().ComputeDifference(GetDestination()).x) < 5 &&
			abs(GetPosition().ComputeDifference(GetDestination()).y) < 5 ){
			SetDestination({ -10.0f, -10.0f });
			GetDataWareHouse()->SetCurrentState(STATE_PASSIVE);
			break;
		}

		if (GetDestination() == SGD::Point(-10.0f, -10.0f))
			SetDestination(Camera::GetInstance()->GetMapLayer()->FindRandomBush());

		tempVec = GetDestination() - this->GetPosition();
		tempVec = tempVec.ComputeNormalized();
		tempVec *= (float)(GetMoveSpeed()) * elapsedTime;
		this->SetMoveMode(MOVE_TROT);
		break;
	}
	default:
		break;
	}


	tempVec = Zoolander(tempVec);


	nextPos.x += tempVec.x; 
	nextPos.y += tempVec.y; 
	this->SetPosition(nextPos); 
	this->GetSenses()->UpdateTimer(elapsedTime); 
	Entity::Update(elapsedTime);
}

void King::Render(SGD::Point bawlz)
{
	Entity::Render(bawlz);
}

void King::HandleCollision(const IEntity* pOther) {

	if (pOther->GetType() == ENT_ANIMAL) 
	{
		Entity* tmpE = (Entity*)pOther;
		switch (tmpE->GetCreature()) {

		case CRT_BISHOP:
		{
			if (false == SGD::AudioManager::GetInstance()->IsAudioPlaying(m_aSound))
				SGD::AudioManager::GetInstance()->PlayAudio(m_aSound);

			if (this->GetRect().IsIntersecting(tmpE->GetRect()))
				tmpE->SetHealth(tmpE->GetHealth() - m_iDamage);
			break;
		}
		case CRT_BUMBLES:
		{
			if (false == SGD::AudioManager::GetInstance()->IsAudioPlaying(m_aSound))
				SGD::AudioManager::GetInstance()->PlayAudio(m_aSound);
			if (this->GetRect().IsIntersecting(tmpE->GetRect()))
				tmpE->SetHealth(tmpE->GetHealth() - m_iDamage);
			break;
		}
		case CRT_PASSANT:
		{
			if (false == SGD::AudioManager::GetInstance()->IsAudioPlaying(m_aSound))
				SGD::AudioManager::GetInstance()->PlayAudio(m_aSound);
			if (this->GetRect().IsIntersecting(tmpE->GetRect()))
				tmpE->SetHealth(tmpE->GetHealth() - m_iDamage);
			break;
		}
		case CRT_PAWN:
		{
			if (false == SGD::AudioManager::GetInstance()->IsAudioPlaying(m_aSound))
				SGD::AudioManager::GetInstance()->PlayAudio(m_aSound);
			if (this->GetRect().IsIntersecting(tmpE->GetRect()))
				tmpE->SetHealth(tmpE->GetHealth() - m_iDamage);
			break;
		}
		case CRT_PRANCER:
		{
			if (false == SGD::AudioManager::GetInstance()->IsAudioPlaying(m_aSound))
				SGD::AudioManager::GetInstance()->PlayAudio(m_aSound);
			if (this->GetRect().IsIntersecting(tmpE->GetRect()))
				tmpE->SetHealth(tmpE->GetHealth() - m_iDamage);
			break;
		}
		case CRT_ROOK:
		{
			if (false == SGD::AudioManager::GetInstance()->IsAudioPlaying(m_aSound))
				SGD::AudioManager::GetInstance()->PlayAudio(m_aSound);
			if (this->GetRect().IsIntersecting(tmpE->GetRect()))
				tmpE->SetHealth(tmpE->GetHealth() - m_iDamage);
			break;
		}
		default:
			break;
		}
	}
	else if (pOther->GetType() == ENT_ITEM)
		this->PerformItemBehavior(pOther); // do this if I sense an item
	else if (pOther->GetType() == ENT_PLAYER)
		this->GetDataWareHouse()->SetCurrentState(STATE_PASSIVE);
}

void King::PerformItemBehavior(const IEntity* pOther)
{
	Items* tmpItem = (Items*)pOther;
	if (tmpItem->GetItemType() == Items::INT_BAIT)
	{
		SetTarget((Entity*)tmpItem);
		//this->GetDataWareHouse()->SetCurrentState(STATE_CURIOUS);
		SetDestination(tmpItem->GetPosition());
		this->GetDataWareHouse()->SetCurrentState(STATE_CURIOUS);

	}
	else if (tmpItem->GetItemType() == Items::INT_REPELLENT)
	{
		if (GetDestination() == SGD::Point(-10.0f, -10.0f))
			SetDestination(Camera::GetInstance()->GetMapLayer()->FindRandomBush());
		this->GetDataWareHouse()->SetCurrentState(STATE_FEAR);
	}
}

void King::UpdateMoveSpeed()
{
	switch (this->GetMoveMode())
	{
	case MOVE_STAND:
		m_moveSpeed = 0;
		break;
	case MOVE_WALK:
		m_moveSpeed = 125;
		break;
	case MOVE_TROT:
		m_moveSpeed = 170;
		break;
	case MOVE_RUN:
		m_moveSpeed = 200;
		break;
	case MOVE_EXHAUSTED:
		m_moveSpeed = 100;
		break;
	}
}