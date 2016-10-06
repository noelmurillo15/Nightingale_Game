#include "Player.h"

#include "HUD.h"
#include "Game.h"
#include "AddDialogMsg.h"

#include "..\SGD Wrappers\SGD_InputManager.h"


Player::Player(){

	SetSize({ 32, 32 });
	SetSightEmitSz(this->GetSize() * 4);
	SetSoundEmitSz(this->GetSize() * 5);

	SetHealth(100);
	SetStamina(100);
	SetNumBaits(3);
	SetNumRepellents(2);


	m_nighttime = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/NightTime.png");
	m_bHurt = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/Hurt.png");


	if (m_Name == "Jenny")
	{
		m_sHurt = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/Jenny.wav");
		m_sDeath = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/DeathFemale.wav");
	}
	else
	{
		m_sHurt = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/Hurt.wav");
		m_sDeath = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/DeathMale.wav"); 
	}
	AnimationSystem::GetInstance()->Load(GetName(), GetName());

	m_hurtTimer = 0.0f;
	m_NighttimeTimer = 0.0f;

	m_Hud = new HUD();
	m_Hud->SetCurrentPlayer(this);
}

Player::Player(string charName){

	SetName(charName);
	SetSize({ 32, 32 });
	SetSightEmitSz(this->GetSize() * 4);
	SetSoundEmitSz(this->GetSize() * 5);

	SetHealth(100);
	SetStamina(100);
	SetNumBaits(10);
	SetNumRepellents(10);

	m_NighttimeDeath = false;


	LoadData({}, {}, {}, Camera::GetInstance()->GetMapLayer()->FindSpawnPoint("Player"));

	m_nighttime = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/NightTime.png");
	m_bHurt = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/Hurt.png");

	if (m_Name == "Jenny")
		m_sHurt = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/Jenny.wav");
	else
		m_sHurt = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/Hurt.wav");

	AnimationSystem::GetInstance()->Load(GetName(), GetName());

	for (size_t curr = 0; curr < m_ListSmellTrail.size(); curr++)
	{
		m_ListSmellTrail[curr]->Terminate();
	}
	m_Hud = new HUD();
	m_Hud->SetCurrentPlayer(this);
}

Player::~Player(){

	if (m_bHurt != SGD::INVALID_HANDLE)
		SGD::GraphicsManager::GetInstance()->UnloadTexture(m_bHurt);

	if (m_nighttime != SGD::INVALID_HANDLE)
		SGD::GraphicsManager::GetInstance()->UnloadTexture(m_nighttime);

	if (m_sHurt != SGD::INVALID_HANDLE)
		SGD::AudioManager::GetInstance()->UnloadAudio(m_sHurt);

	if (m_sDeath != SGD::INVALID_HANDLE)
		SGD::AudioManager::GetInstance()->UnloadAudio(m_sDeath);

	for (unsigned int curr = 0; curr < m_ListSmellTrail.size(); curr++)
		delete m_ListSmellTrail[curr];

	m_ListSmellTrail.clear();

	m_StringTable.clear();

	delete m_Hud;
	m_Hud = nullptr;
}

void Player::LoadDialog() {

	m_StringTable[MSG_ENVIRONMENT].push_back("I think I heard something");
	m_StringTable[MSG_ENVIRONMENT].push_back("I feel like something is \n following me..");
	m_StringTable[MSG_ENVIRONMENT].push_back("Did that bush just move?");
	m_StringTable[MSG_ENVIRONMENT].push_back("What was that?");
	m_StringTable[MSG_ENVIRONMENT].push_back("This place creeps me out");
	m_StringTable[MSG_ENVIRONMENT].push_back("Wish I could go back \n but I have to do this for Kao");
	m_StringTable[MSG_ENVIRONMENT].push_back("I wonder if any of the creatures \n here are friendly");

	m_StringTable[MSG_ENVIRONMENT].push_back("Can we really do this?");
	m_StringTable[MSG_ENVIRONMENT].push_back("I think we can do this!");
	m_StringTable[MSG_ENVIRONMENT].push_back("We have to do this for Kao");
	m_StringTable[MSG_ENVIRONMENT].push_back("What happens if we fail?");
	m_StringTable[MSG_ENVIRONMENT].push_back("We cannot fail our mission");

	std::string tmp = "Hello there I am " + this->GetName() + " who are you?";
	m_StringTable[MSG_GREETING].push_back(tmp);
}

void Player::SendDialog(unsigned int dialogtype){
	auto it = m_StringTable[dialogtype].begin();
	int num = rand() % m_StringTable[dialogtype].size();
	std::advance(it, num);
	std::string ran = it->c_str();

	AddDialogMsg* pMsg = new AddDialogMsg(ran, GetName());
	pMsg->QueueMessage();
	pMsg = nullptr;
}


