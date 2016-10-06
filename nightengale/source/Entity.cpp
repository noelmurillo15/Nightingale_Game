#include "Entity.h"

#include "SetPlayerTargetMessage.h"
#include "DestroyEntityMessage.h"
#include "Bumbles.h"
#include <cassert>


Entity::~Entity(void) {
	delete m_ptBrain;
	delete TimeStamp;
	delete LocalSenses;
}

void Entity::LoadData(SGD::Size soundRange, SGD::Size sightRange, SGD::Size smellRange, SGD::Point spawnPoint){

	LocalSenses = new Senses;
	m_ptBrain = new DataWereHouse;
	TimeStamp = new AnimationTimestamp(m_Name, "Idle");

	m_SightSenseSz = sightRange;
	m_SoundSenseSz = soundRange;
	m_SmellSenseSz = smellRange;
	SetInitialPosition(spawnPoint);

	m_BaseRect = SGD::Rectangle(GetPosition(), GetSize());
	m_SoundEmitRect = SGD::Rectangle({}, GetSoundEmitSz());
	m_SightEmitRect = SGD::Rectangle({}, GetSightEmitSz());

	SetMoveMode(MOVE_STAND);
	m_ptBrain->SetCurrentState(Entity::STATE_PASSIVE);
}

void Entity::Update(float elapsedTime) {

	if (GetAlive()) {
#pragma region If the creature is dead
		if (GetHealth() <= 0 && (GetType() == ENT_ANIMAL || GetType() == ENT_PLAYER)){
			SetAlive(false);
			SetTarget(nullptr);
			SetHealth(100);
			SetRespawnTimer(20.0f);
			SetDestination({ -10.0f, -10.0f });

			DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);
			pMsg->QueueMessage();
			pMsg = nullptr;
			return;
		}
#pragma endregion

		Warp();

		UpdateBaseRect(Camera::GetInstance()->GetCameraOffset());
		UpdateMoveSpeed();

		if (GetMoveMode() == MOVE_STAND && GetInHiding())
			GetDataWareHouse()->SetCurrentState(STATE_ALERT);

		if (m_soundTimer > 0)
			m_soundTimer -= elapsedTime;

		if (m_warpTimer > 0)
			m_warpTimer -= elapsedTime;	

#pragma region Plays creature sound effect
		if (m_aSound != SGD::INVALID_HANDLE) {
			SGD::Vector H = Camera::GetInstance()->GetCameraPosition() - GetPosition();
			float g = ((abs(H.x) + abs(H.y)) / 15);
			float b = 100 - g;
			SGD::AudioManager::GetInstance()->SetAudioVolume(m_aSound, (int)b);

			if (m_soundTimer <= 0 && b > 10) {
				if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_aSound))
					SGD::AudioManager::GetInstance()->PlayAudio(m_aSound);
			}
		}
#pragma endregion		

		AnimationSystem::GetInstance()->Update(TimeStamp, elapsedTime);
	}
	else {
		if (m_respawnTimer > 0)
			m_respawnTimer -= elapsedTime;

		Respawn();
	}
}

