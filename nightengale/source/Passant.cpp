#include "Passant.h"

#include "PassantTurnMessage.h"



Passant::Passant() {
	SetName("Passant");
	SetSize({ 24, 24 });
	SetSightEmitSz(GetSize() * 4);
	SetSoundEmitSz(GetSize() * 5);
	LoadData({ 1000, 1000 }, { 300, 300 }, { 600, 600 }, Camera::GetInstance()->GetMapLayer()->FindSpawnPoint(GetName()));

	m_iDamage = 51;
	SetHealth(30);
	
	m_aSound = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/PassantFear.wav");
	m_sAttack = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/PassantAttack.wav");
	AnimationSystem::GetInstance()->Load("Passant", "Passant");
}

Passant::~Passant()
{
	if (SecTimeStamp)
		delete SecTimeStamp;
	SGD::AudioManager::GetInstance()->UnloadAudio(m_aSound);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_sAttack);
}

void Passant::Update(float elapsedTime) {


	if (!Target)
		this->GetDataWareHouse()->SetCurrentState(STATE_PASSIVE);
	else if (Target->GetType() == ENT_ITEM)
	{
		Items* p = (Items*)Target;
		if (p->isDying())
		{
			this->GetDataWareHouse()->SetCurrentState(STATE_PASSIVE);
			Target = nullptr;
		}
	}

	SGD::Vector tempVec = SGD::Vector(0, 0);
	SGD::Point nextPos = this->GetPosition();

	if (!Target || Target->GetType() != Entity::ENT_ITEM)
	{

		if (this->GetSenses()->CheckProximity(this) == this->GetSenses()->VISIBLE ||
			this->GetSenses()->CheckProximity(this) == this->GetSenses()->CAN_HEAR)
		{
			//If target is a king, queen, or plant
			if (GetTarget()->GetCreature() == CRT_KING ||
				GetTarget()->GetCreature() == CRT_QUEEN ||
				GetTarget()->GetCreature() == CRT_BUMBLES ||
				GetTarget()->GetCreature() == CRT_PACK)
			{
				//Doesn't care about it so its state is passive
				this->GetDataWareHouse()->SetCurrentState(STATE_FEAR);
			}
			else if (GetTarget()->GetCreature() == CRT_BISHOP ||
				GetTarget()->GetCreature() == CRT_PRANCER ||
				GetTarget()->GetCreature() == CRT_CASTLE)
				this->GetDataWareHouse()->SetCurrentState(STATE_PASSIVE);
			//Otherwise it gets curious about anything that gets too close
			else
			{
				this->GetDataWareHouse()->SetCurrentState(STATE_CURIOUS);
			}
		}

		else if (true == this->GetSenses()->CheckListenCollision(this) && this->GetDataWareHouse()->GetCurrentState() != STATE_FEAR)//Check sound
		{
			//If the target is a king, queen, or plant
			if (GetTarget()->GetCreature() == CRT_KING ||
				GetTarget()->GetCreature() == CRT_QUEEN ||
				GetTarget()->GetCreature() == CRT_BUMBLES ||
				GetTarget()->GetCreature() == CRT_PACK)
			{
				//Then set the target to something else
				this->GetDataWareHouse()->SetCurrentState(STATE_FEAR);
			}
			else
			{
				this->GetDataWareHouse()->SetCurrentState(STATE_AGGRO);
			}
		}
	}

	switch (this->GetDataWareHouse()->GetCurrentState())
	{
	case STATE_FEAR:
	{
					   this->SetMoveMode(MOVE_RUN);
					   if (Target->GetType() == Entity::ENT_ITEM)
					   {
						   Items* T = (Items*)Target;
						   tempVec = T->GetPosition() - m_currPosition;
					   }
					   else
					   tempVec = Target->GetPosition() - this->GetPosition();
					   tempVec = tempVec.ComputeNormalized();
					   tempVec = -tempVec * (float)(GetMoveSpeed()) * elapsedTime;

					   break;
	}
	case STATE_CURIOUS:
	{
						  this->SetMoveMode(MOVE_WALK);
						  tempVec = Target->GetPosition() - this->GetPosition();
						  tempVec = tempVec.ComputeNormalized();
						  tempVec = tempVec * (float)(GetMoveSpeed()) * elapsedTime;


						  break;
	}
	case STATE_PASSIVE:
	{
						  m_soundTimer = 20;
						  this->SetMoveMode(MOVE_WALK);
						  break;
	}
	case STATE_AGGRO:
	{
						this->SetMoveMode(MOVE_RUN);
						if (Target->GetType() == Entity::ENT_ITEM)
						{
							Items* T = (Items*)Target;
							tempVec = T->GetPosition() - m_currPosition;
						}
						else
						tempVec = Target->GetPosition() - m_currPosition;
						tempVec = tempVec.ComputeNormalized();
						tempVec = tempVec * (float)(GetMoveSpeed()) * elapsedTime;

						if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_sAttack))
						{
							SGD::Vector H = Camera::GetInstance()->GetCameraPosition() - GetPosition();
							float g = ((abs(H.x) + abs(H.y)) / 15);
							float b = 100 - g;
							SGD::AudioManager::GetInstance()->SetAudioVolume(m_sAttack, (int)b);

							SGD::AudioManager::GetInstance()->PlayAudio(m_sAttack);
						}
						break;
	}
	default:
		break;
	}
	
	tempVec = Zoolander(tempVec);


	nextPos.x += tempVec.x;
	nextPos.y += tempVec.y;
	m_vecDirection = tempVec;
	this->SetPosition(nextPos);
	this->GetSenses()->UpdateTimer(elapsedTime);
	Entity::Update(elapsedTime);
	if (GetDataWareHouse()->GetCurrentState() != STATE_FEAR)
	{
		m_soundTimer = 20;
	}
	else if (m_soundTimer < 0 || m_soundTimer > 2)
		m_soundTimer = 2;
	if (SecTimeStamp)
	{
		if (AnimationSystem::GetInstance()->GetLoaded()[this->SecTimeStamp->m_SCurrentObject].size() < 8)
			SecTimeStamp->SetAnimation(this->GetDirection_4(m_vecDirection));
		else
			SecTimeStamp->SetAnimation(this->GetDirection_8(m_vecDirection));
		AnimationSystem::GetInstance()->Update(SecTimeStamp, elapsedTime);
	}

}