void Player::Update(float elapsedTime){

	SGD::InputManager* input = SGD::InputManager::GetInstance();
	Camera* cam = Camera::GetInstance();


	if (m_NighttimeTimer < 254)
		m_NighttimeTimer += elapsedTime;
	else{
		m_NighttimeDeath = true;
	}

	if (arcadebuffer >= 0)
		arcadebuffer -= elapsedTime;

	if (m_hurtTimer > 0)
		m_hurtTimer -= elapsedTime;

	if (hurtOp > 0)
		hurtOp -= elapsedTime * 60;
	else
		hurtOp = 0;

	if (GetMoveMode() == MOVE_RUN && m_Stamina > 0)
		m_Stamina -= elapsedTime * 15;
	else if (GetMoveMode() == MOVE_TROT && m_Stamina > 0)
		m_Stamina -= elapsedTime * 18;



	Input();

#pragma region Stank trail
	m_fSmellTimer += elapsedTime;
	if (m_fSmellTimer > SMELL_CONTROL)
	{
		StankNode* Smell = CreateSmellNode(this);

		if (m_ListSmellTrail.size())
			m_ListSmellTrail[m_ListSmellTrail.size() - 1]->Next = Smell;

		m_ListSmellTrail.push_back(Smell);


		m_fSmellTimer = 0;
	}

	for (unsigned int curr = 0; curr < m_ListSmellTrail.size(); curr++)
	{
		if (m_ListSmellTrail[curr]->Update(elapsedTime) == false)
		{
			auto Iter = m_ListSmellTrail.begin() + curr;
			delete m_ListSmellTrail[curr];
			m_ListSmellTrail.erase(Iter);
		}
	}
#pragma endregion

#pragma region Update Movement
	MoveSpeedManip(elapsedTime);

	SGD::Point nextmove = { 0.0f, 0.0f };

	if (GetPosition().y > 0.0f && input->IsKeyDown(SGD::Key::W) ||
		input->IsDPadDown(0, SGD::DPad::Up) ||
		input->GetLeftJoystick(0).y < -0.25f){
		nextmove.y -= m_moveSpeed * elapsedTime;
		if (Camera::GetInstance()->GetMapLayer()->CheckCollision(this, cam->GetCameraOffset(), nextmove) || Camera::GetInstance()->GetMapLayer()->CheckWater(this, nextmove))
			nextmove.y = 0;

	}

	if (GetPosition().y + 32.0f < cam->GetWorldSize().height && input->IsKeyDown(SGD::Key::S) ||
		input->IsDPadDown(0, SGD::DPad::Down) ||
		input->GetLeftJoystick(0).y > 0.25f){
		nextmove.y += m_moveSpeed * elapsedTime;
		if (Camera::GetInstance()->GetMapLayer()->CheckCollision(this, cam->GetCameraOffset(), nextmove) || Camera::GetInstance()->GetMapLayer()->CheckWater(this, nextmove))
			nextmove.y = 0;
	}
	if (GetPosition().x > 0.0f && input->IsKeyDown(SGD::Key::A) ||
		input->IsDPadDown(0, SGD::DPad::Left) ||
		input->GetLeftJoystick(0).x < -0.25f){
		nextmove.x -= m_moveSpeed * elapsedTime;
		if (Camera::GetInstance()->GetMapLayer()->CheckCollision(this, cam->GetCameraOffset(), nextmove) || Camera::GetInstance()->GetMapLayer()->CheckWater(this, nextmove))
			nextmove.x = 0;

	}

	if (GetPosition().x + 32.0f < cam->GetWorldSize().width && input->IsKeyDown(SGD::Key::D) ||
		input->IsDPadDown(0, SGD::DPad::Right) ||
		input->GetLeftJoystick(0).x > 0.25f){
		nextmove.x += m_moveSpeed * elapsedTime;
		if (Camera::GetInstance()->GetMapLayer()->CheckCollision(this, cam->GetCameraOffset(), nextmove) || Camera::GetInstance()->GetMapLayer()->CheckWater(this, nextmove))
			nextmove.x = 0;

	}


	m_destination = { m_currPosition.x + nextmove.x, m_currPosition.y + nextmove.y };
	SetDestination(SGD::Point(this->GetPosition().x + nextmove.x, this->GetPosition().y + nextmove.y));
	SetPosition(SGD::Point(m_currPosition.x + nextmove.x, m_currPosition.y + nextmove.y));
#pragma endregion

	Entity::Update(elapsedTime);
}