void Entity::Render(SGD::Point bawlz) {

	if (GetAlive()){
		SGD::GraphicsManager* g = SGD::GraphicsManager::GetInstance();	

#pragma region DEBUG
		switch (Camera::GetInstance()->GetMapLayer()->DebugMode()){

		case Layer::DEBUG_ALL:
			if (!GetRect().IsEmpty())
				g->DrawRectangle(GetRect(), {}, SGD::Color(0, 0, 50), 1);
			if (!GetSightEmitRect().IsEmpty() && !GetSoundEmitRect().IsEmpty()){
				g->DrawRectangle(GetSightEmitRect(), { 25, 155, 0, 0 }, SGD::Color(155, 0, 0));
				g->DrawRectangle(GetSoundEmitRect(), { 25, 0, 0, 155 }, SGD::Color(0, 0, 155));
			}
			if (!m_SightSenseRect.IsEmpty() && !m_SoundSenseRect.IsEmpty() && !m_SmellSenseRect.IsEmpty()){
				g->DrawRectangle(m_SightSenseRect, { 0, 0, 0, 0 }, SGD::Color(255, 0, 0));
				g->DrawRectangle(m_SoundSenseRect, { 0, 0, 0, 0 }, SGD::Color(0, 0, 255));
				g->DrawRectangle(m_SmellSenseRect, { 0, 0, 0, 0 }, SGD::Color(255, 255, 0));
			}
			g->DrawLine(SGD::Point(GetPosition().x - bawlz.x, GetPosition().y - bawlz.y), SGD::Point(GetPosition().x - bawlz.x, GetPosition().y - bawlz.y) + m_vecDirection * 400);
			break;

		case Layer::DEBUG_SENSERECTS:
			if (!m_SightSenseRect.IsEmpty() && !m_SoundSenseRect.IsEmpty() && !m_SmellSenseRect.IsEmpty()){
				g->DrawRectangle(m_SightSenseRect, { 0, 0, 0, 0 }, SGD::Color(255, 0, 0));
				g->DrawRectangle(m_SoundSenseRect, { 0, 0, 0, 0 }, SGD::Color(0, 0, 255));
				g->DrawRectangle(m_SmellSenseRect, { 0, 0, 0, 0 }, SGD::Color(255, 255, 0));
			}
			break;

		case Layer::DEBUG_EMITRECTS:
			if (!GetSightEmitRect().IsEmpty() && !GetSoundEmitRect().IsEmpty()){
				g->DrawRectangle(GetSightEmitRect(), { 25, 155, 0, 0 }, SGD::Color(155, 0, 0));
				g->DrawRectangle(GetSoundEmitRect(), { 25, 0, 0, 155 }, SGD::Color(0, 0, 155));
			}
			break;

		case Layer::DEBUG_BASE:
			if (!GetRect().IsEmpty())
				g->DrawRectangle(GetRect(), {}, SGD::Color(0, 0, 50), 1);

			g->DrawLine(SGD::Point(GetPosition().x - bawlz.x, GetPosition().y - bawlz.y), SGD::Point(GetPosition().x - bawlz.x, GetPosition().y - bawlz.y) + m_vecDirection * 400);
			break;

		default:
			break;
		}
#pragma endregion

		if (GetCreature() != CRT_PASSANT && GetCreature() != CRT_CASTLE && GetCreature() != CRT_BUMBLES)
			AnimationSystem::GetInstance()->Render(TimeStamp, m_currPosition.x - bawlz.x, m_currPosition.y - bawlz.y, { 1.25f, 1.25f }, { (unsigned char)this->GetAlpha(), 255, 255, 255 });
	}
}

void Entity::HandleCollision(const IEntity* pOther) {

}

void Entity::SetInHiding(bool boo){
	m_inHiding = boo;

	if (m_inHiding == true){
		if (GetType() == ENT_ANIMAL && GetCreature() != CRT_QUEEN && GetCreature() != CRT_CASTLE)
			SetAlpha(0);

		if (ENT_PLAYER == GetType())
			SetAlpha(80);
	}
	else
		SetAlpha(255);
}

void Entity::PreformItemBehavaior(const IEntity* pOther) {

}

void Entity::UpdateMoveSpeed(){

}

StankNode* Entity::CreateSmellNode(Entity* _Entity) {
	StankNode* TempNode = new StankNode;
	TempNode->m_origin = { _Entity->GetPosition() };
	TempNode->m_StankSize = SGD::Size(50, 50);
	TempNode->m_recStankVolume = SGD::Rectangle(TempNode->m_origin, TempNode->m_StankSize);
	TempNode->m_iDecayTime = 2.0f;
	return TempNode;
}

float Entity::CheckLoS(IEntity* ToCheck) {

	Entity* m_ptThis = (Entity*)ToCheck;

	if (m_ptThis->GetInHiding() == false)
	{
		SGD::Vector m_Distance = GetPosition().ComputeDifference(m_ptThis->GetPosition());

		if (m_Distance.ComputeLength() < 400) {
			SGD::Vector TowardsObject = m_ptThis->GetPosition() - GetPosition();
			TowardsObject.Normalize();
			m_vecDirection.Normalize();

			float DegreeBetween = acos(TowardsObject.ComputeDotProduct(m_vecDirection));
			return DegreeBetween;
		}
	}

	return -1;
}

vector<Entity*> Entity::InLineOfSight() {
	vector<Entity*> LoS_Container;
	return LoS_Container;
}

void Entity::UpdateBaseRect(SGD::Point bawlz){
	m_BaseRect.Resize(GetSize());
	if (GetCreature() != CRT_CASTLE)
		m_BaseRect.MoveTo({ GetPosition().x - bawlz.x, GetPosition().y - bawlz.y - (GetSize().height / 2) });
	else
		m_BaseRect.MoveTo({ GetPosition().x - bawlz.x + (GetSize().width / 4), GetPosition().y - bawlz.y });

	UpdateEmitRects();
	UpdateSenseRects();
}

