#include "Bishop.h"


struct BishopDirt
{
	float timer = 2.0f; 
	SGD::Point pos; 
};

Bishop::Bishop() {
	SetName("Bishop");
	SetSize({ 32, 32 });
	SetSightEmitSz(GetSize() * 4);
	SetSoundEmitSz(GetSize() * 5);
	LoadData({ 300, 300 }, { 50, 50 }, { 30, 30 }, Camera::GetInstance()->GetMapLayer()->FindSpawnPoint(GetName()));

	SetHealth(60);	
	wanderTimer = 3.0f; 
	m_iDamage = 20;
	m_fFearTimer = 4.0f; 	
	m_fDirtTimer = 0.001f; 
	m_bAttacking = false; 
	GetDataWareHouse()->SetCurrentState(STATE_PASSIVE);
	wanderDestination = Camera::GetInstance()->GetMapLayer()->FindRandomBush();

	m_hBishopDirtTrail = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/BishopDirtTrail.png");
	AnimationSystem::GetInstance()->Load("Bishop", "Bishop");
}


Bishop::~Bishop()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hBishopDirtTrail); 
}

void Bishop::Update(float elapsedTime)
{
	if (!Target)
		this->GetDataWareHouse()->SetCurrentState(STATE_PASSIVE);
	//this->GetSenses()->ProjectSight(this); 
	SGD::Vector tempVec = SGD::Vector(0, 0); 
	SGD::Point nextPos = this->GetPosition(); 

	m_fDirtTimer -= elapsedTime; 
	if (m_fDirtTimer <= 0)
	{
		m_fDirtTimer = 0.001f;
		BishopDirt dirt;
		dirt.pos = this->GetPosition(); 
		DirtPath.push_back(dirt); 
	}

	for (unsigned int i = 0; i < DirtPath.size(); i++)
	{
		DirtPath[i].timer -= elapsedTime; 
		if (DirtPath[i].timer <= 0)
			DirtPath.erase(DirtPath.begin() + i); 
	}

	if (this->GetDataWareHouse()->GetCurrentState() == STATE_PASSIVE && wanderDestination == SGD::Point(-10.0f, -10.0f) && wanderTimer <= 0)
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
		wanderTimer = 3.0f;
	}

	//Check for anything within proximity to the bishop
	if (this->GetSenses()->CheckProximity(this) == this->GetSenses()->VISIBLE ||
		this->GetSenses()->CheckProximity(this) == this->GetSenses()->CAN_HEAR)
	{
		//If target is a king, queen, or plant
		if (GetTarget()->GetCreature() == CRT_KING   ||
			GetTarget()->GetCreature() == CRT_QUEEN  ||
			GetTarget()->GetCreature() == CRT_CASTLE ||
			GetTarget()->GetCreature() == CRT_PASSANT)
		{
			//Is afraid of the larger predators, spiders, and plants
			this->GetDataWareHouse()->SetCurrentState(STATE_FEAR);
		}
		else if (GetTarget()->GetCreature() == CRT_BISHOP)
		{
			//Is passive towards its own species
			this->GetDataWareHouse()->SetCurrentState(STATE_PASSIVE);
		}
		//Otherwise it gets curious about anything that gets too close
		else
			this->GetDataWareHouse()->SetCurrentState(STATE_CURIOUS);

	}

	switch (this->GetDataWareHouse()->GetCurrentState())
	{
	case STATE_AGGRO:
	{
		this->SetMoveMode(MOVE_RUN); 
		tempVec = this->GetTarget()->GetPosition() - this->GetPosition();
		tempVec = tempVec.ComputeNormalized();
		tempVec = tempVec * (float)(GetMoveSpeed()) * elapsedTime;
		wanderTimer -= elapsedTime; 
		if (wanderTimer <= 0)
		{
			wanderTimer = 3.0f; 
			this->GetDataWareHouse()->SetCurrentState(STATE_PASSIVE);
		}
		break;
	}
	case STATE_CURIOUS:
	{
		this->SetMoveMode(MOVE_WALK);
		tempVec = this->GetTarget()->GetPosition() - this->GetPosition();
		tempVec = tempVec.ComputeNormalized();
		tempVec = tempVec * (float)(GetMoveSpeed()) * elapsedTime;
		wanderTimer -= elapsedTime;
		if (wanderTimer <= 0)
		{
			wanderTimer = 3.0f; 
			this->GetDataWareHouse()->SetCurrentState(STATE_AGGRO);
		}
		break;
	}
	case STATE_FEAR: 
	{
		this->SetMoveMode(MOVE_RUN);
		tempVec = wanderDestination - this->GetPosition();
		tempVec = tempVec.ComputeNormalized();
		tempVec = tempVec * (float)(GetMoveSpeed()) * elapsedTime;
		m_fFearTimer -= elapsedTime; 
		if (m_fFearTimer <= 0)
		{
			m_fFearTimer = 4.0f; 
			this->GetDataWareHouse()->SetCurrentState(STATE_PASSIVE);
		}
		break;
	}
	case STATE_PASSIVE:
	{
		this->SetMoveMode(MOVE_WALK);
		tempVec = wanderDestination - this->GetPosition();
		tempVec = tempVec.ComputeNormalized();
		tempVec = tempVec * (float)(GetMoveSpeed()) * elapsedTime;
		break;
	}
	default:
		break;
	}

	if (true == wanderDestination.IsPointInRectangle(this->GetRect()))
		wanderDestination = Camera::GetInstance()->GetMapLayer()->FindRandomBush();


	tempVec = Zoolander(tempVec);


	nextPos.x += tempVec.x;
	nextPos.y += tempVec.y;
	this->SetPosition(nextPos);
	this->GetSenses()->UpdateTimer(elapsedTime);
	Entity::Update(elapsedTime);
}