void Player::Render(SGD::Point bawlz) {
	SGD::GraphicsManager* graphics = SGD::GraphicsManager::GetInstance();
	SGD::Size scale = Game::GetInstance()->GetScreenRes();

	scale.width /= 1000;
	scale.height /= 1000;

	SGD::GraphicsManager::GetInstance()->DrawTexture(m_nighttime, { 0, 0 }, 0, {}, SGD::Color{ (unsigned char)m_NighttimeTimer, 10, 0, 10 }, { scale.width * 2, scale.height * 2 });

	if (hurtOp > 0)
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_bHurt, { 0, 0 }, 0, {}, SGD::Color{ (unsigned char)hurtOp, 255, 0, 0 }, { scale.width * 2, scale.height * 2 });


#pragma region Debug
	if (Camera::GetInstance()->GetMapLayer()->DebugMode() == Layer::DEBUG_EMITRECTS || Camera::GetInstance()->GetMapLayer()->DebugMode() == Layer::DEBUG_ALL){
		if (m_ListSmellTrail.size() > 0) {
			for (unsigned int curr = 0; curr < m_ListSmellTrail.size(); curr++)
			{
				SGD::Rectangle rec = m_ListSmellTrail[curr]->m_recStankVolume;
				SGD::Rectangle offsetrec = { rec.left, rec.top, rec.right, rec.bottom };
				if (!offsetrec.IsEmpty())
					graphics->DrawRectangle(offsetrec, SGD::Color(0, 255, 255, 255), SGD::Color(155, 255, 75));
			}
		}
	}
#pragma endregion


	m_Hud->Render();
	Entity::Render(bawlz);
}

void Player::HandleCollision(const IEntity* pOther) {
	if (pOther->GetType() == ENT_ANIMAL) {
		Entity* tmpE = (Entity*)pOther;

		if (this->GetRect().IsIntersecting(tmpE->GetRect())){

			if (hurtOp <= 0)
				hurtOp = 135.0f;

			if (Camera::GetInstance()->GetMapLayer()->DebugMode() == Layer::DEBUG_OFF && m_hurtTimer <= 0){
				SetHealth(GetHealth() - pOther->GetDamage());
				m_hurtTimer = 5.0f;
			}

			if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_sHurt) == false && m_health > 0)
				SGD::AudioManager::GetInstance()->PlayAudio(m_sHurt);
			else if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_sDeath) == false && m_health <= 0)
				SGD::AudioManager::GetInstance()->PlayAudio(m_sDeath);
			
		}
	}
}

void Player::PreformItemBehavaior(const IEntity* pOther){

}

bool Player::EventHandleCollision(std::vector<SGD::Rectangle> eventtiles, SGD::Point bawlz, bool poison){
	for (unsigned int x = 0; x < eventtiles.size(); ++x){
		if (eventtiles[x].IsIntersecting(Camera::GetInstance()->GetCameraBounds())){

			SGD::Rectangle tmp = SGD::Rectangle(eventtiles[x].left - bawlz.x,
				eventtiles[x].top - bawlz.y,
				eventtiles[x].right - bawlz.x,
				eventtiles[x].bottom - bawlz.y);

			if (this->GetRect().IsIntersecting(tmp) && m_hurtTimer <= 0){
				if (poison){
					SGD::AudioManager::GetInstance()->PlayAudio(m_sHurt);
					SetHealth(GetHealth() - 5);
					m_hurtTimer = 5.0f;
				}
				return true;
			}
		}
	}
	return false;
}

void Player::Trans(){
	SetHealth(100);
	SetStamina(100); 
	SetInitialPosition(Camera::GetInstance()->GetMapLayer()->FindSpawnPoint("Player"));
	Camera::GetInstance()->ResetCamera();
	m_NighttimeTimer = 0;
	SetNumBaits(GetNumBaits() + 1);
	SetNumRepellents(GetNumRepellents() + 1);
	SetLevelTransition(false);	
}

void Player::MoveSpeedManip(float elapsedTime){

	if (m_ExhaustTimer > 0)
		m_ExhaustTimer -= elapsedTime;

	else if (Exhausted && m_ExhaustTimer <= 0)
		this->SetMoveMode(MOVE_WALK);


	if (GetMoveMode() == MOVE_STAND && m_Stamina < 100 && m_ExhaustTimer <= 0)
		m_Stamina += elapsedTime * 15;
	else if (GetMoveMode() == MOVE_WALK && m_Stamina < 100 && m_ExhaustTimer <= 0)
		m_Stamina += elapsedTime * 12;

	if (m_Stamina < 10 && !Exhausted)
		this->SetMoveMode(MOVE_TROT);

	if (m_Stamina < 1 && !Exhausted){
		this->SetMoveMode(MOVE_EXHAUSTED);
		m_ExhaustTimer = 4.0f;
		Exhausted = true;
	}

	if (m_Stamina > 60 && Exhausted)
		Exhausted = false;
}

