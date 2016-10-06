#include "Queen.h"

#include "EntityManager.h"
#include "GameplayState.h"


Queen::Queen() {
	SetName("Queen");
	SetSize({ 96, 96 });
	SetSightEmitSz(GetSize() * 8);
	SetSoundEmitSz(GetSize() * 0);
	LoadData({ 0, 0 }, { 3000, 3000 }, { 0, 0 }, Camera::GetInstance()->GetMapLayer()->FindSpawnPoint(GetName()));

	SetHealth(1000);
	
	SetAlpha(155);
	m_iDamage = 60;
	m_soundTimer = 3;
	LandTimer = 20;

	m_aSound = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/Queen.wav");
	m_sAttack = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/QueenAttack.wav");
	AnimationSystem::GetInstance()->Load("Queen", "Queen");
}

Queen::~Queen()
{
	SGD::AudioManager::GetInstance()->UnloadAudio(m_aSound);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_sAttack);
}

void Queen::Update(float elapsedTime)
{
	if (Camera::GetInstance()->GetMapLayer()->FindEventTiles("QueensLanding").size() > 0)
	LandTimer -= elapsedTime;
	EntityManager* m_pLiasion = GameplayState::GetInstance()->GetManager();
	Entity* _TempPlayer = (Entity*)m_pLiasion->m_tEntities[Entity::ENT_PLAYER][0];


	if (!Target)
		this->GetDataWareHouse()->SetCurrentState(STATE_PASSIVE);
	else if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_sAttack))
		SGD::AudioManager::GetInstance()->PlayAudio(m_sAttack);


	if (currAction == CON_FLY || currAction == CON_REST && this->GetDataWareHouse()->GetCurrentState() != STATE_FEAR && this->GetDataWareHouse()->GetCurrentState() != STATE_AGGRO)
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
				this->GetDataWareHouse()->SetCurrentState(STATE_PASSIVE);
			}
			else if (GetTarget()->GetCreature() == CRT_BISHOP ||
				GetTarget()->GetCreature() == CRT_PRANCER ||
				GetTarget()->GetCreature() == CRT_CASTLE)
				this->GetDataWareHouse()->SetCurrentState(STATE_PASSIVE);
			//Otherwise it gets curious about anything that gets too close
			else
			{
				if (currAction == CON_REST)
				{
					if (Target->GetMoveMode() == MOVE_RUN || abs(Target->GetPosition().ComputeDifference(m_currPosition).x) < 250 && abs(Target->GetPosition().ComputeDifference(m_currPosition).y) < 250)
					{
						currAction = CON_FLY;
						this->GetDataWareHouse()->SetCurrentState(STATE_AGGRO);
						LandTimer = 20;
					}
				}
				else
				{
					currAction = CON_FLY;
					this->GetDataWareHouse()->SetCurrentState(STATE_AGGRO);
					LandTimer = 5;
				}
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
				this->GetDataWareHouse()->SetCurrentState(STATE_PASSIVE);
			}
			else
			{
				if (currAction == CON_REST && Target && Target->GetMoveMode() == MOVE_RUN)
				{
					int g = currAction;
				}
				else
				{
					currAction = CON_FLY;

					this->GetDataWareHouse()->SetCurrentState(STATE_AGGRO);
					LandTimer = 5;
					
				}
			}
		}
	}

	SGD::Vector TempVec = SGD::Vector(0, 0);
	SGD::Point nextPos = this->GetPosition();
	switch (this->GetDataWareHouse()->GetCurrentState())
	{

	case STATE_AGGRO:
	{

						SGD::Point TargetPos = Target->GetPosition();
						this->SetMoveMode(MOVE_RUN);
						if (direction.x > 0)
						{
							if (TargetPos.x - m_currPosition.x > 100)
								TempVec = this->GetTarget()->GetPosition() - this->GetPosition();
							else
							{
								TempVec = direction;
								Target = nullptr;
							}
						}
						else
						{
							if (m_currPosition.x - TargetPos.x > 100)
								TempVec = this->GetTarget()->GetPosition() - this->GetPosition();
							else
							{
								TempVec = direction;
								Target = nullptr;
							}
						}

						break;
	}
	case STATE_PASSIVE:
		TempVec = direction;
		SetMoveMode(MOVE_TROT);
	default:
		TempVec = direction;
		break;
	}







	if (m_currPosition.x < -2000 && direction.x < 0)
	{
		SGD::Point tmp = GetPosition();
		if (LandTimer < 0)
		{

			m_currPosition.y = Camera::GetInstance()->GetMapLayer()->FindEventTiles("QueensLanding")[0].top + 90;

		}
		else
			m_currPosition.y = rand() % (int)Camera::GetInstance()->GetWorldSize().height + 1.0f;
		direction = { 1, 0 };
		SecDir = direction;
		m_vecDirection = direction;
	}
	else if (m_currPosition.x > (Camera::GetInstance()->GetWorldSize().width + 2000) && direction.x == 1)
	{
		SGD::Point tmp = GetPosition();
		if (LandTimer < 0){

			m_currPosition.y = Camera::GetInstance()->GetMapLayer()->FindEventTiles("QueensLanding")[0].top + 90;

		}
		else
			m_currPosition.y = rand() % (int)Camera::GetInstance()->GetWorldSize().height + 1.0f;
		direction = { -1, 0 };
		SecDir = direction;
		m_vecDirection = direction;
	}

	if (TempVec.x < 0 && direction.x > 0)
		TempVec = direction;
	else if (TempVec.x > 0 && direction.x < 0)
		TempVec = direction;

	TempVec = TempVec.ComputeNormalized();
	TempVec.x = TempVec.x * (float)(GetMoveSpeed()) * elapsedTime;
	TempVec.y = (TempVec.y * (float)(GetMoveSpeed()) * elapsedTime) / 1.5f;
	SGD::Point NextPos = m_currPosition;
	NextPos.x += TempVec.x;
	NextPos.y += TempVec.y;
	this->SetPosition(NextPos);
	if (direction.x < 0)
	{
		if (LandTimer < 0 && (m_currPosition.x - Camera::GetInstance()->GetMapLayer()->FindEventTiles("QueensLanding")[0].left) < 500)
		{
			SetMoveMode(MOVE_WALK);
			Landing = true;
			TimeStamp->m_sLastAnimation = "LandingWest";

		}
	}
	else
	{
		if (LandTimer < 0 && (Camera::GetInstance()->GetMapLayer()->FindEventTiles("QueensLanding")[0].left - m_currPosition.x) < 500)
		{
			SetMoveMode(MOVE_WALK);
			Landing = true;
			TimeStamp->m_SCurrentAnimation = "LandingEast";

		}
	}
	if (LandTimer < 0 && Landing == true)
		currAction = CON_LAND;
	if (currAction == CON_LAND)
	{
		for (size_t i = 0; i < 4; i++)
		{
			if (abs(Camera::GetInstance()->GetMapLayer()->FindEventTiles("QueensLanding")[0].left - m_currPosition.x) < 100 &&
				abs(Camera::GetInstance()->GetMapLayer()->FindEventTiles("QueensLanding")[0].top - m_currPosition.y) < 100)
			{
				currAction = CON_REST;
				m_SightSenseSz = SGD::Size{ 20, 20 };
				m_SightEmitRect = SGD::Rectangle({}, GetSightEmitSz());
				m_SoundSenseSz = SGD::Size{ 700, 700 };

				SetMoveMode(MOVE_STAND);
				TimeStamp->m_SCurrentAnimation = "Sleeping";
				break;
			}
		}
	}
	if (currAction == CON_REST)
	{
		m_SightSenseSz = SGD::Size{ 20, 20 };
		m_SightEmitRect = SGD::Rectangle({}, GetSightEmitSz());
		m_SoundSenseSz = SGD::Size{ 700, 700 };
		Target = nullptr;
	}
	else
	{
		m_SightSenseSz = SGD::Size{ 4000, 4000 };
		m_SightEmitRect = SGD::Rectangle({}, GetSightEmitSz());
		m_SoundSenseSz = SGD::Size{ 0, 0 };
	}
	Entity::Update(elapsedTime);

	if (m_soundTimer < 0)
	{
		m_soundTimer = 7;
	}
}
void Queen::Render(SGD::Point bawlz)
{
	Entity::Render(bawlz);
	this->GetSenses()->ProjectSight(this);
	if (true)
	{
		if (GetAlpha() < 255)
		{
			SetAlpha(GetAlpha() + 1);
		}
	}
	else
	{
		if (GetAlpha() > 150)
			SetAlpha(GetAlpha() - 1);
	}
}
void Queen::HandleCollision(const IEntity* pOther)
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
}

void Queen::PreformItemBehavaior(const IEntity* pOther)
{

}

void Queen::UpdateMoveSpeed(){
	switch (this->GetMoveMode()) {

	case MOVE_STAND:
		m_moveSpeed = 0;
		break;

	case MOVE_WALK:
		m_moveSpeed = 200;
		break;

	case MOVE_TROT:
		m_moveSpeed = 400;
		break;

	case MOVE_RUN:
		m_moveSpeed = 700;
		break;

	case MOVE_EXHAUSTED:
		m_moveSpeed = 200;
		break;
	}
}