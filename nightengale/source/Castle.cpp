#include "Castle.h"

#include "AnimationSystem.h"


Castle::Castle() {
	SetName("Castle");
	SetSize({ 28, 28 });
	SetSightEmitSz(GetSize() * 4);
	SetSoundEmitSz(GetSize() * 5);
	LoadData({ 1,1 }, { 1,1 }, { 140, 140 }, Camera::GetInstance()->GetMapLayer()->FindSpawnPoint(GetName()));

	SetHealth(100);

	m_iDamage = 100;
	m_iDevourCount = 1;
	m_bCanGrow = false;
	GetDataWareHouse()->SetCurrentState(STATE_AGGRO);
	
	AnimationSystem::GetInstance()->Load("Castle", "Castle");
	SecTimeStamp = new AnimationTimestamp("Castle", "Idle");
	m_hRevealScream = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/NightengaleCastleRevealedSFX.wav"); 	
}


Castle::~Castle()
{
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hRevealScream); 
	delete SecTimeStamp;
}

void Castle::Update(float elapsedTime)
{	

#pragma region Set own In Hiding
	if (m_iDevourCount > 2){
		m_bIsRevealed = true;
		m_ReavealedTimer = 10.0f;
	}

	if (m_ReavealedTimer > 0){
		m_ReavealedTimer -= elapsedTime;
		SetAlpha(255);
		SetInHiding(false);
	}
	else{
		SetAlpha(0);
		m_bIsRevealed = false;
		SetInHiding(true);
	}
#pragma endregion

#pragma region Grow!
	if (m_bCanGrow) {
		SetSize({ (float)m_iDevourCount * 28, (float)m_iDevourCount * 28 });
		SetSightEmitSz(GetSize() * 4);
		SetSoundEmitSz(GetSize() * 5);
		SetInitialPosition({ GetPosition().x - 14, GetPosition().y - 14 });
		m_bCanGrow = false; 
	}
#pragma endregion

	Entity::Update(elapsedTime);
}

void Castle::Render(SGD::Point bawlz)
{
	Entity::Render(bawlz);

	if (m_bIsRevealed )
		AnimationSystem::GetInstance()->Render(SecTimeStamp, GetPosition().x - bawlz.x, GetPosition().y - bawlz.y, { (float)m_iDevourCount, (float)m_iDevourCount }, { 155, 255, 155 });
}

void Castle::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_ANIMAL) 
	{
		Entity* tmpE = (Entity*)pOther;

		if (this->GetRect().IsIntersecting(tmpE->GetRect()))
		{
			switch (tmpE->GetCreature()) 
			{
			case CRT_BISHOP:
			{
				if (this->GetRect().IsIntersecting(pOther->GetRect()))
				{
					if (tmpE->GetHealth() - m_iDamage <= 0)
					{
						if (m_iDevourCount <= 4)
						{
							m_iDevourCount++;
							m_bCanGrow = true;
						}
					}
					tmpE->SetHealth(tmpE->GetHealth() - m_iDamage);
					break;
				}
			}
			case CRT_BUMBLES:
			{
				if (this->GetRect().IsIntersecting(pOther->GetRect()))
				{
					if (tmpE->GetHealth() - m_iDamage <= 0)
					{
						if (m_iDevourCount <= 4)
						{
							m_iDevourCount++;
							m_bCanGrow = true;
						}
					}
					tmpE->SetHealth(tmpE->GetHealth() - m_iDamage);
				}
				break;
			}
			case CRT_PASSANT:
			{
				if (this->GetRect().IsIntersecting(pOther->GetRect()))
				{
					if (tmpE->GetHealth() - m_iDamage <= 0)
					{
						if (m_iDevourCount <= 4)
						{
							m_iDevourCount++;
							m_bCanGrow = true;
						}
					}
					tmpE->SetHealth(tmpE->GetHealth() - m_iDamage);
				break;
				}
			}
			case CRT_PAWN:
			{
				if (this->GetRect().IsIntersecting(pOther->GetRect()))
				{
					if (tmpE->GetHealth() - m_iDamage <= 0)
					{
						if (m_iDevourCount <= 4)
						{
							m_iDevourCount++;
							m_bCanGrow = true;
						}
					}
					tmpE->SetHealth(tmpE->GetHealth() - m_iDamage);
				break;
				}
			}
			case CRT_PRANCER:
			{
				if (this->GetRect().IsIntersecting(pOther->GetRect()))
				{
					if (tmpE->GetHealth() - m_iDamage <= 0)
					{
						if (m_iDevourCount <= 4)
						{
							m_iDevourCount++;
							m_bCanGrow = true;
						}
					}
					tmpE->SetHealth(tmpE->GetHealth() - m_iDamage);
				break;
				}
			}
			case CRT_ROOK:
			{
				if (this->GetRect().IsIntersecting(pOther->GetRect()))
				{
					if (tmpE->GetHealth() - m_iDamage <= 0)
					{
						if (m_iDevourCount <= 4)
						{
							m_iDevourCount++;
							m_bCanGrow = true;
						}
					}
					tmpE->SetHealth(tmpE->GetHealth() - m_iDamage);
				break;
				}
			}
			default:
				break;
			}
		}
	}
	else if (pOther->GetType() == ENT_ITEM)
	{
		this->PreformItemBehavaior(pOther); // do this if i sense an item
	}
	else if (pOther->GetType() == ENT_PLAYER)
	{
		if (m_iDevourCount <= 4)
		{
			m_iDevourCount++;
			m_bCanGrow = true;						
		}
		Entity* p = (Entity*)pOther;
		p->SetHealth(p->GetHealth() - m_iDamage);
	}
}

void Castle::PreformItemBehavaior(const IEntity* pOther)
{	
	if (!m_bIsRevealed){
		m_ReavealedTimer = 7.5f;
		m_bIsRevealed = true;
	}
	if (false == SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hRevealScream))
		SGD::AudioManager::GetInstance()->PlayAudio(m_hRevealScream, false);
}

void Castle::UpdateMoveSpeed(){
	switch (this->GetMoveMode()) {
	case MOVE_STAND:
		m_moveSpeed = 0;// = infinity
		break;
	default:
		break;
	}
}