void Player::UpdateMoveSpeed(){
	if (Camera::GetInstance()->GetMapLayer()->DebugMode() == Layer::DEBUG_OFF){
		switch (this->GetMoveMode()) {

		case MOVE_STAND:
			m_moveSpeed = 0;
			break;

		case MOVE_WALK:
			if (this->GetName() == "Jenny")
				m_moveSpeed = 100;
			else
				m_moveSpeed = 80;
			break;

		case MOVE_TROT:
			m_moveSpeed = 150;
			break;

		case MOVE_RUN:
			if (this->GetName() == "Brock")
				m_moveSpeed = 260;
			else
				m_moveSpeed = 225;
			break;

		case MOVE_EXHAUSTED:
			m_moveSpeed = 50;
			break;
		}
	}
	else
		m_moveSpeed = 600;
}

void Player::Input(){
	SGD::InputManager* input = SGD::InputManager::GetInstance();

	switch (Game::GetInstance()->GetCurrentInput()){

	case Game::CTRL_KEYBOARD:
#pragma region Keyboard
		if (input->IsKeyDown(SGD::Key::Shift)){
			if (GetMoveMode() != MOVE_RUN && GetMoveMode() != MOVE_EXHAUSTED && GetMoveMode() != MOVE_TROT)
				this->SetMoveMode(MOVE_RUN);

			this->GetTimestamp()->m_FFrameDuration -= .01f;
		}
		else{
			if (input->IsKeyDown(SGD::Key::W) || input->IsKeyDown(SGD::Key::A) || input->IsKeyDown(SGD::Key::S) || input->IsKeyDown(SGD::Key::D)){
				if (GetMoveMode() != MOVE_WALK && GetMoveMode() != MOVE_EXHAUSTED)
					this->SetMoveMode(MOVE_WALK);
			}
			else
				this->SetMoveMode(MOVE_STAND);
		}
		if (GetCanHarvestBait()) {
			if (input->IsKeyPressed(SGD::Key::Enter)) {
				if (GetNumBaits() <= 4) {
					SetNumBaits(GetNumBaits() + 1);
					SetCanHarvestBait(false);
				}
			}
		}

		if (GetCanHarvestRepellent()) {
			if (input->IsKeyPressed(SGD::Key::Enter)) {
				if (GetNumRepellents() <= 3) {
					SetNumRepellents(GetNumRepellents() + 1);
					SetCanHarvestRepellent(false);
				}
			}
		}
#pragma endregion
		break;

	case Game::CTRL_CONTROLLER:
#pragma region Controller
		if (input->IsButtonDown(0, 1)){
			if (GetMoveMode() != MOVE_RUN && GetMoveMode() != MOVE_EXHAUSTED && GetMoveMode() != MOVE_TROT)
				this->SetMoveMode(MOVE_RUN);

			this->GetTimestamp()->m_FFrameDuration -= .01f;
		}
		else{
			if (GetMoveMode() != MOVE_WALK && GetMoveMode() != MOVE_EXHAUSTED)
				this->SetMoveMode(MOVE_WALK);
		}
		if (GetCanHarvestBait()) {
			if (input->IsButtonPressed(0, 0)) {
				if (GetNumBaits() <= 4) {
					SetNumBaits(GetNumBaits() + 1);
					SetCanHarvestBait(false);
				}
			}
		}

		if (GetCanHarvestRepellent()) {
			if (input->IsButtonPressed(0, 0)) {
				if (GetNumRepellents() <= 3) {
					SetNumRepellents(GetNumRepellents() + 1);
					SetCanHarvestRepellent(false);
				}
			}
		}
#pragma endregion
		break;

	case Game::CTRL_ARCADE:
#pragma region Arcade
		if (input->IsButtonDown(0, 0)){
			if (GetMoveMode() != MOVE_RUN && GetMoveMode() != MOVE_EXHAUSTED && GetMoveMode() != MOVE_TROT)
				this->SetMoveMode(MOVE_RUN);

			this->GetTimestamp()->m_FFrameDuration -= .01f;
		}
		else{
			if (GetMoveMode() != MOVE_WALK && GetMoveMode() != MOVE_EXHAUSTED)
				this->SetMoveMode(MOVE_WALK);
		}
		if (GetCanHarvestBait()) {
			if (input->IsButtonPressed(0, 4)) {
				if (GetNumBaits() <= 4) {
					SetNumBaits(GetNumBaits() + 1);
					SetCanHarvestBait(false);
				}
			}
		}

		if (GetCanHarvestRepellent()) {
			if (input->IsButtonPressed(0, 4)) {
				if (GetNumRepellents() <= 3) {
					SetNumRepellents(GetNumRepellents() + 1);
					SetCanHarvestRepellent(false);
				}
			}
		}
#pragma endregion
		break;

	default:
		break;
	}
}