void Bishop::Render(SGD::Point bawlz)
{
	if (true == m_bAttacking)
	{
		Entity::Render(bawlz);
		m_bAttacking = false; 
	}
	for (unsigned int i = 0; i < DirtPath.size(); i++)
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hBishopDirtTrail, SGD::Point(DirtPath[i].pos.x - bawlz.x, DirtPath[i].pos.y - bawlz.y));
}

void Bishop::HandleCollision(const IEntity* pOther)
{
	
	if (pOther->GetType() == ENT_ANIMAL)
	{
		Entity* tmpE = (Entity*)pOther;
		switch (tmpE->GetCreature())
		{
		case CRT_BISHOP:
		{
			m_bAttacking = true;
			this->GetDataWareHouse()->SetCurrentState(STATE_PASSIVE);
			if (this->GetRect().IsIntersecting(tmpE->GetRect()))
				tmpE->SetHealth(tmpE->GetHealth() - this->m_iDamage); 
			break;
		}
		case CRT_BUMBLES:
		{
			this->GetDataWareHouse()->SetCurrentState(STATE_FEAR);
			if (this->GetRect().IsIntersecting(tmpE->GetRect()))
				tmpE->SetHealth(tmpE->GetHealth() - this->m_iDamage);
			break;
		}
		case CRT_CASTLE:
		{
			this->GetDataWareHouse()->SetCurrentState(STATE_PASSIVE);
			if (this->GetRect().IsIntersecting(tmpE->GetRect()))
				tmpE->SetHealth(tmpE->GetHealth() - this->m_iDamage);
			break;
		}
		case CRT_KING:
		{
			this->GetDataWareHouse()->SetCurrentState(STATE_FEAR); 
			if (this->GetRect().IsIntersecting(tmpE->GetRect()))
				tmpE->SetHealth(tmpE->GetHealth() - this->m_iDamage);
			break;
		}
		case CRT_PASSANT:
		{
			m_bAttacking = true;
			this->GetDataWareHouse()->SetCurrentState(STATE_FEAR);
			if (this->GetRect().IsIntersecting(tmpE->GetRect()))
				tmpE->SetHealth(tmpE->GetHealth() - this->m_iDamage);
			break;
		}
		case CRT_PAWN:
		{
			m_bAttacking = true;
			this->GetDataWareHouse()->SetCurrentState(STATE_AGGRO);
			if (this->GetRect().IsIntersecting(tmpE->GetRect()))
				tmpE->SetHealth(tmpE->GetHealth() - this->m_iDamage);
			this->SetTarget(tmpE); 
			break;
		}
		case CRT_PRANCER:
		{
			m_bAttacking = true;
			this->GetDataWareHouse()->SetCurrentState(STATE_PASSIVE);
			if (this->GetRect().IsIntersecting(tmpE->GetRect()))
				tmpE->SetHealth(tmpE->GetHealth() - this->m_iDamage);
			break;
		}
		case CRT_QUEEN:
		{
			this->GetDataWareHouse()->SetCurrentState(STATE_FEAR); 
			if (this->GetRect().IsIntersecting(tmpE->GetRect()))
				tmpE->SetHealth(tmpE->GetHealth() - this->m_iDamage);
			break;
		}
		case CRT_ROOK:
		{
			m_bAttacking = true;
			this->GetDataWareHouse()->SetCurrentState(STATE_AGGRO);
			if (this->GetRect().IsIntersecting(tmpE->GetRect()))
				tmpE->SetHealth(tmpE->GetHealth() - this->m_iDamage);
			this->SetTarget(tmpE);
			break;
		}

		default:
			break;
		}
	}	
	else if (pOther->GetType() == ENT_ITEM)
	{
		this->PerformItemBehavior(pOther); // do this if I sense an item
	}
}

void Bishop::PerformItemBehavior(const IEntity* pOther)
{
	Items* tmpItem = (Items*)pOther;

	if (tmpItem->GetItemType() == Items::INT_BAIT)
	{
		this->SetTarget((Entity*)pOther); 
		this->GetDataWareHouse()->SetCurrentState(STATE_CURIOUS); 
	}
	else if (tmpItem->GetItemType() == Items::INT_REPELLENT)
		this->GetDataWareHouse()->SetCurrentState(STATE_FEAR); 
}

void Bishop::UpdateMoveSpeed()
{
	switch (this->GetMoveMode()) 
	{
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
		m_moveSpeed = 200;
		break;

	case MOVE_EXHAUSTED:
		m_moveSpeed = 45;
		break;
	}
}