void Entity::UpdateSenseRects(){
	float scale = 1.0f;
	float secscale = 2.0f;

	if (this->GetDataWareHouse()->GetCurrentState() == STATE_ALERT){
		scale = 1.35f;
		secscale = 1.65f;
	}

	m_SightSenseRect = SGD::Rectangle(GetRect());
	m_SoundSenseRect = SGD::Rectangle(GetRect());
	m_SmellSenseRect = SGD::Rectangle(GetRect());

	m_SightSenseRect.Resize(GetSightSenseSz() * scale);
	m_SoundSenseRect.Resize(GetSoundSenseSz() * scale);
	m_SmellSenseRect.Resize(GetSmellSenseSz() * scale);

	m_SightSenseRect.MoveTo(SGD::Point(GetRect().left - GetSightSenseSz().width / secscale + (GetSize().width / 2), GetRect().top - GetSightSenseSz().height / secscale + (GetSize().height / 2)));
	m_SoundSenseRect.MoveTo(SGD::Point(GetRect().left - GetSoundSenseSz().width / secscale + (GetSize().width / 2), GetRect().top - GetSoundSenseSz().height / secscale + (GetSize().height / 2)));
	m_SmellSenseRect.MoveTo(SGD::Point(GetRect().left - GetSmellSenseSz().width / secscale + (GetSize().width / 2), GetRect().top - GetSmellSenseSz().height / secscale + (GetSize().height / 2)));
}

void Entity::UpdateEmitRects(){

	{		//	Set Sense Emitter rects
		SetSightEmitRect(GetRect());
		m_SightEmitRect.Resize(GetSightEmitSz());
		m_SightEmitRect.MoveTo(SGD::Point(GetRect().left - GetSightEmitSz().width / 2 + (GetSize().width / 2), GetRect().top - GetSightEmitSz().height / 2 + (GetSize().height / 2)));

		SetSoundEmitRect(GetRect());
		m_SoundEmitRect.Resize(GetSoundEmitSz());
		m_SoundEmitRect.MoveTo(SGD::Point(GetRect().left - GetSoundEmitSz().width / 2 + (GetSize().width / 2), GetRect().top - GetSoundEmitSz().height / 2 + (GetSize().height / 2)));
	}

	if (!GetInHiding() || GetCreature() == CRT_QUEEN){
		if (GetMoveMode() == MOVE_RUN){
			m_SightEmitRect.Resize(GetSightEmitSz() * 1.25f);
			m_SightEmitRect.MoveTo({ m_SightEmitRect.left - m_SightEmitRect.ComputeSize().width / 8, m_SightEmitRect.top - m_SightEmitRect.ComputeSize().height / 8 });
		}
	}
	else{ // if hiding == true
		if (GetMoveMode() == MOVE_RUN){
			m_SightEmitRect.Resize(GetSightEmitSz() / 2);
			m_SightEmitRect.MoveTo({ m_SightEmitRect.left + m_SightEmitRect.ComputeSize().width / 2, m_SightEmitRect.top + m_SightEmitRect.ComputeSize().height / 2 });
		}
		else{
			SetSightEmitRect(GetRect());
		}
	}
}

std::string Entity::GetDirection_8(SGD::Vector loc) {

	if (loc == SGD::Vector{ 0, 0 })
		return "Idle";

		if (loc.x > .10)
		{
			if (loc.y > .10)
			{
				m_previousDirection = "SouthEast";
				return "SouthEast";
			}
			else if (loc.y < -.10)
			{
				m_previousDirection = "NorthEast";
				return "NorthEast";
			}

			else
			{
				m_previousDirection = "East";
				return "East";
			}

		}
		else if (loc.x < -.10)
		{
			if (loc.y > .10)
			{
				m_previousDirection = "SouthWest";
				return "SouthWest";
			}

			else if (loc.y < -.10)
			{
				m_previousDirection = "NorthWest";
				return "NorthWest";
			}

			else
			{
				m_previousDirection = "West";
				return "West";
			}

		}
		else
		{
			if (loc.y > 0)
			{
				m_previousDirection = "South";
				return "South";
			}

			else if (loc.y < 0)
			{
				m_previousDirection = "North";
				return "North";
			}

		}
	
	return "Idle";
}

