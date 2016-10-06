#include "Prancer.h"


Prancer::Prancer() {
	SetName("Prancer");
	SetSize({ 48, 48 });
	SetSightEmitSz(GetSize() * 4);
	SetSoundEmitSz(GetSize() * 5);	
	LoadData({ 350, 350 }, { 475, 475 }, { 250, 250 }, Camera::GetInstance()->GetMapLayer()->FindSpawnPoint(GetName()));

	SetHealth(110);
	
	m_soundTimer = 3;
	m_aSound = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/Prancer.wav");
	AnimationSystem::GetInstance()->Load("Prancer", "Prancer");
}


Prancer::~Prancer() {
	SGD::AudioManager::GetInstance()->UnloadAudio(m_aSound);
}

void Prancer::Update(float elapsedTime) {
	if (!Target)
	{
		ATTACK = false;
		RUNNIGGA = false;
	}

	this->GetSenses()->ProjectSight(this);
	SGD::Vector  TempVec = { 0, 0 };
	SGD::Point NextPos = this->GetPosition();

	if (attacktime > 0)
		attacktime -= elapsedTime;
	else if (attacktime < 0 && TAUNT == true){
		TAUNT = false;
		this->SetMoveMode(MOVE_STAND);
	}

	if (this->GetSenses()->CheckProximity(this) == Senses::CAN_HEAR && !ATTACK && !TAUNT){
		RUNNIGGA = true;
		this->SetMoveMode(MOVE_WALK);
	}
	else if (this->GetSenses()->CheckProximity(this) == Senses::VISIBLE && attacktime <= 0){
		ATTACK = true;
		RUNNIGGA = false;
		this->SetMoveMode(MOVE_RUN);
	}

	if (RUNNIGGA){
		TempVec = this->GetTarget()->GetPosition() - this->GetPosition();
		TempVec = TempVec.ComputeNormalized();
		TempVec = TempVec * (float)(GetMoveSpeed()) * elapsedTime;
	}
	else if (ATTACK){
		TempVec = this->GetTarget()->GetPosition() - this->GetPosition();
		TempVec = TempVec.ComputeNormalized();
		TempVec = TempVec * (float)(GetMoveSpeed())* elapsedTime;

		if (this->GetTarget()->GetRect().IsIntersecting(this->GetRect())){
			ATTACK = false;
			attacktime = 3.0f;
			this->SetMoveMode(MOVE_TROT);
			RUNNIGGA = false;
			TAUNT = true;
		}
	}


	TempVec = Zoolander(TempVec);

	NextPos.x += TempVec.x;
	NextPos.y += TempVec.y;
	m_vecDirection = TempVec;
	this->SetPosition(NextPos);

	if (TAUNT)
		this->GetTimestamp()->SetAnimation("Taunt");
	Entity::Update(elapsedTime);
	if (m_soundTimer < 0)
	{

		m_soundTimer = 5;
	}
}

void Prancer::Render(SGD::Point bawlz)
{
	Entity::Render(bawlz);
}

void Prancer::HandleCollision(const IEntity* pOther)
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

void Prancer::PreformItemBehavaior(const IEntity* pOther)
{
	Items* tmpItem = (Items*)pOther;

	if (tmpItem->GetItemType() == Items::INT_BAIT){
		//	If i come near a bait -- do this
	}
	else if (tmpItem->GetItemType() == Items::INT_REPELLENT){
		//	If i come near a repellant -- do this
	}
}

void Prancer::UpdateMoveSpeed(){
	switch (this->GetMoveMode()) {

	case MOVE_STAND:
		m_moveSpeed = 0;
		break;

	case MOVE_WALK:
		m_moveSpeed = 85;
		break;

	case MOVE_TROT:
		m_moveSpeed = 165;
		break;

	case MOVE_RUN:
		m_moveSpeed = 265;
		break;

	case MOVE_EXHAUSTED:
		m_moveSpeed = 70;
		break;
	}
}