void Passant::Render(SGD::Point bawlz) {

	Entity::Render(bawlz);

	if (SecTimeStamp)
	{
		AnimationSystem::GetInstance()->Render(SecTimeStamp, GetPosition().x - bawlz.x, GetPosition().y - bawlz.y, { 1, 1 }, { 155, 255, 155 });
	}
	else
		AnimationSystem::GetInstance()->Render(this->GetTimestamp(), GetPosition().x - bawlz.x, GetPosition().y - bawlz.y, { 1, 1 }, { 255, 255, 255 });
}

void Passant::HandleCollision(const IEntity* pOther)
{
	Entity* u = (Entity*)pOther;
	if (pOther->GetType() == ENT_ANIMAL) {
		Entity* tmpE = (Entity*)pOther;

		if (this->GetRect().IsIntersecting(pOther->GetRect())){
			PassantTurnMessage* Msg;
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
				if (!tmpE->GetAlive()){
					Msg = new PassantTurnMessage(this, new AnimationTimestamp(u->GetTimestamp()));
					Msg->QueueMessage();
					Msg = nullptr;
				}

				break;

			case CRT_PRANCER:
				if (!tmpE->GetAlive()){
					Msg = new PassantTurnMessage(this, new AnimationTimestamp(u->GetTimestamp()));
					Msg->QueueMessage();
					Msg = nullptr;
				}
				break;

			case CRT_QUEEN:
				//	creatures shouldn't kill themselves when attack.. attacker should handle that
				break;

			case CRT_ROOK:
				if (!tmpE->GetAlive()){
					Msg = new PassantTurnMessage(this, new AnimationTimestamp(u->GetTimestamp()));
					Msg->QueueMessage();
					Msg = nullptr;
				}
				break;

			default:
				break;
			}
		}
	}
	else if (pOther->GetType() == ENT_PLAYER)
	{
		if (u->GetHealth() <= 0)
		{
		PassantTurnMessage* pMsg = new PassantTurnMessage(this, new AnimationTimestamp( u->GetTimestamp()));
		pMsg->QueueMessage();
		pMsg = nullptr;
		}
	
	}

	else if (pOther->GetType() == ENT_ITEM){
		this->PreformItemBehavaior(pOther); // do this if i sense an item
	}
}

void Passant::PreformItemBehavaior(const IEntity* pOther)
{
	Items* tmpItem = (Items*)pOther;

	if (tmpItem->GetItemType() == Items::INT_BAIT){
		Target = (Entity*)pOther;
		m_destination = tmpItem->GetPosition();
		this->GetDataWareHouse()->SetCurrentState(STATE_AGGRO);
	}
	else if (tmpItem->GetItemType() == Items::INT_REPELLENT){
		Target = (Entity*)pOther;
		m_destination = tmpItem->GetPosition();
		m_ptBrain->currentState = STATE_CURIOUS;
		this->GetDataWareHouse()->SetCurrentState(STATE_FEAR);
	}
}

void Passant::UpdateMoveSpeed(){
	switch (this->GetMoveMode()) {

	case MOVE_STAND:
		m_moveSpeed = 0;
		break;

	case MOVE_WALK:
		m_moveSpeed = 80;
		break;

	case MOVE_TROT:
		m_moveSpeed = 170;
		break;

	case MOVE_RUN:
		m_moveSpeed = 300;
		break;

	case MOVE_EXHAUSTED:
		m_moveSpeed = 50;
		break;
	}
}