std::string Entity::GetDirection_4(SGD::Vector loc) {

	if (loc == SGD::Vector{ 0, 0 })
		return "Idle";
	

		if (loc.x > 0 /*&& abs(loc.x) > abs(loc.y)*/)
		{
			m_previousDirection = "East";
			return "East";
		}


		if (loc.x < 0 /*&& abs(loc.x) > abs(loc.y)*/)
		{
			m_previousDirection = "West";
			return "West";
		}


		if (loc.y > 0)
		{
			m_previousDirection = "South";
			return "South";
		}
		if (loc.y < 0)
		{
			m_previousDirection = "North";
			return "North";
		}

	
	return "Idle";
}

void Entity::SetPosition(SGD::Point pos){

		SGD::Vector  TempVec;
	if (GetPosition() != pos)
	{
		TempVec = GetPosition() - pos;
		m_vecDirection = TempVec.ComputeNormalized();
		m_vecDirection = -m_vecDirection;
	}

	if (AnimationSystem::GetInstance()->GetLoaded()[TimeStamp->m_SCurrentObject].size() < 8 || GetCreature() == CRT_QUEEN)
		GetTimestamp()->SetAnimation(GetDirection_4(-TempVec));
	else
		GetTimestamp()->SetAnimation(GetDirection_8(-TempVec));

	if (GetCreature() != CRT_QUEEN) {
		if (pos.x > 0 && pos.x < Camera::GetInstance()->GetWorldSize().width)
			m_currPosition.x = pos.x;

		if (pos.y > 0 && pos.y < Camera::GetInstance()->GetWorldSize().height)
			m_currPosition.y = pos.y;
	}
	else
		m_currPosition = pos;

	if (this != Camera::GetInstance()->GetCamFocus())
		m_prevPosition = GetPosition();


	if ((GetPrevPosition().ComputeDifference(m_destination).ComputeLength() < 20) && (GetCreature() == CRT_BUMBLES || GetCreature() == CRT_KNIGHT))
	{
		m_destination = GetPosition();
		SetDirectionIdle();
	}


}

void Entity::SetDirectionIdle() {
	string Direction = GetDirection_8(m_vecDirection);
	string Idle = "Idle";

	if (Direction == Idle)
		GetTimestamp()->SetAnimation(Idle + m_previousDirection);
}

void Entity::Warp(){
	if (!GetPosition().IsPointInRectangle(Camera::GetInstance()->GetCameraBounds())){
		if (m_warpTimer <= 0)
		{
			if (GetType() == ENT_ANIMAL && GetCreature() != CRT_QUEEN){
				if (this->GetDataWareHouse()->GetCurrentState() == STATE_PASSIVE){
					SGD::Point newPos = Camera::GetInstance()->GetMapLayer()->FindOffScreenSpawnPt(GetName());
					SetInitialPosition(newPos);
				}
			}
			m_warpTimer = 180.0f;
		}
	}
}

void Entity::Respawn(){
	if (!m_alive && GetType() == ENT_ANIMAL){
		if (m_respawnTimer <= 0){
			m_alive = true;
			SetInitialPosition(Camera::GetInstance()->GetMapLayer()->FindOffScreenSpawnPt(GetName()));
			SetTarget(nullptr);
			GetDataWareHouse()->SetCurrentState(STATE_PASSIVE);
		}
	}
}

SGD::Vector Entity::Zoolander(SGD::Vector tempVec){

	SGD::Vector TV = tempVec;
	SGD::Point TP = SGD::Point{ 0, 0 } +tempVec;

	if (GetCreature() != CRT_BUMBLES)
	{

		if (Camera::GetInstance()->GetMapLayer()->CheckCollision(this, Camera::GetInstance()->GetCameraOffset(), TP) || Camera::GetInstance()->GetMapLayer()->CheckWater(this, Camera::GetInstance()->GetCameraOffset()))
		{
			TV.Rotate(0.7853981f);
			TP = SGD::Point{ 0, 0 } +TV;
			if (Camera::GetInstance()->GetMapLayer()->CheckCollision(this, Camera::GetInstance()->GetCameraOffset(), TP) || Camera::GetInstance()->GetMapLayer()->CheckWater(this, Camera::GetInstance()->GetCameraOffset()))
			{
				TV = tempVec;
				TV.Rotate(-0.78539816f);
				TP = SGD::Point{ 0, 0 } +TV;
				if (Camera::GetInstance()->GetMapLayer()->CheckCollision(this, Camera::GetInstance()->GetCameraOffset(), TP) || Camera::GetInstance()->GetMapLayer()->CheckWater(this, Camera::GetInstance()->GetCameraOffset()))
				{
					TV = tempVec;
					TV.Rotate(1.5707963f);
					TP = SGD::Point{ 0, 0 } +TV;
					if (Camera::GetInstance()->GetMapLayer()->CheckCollision(this, Camera::GetInstance()->GetCameraOffset(), TP) || Camera::GetInstance()->GetMapLayer()->CheckWater(this, Camera::GetInstance()->GetCameraOffset()))
					{
						TV = tempVec;
						TV.Rotate(-1.5707963f);
						TP = SGD::Point{ 0, 0 } +TV;
						if (Camera::GetInstance()->GetMapLayer()->CheckCollision(this, Camera::GetInstance()->GetCameraOffset(), TP) || Camera::GetInstance()->GetMapLayer()->CheckWater(this, Camera::GetInstance()->GetCameraOffset()))
						{
							TV = tempVec;
							TV.Rotate(-0.78539816f);
							TV *= 2;
							TP = SGD::Point{ 0, 0 } +TV;
							if (Camera::GetInstance()->GetMapLayer()->CheckCollision(this, Camera::GetInstance()->GetCameraOffset(), TP) || Camera::GetInstance()->GetMapLayer()->CheckWater(this, Camera::GetInstance()->GetCameraOffset()))
							{
								TV = tempVec;
								TV.Rotate(0.7853981f);
								TV *= 2;
								TP = SGD::Point{ 0, 0 } +TV;
								if (Camera::GetInstance()->GetMapLayer()->CheckCollision(this, Camera::GetInstance()->GetCameraOffset(), TP) || Camera::GetInstance()->GetMapLayer()->CheckWater(this, Camera::GetInstance()->GetCameraOffset()))
								{
									TV = tempVec;
									TV *= 3;
									TP = SGD::Point{ 0, 0 } +TV;

									tempVec = TV;
								}
								else
								tempVec = TV;
							}
							else
								tempVec = TV;
						}
						else
							tempVec = TV;
					}
					else
						tempVec = TV;
				}
				else
					tempVec = TV;
			}
			else
				tempVec = TV;
		}
	}
	else
	{
		if (Camera::GetInstance()->GetMapLayer()->CheckCollision(this, Camera::GetInstance()->GetCameraOffset(), TP))
		{
			TV.Rotate(0.7853981f);
			TP = SGD::Point{ 0, 0 } +TV;
			if (Camera::GetInstance()->GetMapLayer()->CheckCollision(this, Camera::GetInstance()->GetCameraOffset(), TP))
			{
				TV = tempVec;
				TV.Rotate(-0.78539816f);
				TP = SGD::Point{ 0, 0 } +TV;
				if (Camera::GetInstance()->GetMapLayer()->CheckCollision(this, Camera::GetInstance()->GetCameraOffset(), TP))
				{
					TV = tempVec;
					TV.Rotate(1.5707963f);
					TP = SGD::Point{ 0, 0 } +TV;
					if (Camera::GetInstance()->GetMapLayer()->CheckCollision(this, Camera::GetInstance()->GetCameraOffset(), TP))
					{
						TV = tempVec;
						TV.Rotate(-1.5707963f);
						TP = SGD::Point{ 0, 0 } +TV;
						if (Camera::GetInstance()->GetMapLayer()->CheckCollision(this, Camera::GetInstance()->GetCameraOffset(), TP))
						{
							TV = tempVec;
							TV.Rotate(2.356194f);
							TP = SGD::Point{ 0, 0 } +TV;
							if (Camera::GetInstance()->GetMapLayer()->CheckCollision(this, Camera::GetInstance()->GetCameraOffset(), TP))
							{
								TV = tempVec;
								TV.Rotate(2.356194f);
								TP = SGD::Point{ 0, 0 } +TV;

								tempVec = TV;
							}
							else
								tempVec = TV;
						}
						else
							tempVec = TV;
					}
					else
						tempVec = TV;
				}
				else
					tempVec = TV;
			}
			else
				tempVec = TV;
		}
		Bumbles* BigB = (Bumbles*)this;
		if (Camera::GetInstance()->GetMapLayer()->CheckWater(this, Camera::GetInstance()->GetCameraOffset()))
		{

			if (BigB->m_ResetTrans)
			{
				if (BigB->m_bTransitioning == false)
					BigB->InWater = !BigB->InWater;

				BigB->m_bTransitioning = true;
				BigB->m_ResetTrans = false;
			}

		}
		else
			BigB->m_ResetTrans = true;

	}
	return tempVec;
}