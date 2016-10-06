#include "GameplayState.h"

#include "Items.h"
#include "Camera.h"
#include "Player.h"
#include "Analyzer.h"

#include "TileEngine.h"
#include "DialogSystem.h"
#include "AddDialogMsg.h"
#include "AnimationSystem.h"
#include "MainMenuState.h"
#include "tinyxml.h"
#include "SaveSlots.h"
#include "CreditsState.h"

#include "CreatureFactory.h"
#include "EntityManager.h"
#include "DirectorAI.h"
#include "MessageID.h"
#include "CharacterSelect.h"
#include "ParticleManager.h"
#include "CreatureLogs.h"
#include "PackComposite.h"
#include "DataWerehouse.h"
#include "CreatureProfile.h"
#include "Controls.h"
#include "Passant.h"
#include "DestroyEntityMessage.h"
#include "PassantTurnMessage.h"
#include "SetPlayerTargetMessage.h"
#include "CreatureLogMessage.h"
#include "StaticObject.h"
#include "DialogSystem.h"

#include "../SGD Wrappers/SGD_MessageManager.h"
#include "../SGD Wrappers/SGD_String.h"

#include <vector>
#include <cassert>


GameplayState* GameplayState::GetInstance(void) {
	static GameplayState s_Instance;
	return &s_Instance;
}

bool GameplayState::Initialize(void) {
	if (false == m_bMessagesInit)
	{
		SGD::MessageManager::GetInstance()->Initialize(&MessageProc);
		m_bMessagesInit = true; 
	}
	AnimationSystem::GetInstance();
	TileEngine::GetInstance()->Enter();		

	m_endgame = new EndGame;

	AnaDex = new Analyzer();
	m_mKnight = new DirectorAI();
	m_Instructions = new Controls;
	m_CreatureLog = new CreatureLogs();
	m_ObjectManager = new EntityManager();
	m_ParticleManager = new ParticleManager();
	m_Dialog = new DialogSystem();

	cam = Camera::GetInstance();

	if (m_SaveSlots->LoadGame(loadfile, m_AnimalFactory, m_ObjectManager)) {
#pragma region Load bushes
		for (unsigned int i = 0; i < cam->GetMapLayer()->GetBushPoints().size(); i++) {
			TileEngine::GetInstance()->UpdateRender();
			StaticObject* Bush = m_AnimalFactory->CreateBrush(cam->GetMapLayer()->GetBushPoints()[i]);
			m_ObjectManager->AddEntity(Bush, Entity::ENT_BUSH);
	}
#pragma endregion

#pragma region Load Events
		std::vector<SGD::Rectangle> tmp = cam->GetMapLayer()->FindEventTiles("Poison");
		for (unsigned int i = 0; i < tmp.size(); i++){
			TileEngine::GetInstance()->UpdateRender();
			m_ParticleManager->CreatePoisonEmitter(tmp[i].GetBottomRight(), 1.2f, 120, SGD::Color(100, 0, 100));
		}

		tmp.clear();
		tmp = cam->GetMapLayer()->FindEventTiles("SaveEmit");
		for (unsigned int i = 0; i < tmp.size(); i++){
			TileEngine::GetInstance()->UpdateRender();
			m_ParticleManager->CreatePoisonEmitter(tmp[i].GetBottomRight(), 0.5f, 20, SGD::Color(200, 0, 255));
		}
#pragma endregion

#pragma region Load Player
		Player* tempPlayer = (Player*)m_ObjectManager->m_tEntities[Entity::ENT_PLAYER][0];
		SetPlayer(tempPlayer);
#pragma endregion
	}
	else {
		WorldTransition();
	}
	
	cam->ResetCamera();

	AnaDex->SetReady(true);

	m_ObjectManager->GvG = cam->GetMapLayer()->FindEventTiles("Water");

	AlreadyInstantiated = true;

	return AlreadyInstantiated;
}

void GameplayState::Terminate(void) {

	if (!m_mKnight)
		return;

		//Unload some more
	m_ObjectManager->RemoveAll();

	delete m_mKnight;
	delete m_ObjectManager;
	delete m_AnimalFactory;
	delete m_endgame;
	delete AnaDex;
	delete m_CreatureLog;
	delete m_ParticleManager;
	delete m_Instructions;
	delete m_Dialog;

	//	Destroy MsgManager
	SGD::MessageManager::GetInstance()->Terminate();
	SGD::MessageManager::DeleteInstance();

	//	Destroy TileEngine & AnimationSystem
	TileEngine::GetInstance()->Exit();
	AnimationSystem::GetInstance()->DeleteInstance();
}

void GameplayState::Enter(void) {
		//Load Audio
	m_win = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/ANM_WinSound.wav");
	m_lose = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/ANM_LoseSound.wav");
	m_repellant = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/RepellantSound.wav");
	m_bait = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/PutBaitDown.wav");
	m_WindHowlPlain = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/AmbianceLvl1.wav");
	m_sSwitch = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/Switch.wav");

	SGD::AudioManager::GetInstance()->SetAudioVolume(m_WindHowlPlain, 25);
	SGD::AudioManager::GetInstance()->PlayAudio(m_WindHowlPlain, true);
}

void GameplayState::Exit(void) {
	SGD::AudioManager* audio = SGD::AudioManager::GetInstance();

	audio->StopAudio(m_WindHowlPlain);

		//	Unload assets
	audio->UnloadAudio(m_win);
	audio->UnloadAudio(m_lose);
	audio->UnloadAudio(m_repellant);
	audio->UnloadAudio(m_bait);
	audio->UnloadAudio(m_WindHowlPlain);
	audio->UnloadAudio(m_sSwitch);	
}

bool GameplayState::Update(float elapsedTime)
{
	SGD::InputManager* input = SGD::InputManager::GetInstance();

	TimePlayed += elapsedTime;

	GetDialogSystem()->Update(elapsedTime);

	if (deathtimer >= 0)
		deathtimer -= elapsedTime;

	if (AnaDex->Scanned >= 2)
		CanWin = true;

	if (m_bInCreatureLog)
		m_CreatureLog->Update(elapsedTime);

	if (m_bInOptionsScreen)
		m_Instructions->Update(elapsedTime);

	if (m_CreatureLog->GetNumAnalyzed() == 10)
		m_bVictory = true;

	if (arcadebuffer >= 0)
		arcadebuffer -= elapsedTime;

	if (m_bVictory){
		if (input->IsKeyPressed(SGD::Key::Enter) || input->IsButtonDown(0, 1) || input->IsButtonDown(0, 0))
			Game::GetInstance()->ChangeState(CreditsState::GetInstance());
	}

	Input();

#pragma region Update Loop
	if (!m_save.isPaused && !m_pause.isPaused){		
		if (m_ObjectManager->m_tEntities[Entity::ENT_PLAYER][0]->GetType() == Entity::ENT_PLAYER) {
			Player* tempPlayer = (Player*)m_ObjectManager->m_tEntities[Entity::ENT_PLAYER][0];

			if (tempPlayer->GetNightimeDeath()){
				if (TansitionFade && deathtimer <= 0){
					tempPlayer->SetAlive(false);
					tempPlayer->SetTarget(nullptr);
					tempPlayer->SetHealth(100);
					tempPlayer->SetRespawnTimer(10.0f);
					tempPlayer->SetDestination({ -10.0f, -10.0f });

					Entity* p = (Entity*)m_ObjectManager->m_tEntities[Entity::ENT_PLAYER][0];
					DestroyEntityMessage* pMsg = new DestroyEntityMessage(p);
					pMsg->SendMessageNow();
					delete pMsg;
					pMsg = nullptr;

					TansitionFade = false;
					return true;
				}

				if (m_Dialog->GetIsEmpty() && !TansitionFade){
					std::string death = "The King excels at hunting during \n the night. No one is safe from him";
					m_Dialog->AddDialogToQueue(death, "Game");					
					TansitionFade = true;
					deathtimer = 8.0f;
				}				
			}

#pragma region Player_Input
			switch (Game::GetInstance()->GetCurrentInput()){
			case Game::CTRL_KEYBOARD:
#pragma region Keyboard
				if (input->IsKeyPressed(SGD::Key::Q)){//Y Button on controller
					if (tempPlayer->GetNumBaits() > 0){
						IEntity* temit = new Items(0, tempPlayer->GetPosition());					
						m_ObjectManager->AddEntity(temit, Entity::ENT_ITEM);
						m_ParticleManager->CreateItemEmitter(tempPlayer->GetPosition(), 0.0001f, 5.5f, SGD::Color(0, 0, 255));
						tempPlayer->SetNumBaits(tempPlayer->GetNumBaits() - 1);
						SGD::AudioManager::GetInstance()->PlayAudio(m_bait, false);
					}
				}
				if (input->IsKeyPressed(SGD::Key::E)){//X Button on controller
					if (tempPlayer->GetNumRepellents() > 0){
						Items* temit = new Items(1, tempPlayer->GetPosition());
						m_ObjectManager->AddEntity(temit, Entity::ENT_ITEM);
						m_ParticleManager->CreateItemEmitter(tempPlayer->GetPosition(), 0.0001f, 5.5f, SGD::Color(255, 0, 0));
						tempPlayer->SetNumRepellents(tempPlayer->GetNumRepellents() - 1);
						SGD::AudioManager::GetInstance()->PlayAudio(m_repellant, false);
					}
				}
				if (tempPlayer->GetCanSave() && input->IsKeyPressed(SGD::Key::Enter)){
					if (!m_save.isPaused && !m_pause.isPaused){
						m_save.isPaused = true;
						m_save.m_saveScreen = {
							50, 50,
							Game::GetInstance()->GetScreenRes().width - 50,
							Game::GetInstance()->GetScreenRes().height - 50
						};
					}
				}
				if (input->IsKeyPressed(SGD::Key::F10))
					WorldTransition();
#pragma endregion
				break;

			case Game::CTRL_CONTROLLER:
#pragma region Controller
				if (input->IsButtonPressed(0, 3)){//Y Button on controller
					if (tempPlayer->GetNumBaits() > 0){
						IEntity* temit = new Items(0, tempPlayer->GetPosition());
						m_ObjectManager->AddEntity(temit, Entity::ENT_ITEM);
						m_ParticleManager->CreateItemEmitter(tempPlayer->GetPosition(), 0.0001f, 5.5f, SGD::Color(0, 0, 255));
						tempPlayer->SetNumBaits(tempPlayer->GetNumBaits() - 1);
						SGD::AudioManager::GetInstance()->PlayAudio(m_bait, false);
					}
				}
				if (input->IsButtonPressed(0, 2)){//X Button on controller
					if (tempPlayer->GetNumRepellents() > 0){
						Items* temit = new Items(1, tempPlayer->GetPosition());
						m_ObjectManager->AddEntity(temit, Entity::ENT_ITEM);
						m_ParticleManager->CreateItemEmitter(tempPlayer->GetPosition(), 0.0001f, 5.5f, SGD::Color(255, 0, 0));
						tempPlayer->SetNumRepellents(tempPlayer->GetNumRepellents() - 1);
						SGD::AudioManager::GetInstance()->PlayAudio(m_repellant, false);
					}
				}
				if (tempPlayer->GetCanSave() && input->IsButtonPressed(0, 0)){
					if (!m_save.isPaused && !m_pause.isPaused){
						m_save.isPaused = true;
						m_save.m_saveScreen = {
							50, 50,
							Game::GetInstance()->GetScreenRes().width - 50,
							Game::GetInstance()->GetScreenRes().height - 50
						};
					}
				}
#pragma endregion
				break;

			case Game::CTRL_ARCADE:
#pragma region Arcade
				if (input->IsButtonPressed(0, 1)){
					if (tempPlayer->GetNumBaits() > 0)
					{
						IEntity* temit = new Items(0, tempPlayer->GetPosition());
						m_ObjectManager->AddEntity(temit, Entity::ENT_ITEM);
						m_ParticleManager->CreateItemEmitter(tempPlayer->GetPosition(), 0.0001f, 5.5f, SGD::Color(0, 0, 255));
						tempPlayer->SetNumBaits(tempPlayer->GetNumBaits() - 1);
						SGD::AudioManager::GetInstance()->PlayAudio(m_bait, false);
					}
				}
				if (input->IsButtonPressed(0, 2)){//X Button on controller
					if (tempPlayer->GetNumRepellents() > 0)
					{
						Items* temit = new Items(1, tempPlayer->GetPosition());
						m_ObjectManager->AddEntity(temit, Entity::ENT_ITEM);
						m_ParticleManager->CreateItemEmitter(tempPlayer->GetPosition(), 0.0001f, 5.5f, SGD::Color(255, 0, 0));
						tempPlayer->SetNumRepellents(tempPlayer->GetNumRepellents() - 1);
						SGD::AudioManager::GetInstance()->PlayAudio(m_repellant, false);
					}
				}
				if (tempPlayer->GetCanSave() && input->IsKeyPressed(SGD::Key::MouseLeft)){
					if (!m_save.isPaused && !m_pause.isPaused){
						m_save.isPaused = true;
						m_save.m_saveScreen = {
							50, 50,
							Game::GetInstance()->GetScreenRes().width - 50,
							Game::GetInstance()->GetScreenRes().height - 50
						};
					}
				}
#pragma endregion
				break;
			}
#pragma endregion

			AnaDex->Update(elapsedTime, m_ObjectManager, m_player);
			m_ParticleManager->UpdateEmitters(elapsedTime, cam->GetCameraOffset());

			TileEngine::GetInstance()->Update(elapsedTime);
			SGD::MessageManager::GetInstance()->Update();
		}

#pragma region Check Collisions
		m_ObjectManager->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_ANIMAL);
		m_ObjectManager->CheckCollisions(Entity::ENT_ANIMAL, Entity::ENT_ANIMAL);
		m_ObjectManager->CheckCollisions(Entity::ENT_ANIMAL, Entity::ENT_ITEM);

		m_ObjectManager->CheckEnvironmentCollisions(Entity::ENT_PLAYER);
		m_ObjectManager->CheckEnvironmentCollisions(Entity::ENT_ANIMAL);		
#pragma endregion

		m_ObjectManager->UpdateAll(elapsedTime);
	}
#pragma endregion	

	return true;
}

void GameplayState::Render(float elapsedTime) {
	SGD::GraphicsManager* graphics = SGD::GraphicsManager::GetInstance();
	BitmapFont* pFont = Game::GetInstance()->GetFont();
	SGD::Size screenres = Game::GetInstance()->GetScreenRes();

#pragma region Environment
	TileEngine::GetInstance()->Render();

	m_ParticleManager->RenderEmitters(elapsedTime, cam->GetCameraOffset());		
#pragma endregion

#pragma region Debug_Mode	
	if (cam->GetMapLayer()->DebugMode() == Layer::DEBUG_ENVIRONEMNT || cam->GetMapLayer()->DebugMode() == Layer::DEBUG_ALL){	//	Event Rects
		vector<SGD::Rectangle> tmp;
		vector<SGD::Point> tmppts = cam->GetMapLayer()->GetCollisionPoints();
		for (unsigned int x = 0; x < tmppts.size(); ++x){

			SGD::Rectangle tmpre = SGD::Rectangle(tmppts[x], SGD::Point(0, 0));
			tmpre.left -= cam->GetCameraOffset().x;
			tmpre.top -= cam->GetCameraOffset().y;
			tmpre.right = tmpre.left + 32;
			tmpre.bottom = tmpre.top + 32;

			graphics->DrawRectangle(tmpre, SGD::Color(0, 0, 0, 0), SGD::Color(100, 0, 0), 1);
		}

		tmppts.clear();
		tmppts = cam->GetMapLayer()->GetBushPoints();
		for (unsigned int x = 0; x < tmppts.size(); ++x){

			SGD::Rectangle tmpre = SGD::Rectangle(tmppts[x], SGD::Point(0, 0));
			tmpre.left -= cam->GetCameraOffset().x;
			tmpre.top -= cam->GetCameraOffset().y;
			tmpre.right = tmpre.left + 32;
			tmpre.bottom = tmpre.top + 32;

			graphics->DrawRectangle(tmpre, SGD::Color(0, 0, 0, 0), SGD::Color(0, 150, 0), 1);
		}
	}
#pragma endregion

#pragma region Entities
	m_ObjectManager->RenderAll(cam->GetCameraOffset());
#pragma endregion

#pragma region DebugOptions
	if (cam->GetMapLayer()->DebugMode() != Layer::DEBUG_OFF){

		SGD::Rectangle tmp = { screenres.width - 300, 10, screenres.width - 10, 250 };

		graphics->DrawRectangle(tmp, { 100, 0, 0, 0 }, { 0, 0, 0 }, 1);

		pFont->Draw("DEBUG MODE", "Game", { screenres.width - 250, 50 },
			.6f, { 150, 0, 0 });
		pFont->Draw("F1  Base", "Game", { screenres.width - 270, 80 },
			.7f, { 150, 0, 0 });
		pFont->Draw("F2  Environment", "Game", { screenres.width - 270, 110 },
			.7f, { 150, 0, 0 });
		pFont->Draw("F3  Emit Rectangles", "Game", { screenres.width - 270, 140 },
			.7f, { 150, 0, 0 });
		pFont->Draw("F4  Sense Rectangle", "Game", { screenres.width - 270, 170 },
			.7f, { 150, 0, 0 });
		pFont->Draw("F5  Debug All", "Game", { screenres.width - 270, 200 },
			.7f, { 150, 0, 0 });

		pFont->Draw(CalculateFps(elapsedTime), "Game", { screenres.width - 200, 240 },
			.7f, { 150, 0, 0 });
	}
#pragma endregion


	if (TileEngine::GetInstance()->GetMinimapOn())
		TileEngine::GetInstance()->RenderMiniMap(m_ObjectManager->m_tEntities[Entity::ENT_PLAYER][0]->GetRect());

	if (TansitionFade) {
		Fade += elapsedTime + .75f;
		if (Fade >= 255)
			Fade = 255;
		graphics->DrawRectangle({ SGD::Point(0, 0), Game::GetInstance()->GetScreenRes() }, SGD::Color((char)Fade, 0, 0, 0));
	}
	else {
		Fade = Fade - elapsedTime - 2.5f;
		if (Fade <= 0)
			Fade = 0;

		graphics->DrawRectangle({ SGD::Point(0, 0), Game::GetInstance()->GetScreenRes() }, SGD::Color((char)Fade, 0, 0, 0));
	}

	GetDialogSystem()->Render(cam->GetCameraBounds(), cam->GetCameraOffset());
	GetDialogSystem()->RenderQuest(cam->GetCameraBounds(), cam->GetCameraOffset(), AnaDex->Scanned);	

#pragma region Save Game
	if (m_save.isPaused) {

		graphics->DrawRectangle(m_save.m_saveScreen, SGD::Color(0, 0, 0), SGD::Color(0, 0, 255));

		// Display the level options centered at 1x scale
		pFont->Draw("Save Slot 1", "Game", { screenres.width / 2 - 160, m_save.m_saveScreen.top + 50 },
			1.0f, { 255, 0, 0 });
		pFont->Draw("Save Slot 2", "Game", { screenres.width / 2 - 160, m_save.m_saveScreen.top + 100 },
			1.0f, { 255, 0, 0 });
		pFont->Draw("Save Slot 3", "Game", { screenres.width / 2 - 160, m_save.m_saveScreen.top + 150 },
			1.0f, { 255, 0, 0 });
		pFont->Draw("Cancel", "Game", { screenres.width / 2 - 160, m_save.m_saveScreen.top + 200 },
			1.0f, { 255, 0, 0 });

		if (m_save.m_nCursor == 1)
			pFont->Draw("=           =", "Game", { screenres.width / 2 - 176, m_save.m_saveScreen.top + (50 * m_save.m_nCursor) },
			1.0f, { 0, 255, 0 });

		else if (m_save.m_nCursor == 2)
			pFont->Draw("=           =", "Game", { screenres.width / 2 - 176, m_save.m_saveScreen.top + (50 * m_save.m_nCursor) },
			1.0f, { 0, 255, 0 });
		else if (m_save.m_nCursor == 3)
			pFont->Draw("=           =", "Game", { screenres.width / 2 - 176, m_save.m_saveScreen.top + (50 * m_save.m_nCursor) },
			1.0f, { 0, 255, 0 });
		else if (m_save.m_nCursor == 4)
			pFont->Draw("=      =", "Game", { screenres.width / 2 - 176, m_save.m_saveScreen.top + (50 * m_save.m_nCursor) },
			1.0f, { 0, 255, 0 });
	}
#pragma endregion

#pragma region Pause Screen

	if (deadTimer > 0)
	{
		PlayDeathDialogue(DeadPlayer);
		deadTimer -= elapsedTime;
	}
	else
		deadTimer = 0;
	if (m_pause.isPaused && !m_pause.m_PauseScreen.IsEmpty() && !m_bInCreatureLog && !m_bInOptionsScreen)
	{
		graphics->DrawRectangle(m_pause.m_PauseScreen,
			SGD::Color(0, 0, 0), SGD::Color(150, 0, 255));

		// Display the level options centered at 1x scale
		pFont->Draw("Resume", "Game", { screenres.width / 2 - 160, m_pause.m_PauseScreen.top + 50 },
			1.0f, { 255, 0, 0 });
		pFont->Draw("Analyzer Encyclopedia", "Game", { screenres.width / 2 - 160, m_pause.m_PauseScreen.top + 100 },
			1.0f, { 255, 0, 0 });
		pFont->Draw("Controls", "Game", { screenres.width / 2 - 160, m_pause.m_PauseScreen.top + 150 },
			1.0f, { 255, 0, 0 });

		if (AnaDex->Scanned >= 2)
			pFont->Draw("Next Level", "Game", { screenres.width / 2 - 160, m_pause.m_PauseScreen.top + 200 },
			1.0f, { 255, 0, 0 });

		pFont->Draw("Quit", "Game", { screenres.width / 2 - 160, m_pause.m_PauseScreen.top + 250 },
			1.0f, { 255, 0, 0 });



		if (m_pause.m_nCursor == 1)
			pFont->Draw("=       =", "Game", { screenres.width / 2 - 176, m_pause.m_PauseScreen.top + (50 * m_pause.m_nCursor) },
			1.0f, { 0, 255, 0 });
		else if (m_pause.m_nCursor == 2)
			pFont->Draw("=                  =", "Game", { screenres.width / 2 - 176, m_pause.m_PauseScreen.top + (50 * m_pause.m_nCursor) },
			1.0f, { 0, 255, 0 });
		else if (m_pause.m_nCursor == 3)
			pFont->Draw("=        =", "Game", { screenres.width / 2 - 176, m_pause.m_PauseScreen.top + (50 * m_pause.m_nCursor) },
			1.0f, { 0, 255, 0 });
		else if (m_pause.m_nCursor == 4)
			pFont->Draw("=          =", "Game", { screenres.width / 2 - 176, m_pause.m_PauseScreen.top + 200 },
			1.0f, { 0, 255, 0 });
		else if (m_pause.m_nCursor == 5)
			pFont->Draw("=    =", "Game", { screenres.width / 2 - 176, m_pause.m_PauseScreen.top + 250 },
			1.0f, { 0, 255, 0 });
	}
	else if (m_pause.isPaused && !m_pause.m_PauseScreen.IsEmpty() && true == m_bInCreatureLog)
	{
		graphics->DrawRectangle(m_pause.m_PauseScreen,
			SGD::Color(0, 0, 0), SGD::Color(0, 255, 255));

		m_CreatureLog->Render(elapsedTime);
	}
	else if (m_pause.isPaused && !m_pause.m_PauseScreen.IsEmpty() && true == m_bInOptionsScreen)
	{
		graphics->DrawRectangle(m_pause.m_PauseScreen,
			SGD::Color(150, 150, 150), SGD::Color(155, 0, 255));

		m_Instructions->Render(Game::GetInstance()->GetCurrentInput());
	}
#pragma endregion

	AnaDex->Render();

	if (true == m_bVictory)
	{
		graphics->DrawRectangle(SGD::Rectangle(50, 50, Game::GetInstance()->GetScreenRes().width - 50, Game::GetInstance()->GetScreenRes().height - 50),
			SGD::Color(0, 0, 0), SGD::Color(150, 0, 255));
		if (Game::GetInstance()->GetCurrentRes() == Game::GetInstance()->CTRL_ARCADE)
		{
			pFont->Draw("The Nightingale research team has managed to research enough\nof the hostile world to ensure the success of the colonizers from Earth. \nThe reseach has ended, but the battle for human survival has only begun", "Test", { (screenres.width / 2) - 600, screenres.height / 2 },
				0.4f, { 255, 0, 0 });
		}
		else
		{
			pFont->Draw("The Nightingale research team has managed to research enough\nof the hostile world to ensure the success of the colonizers from Earth. \nThe reseach has ended, but the battle for human survival has only begun", "Test", { (screenres.width / 2) - 600, screenres.height / 2 },
				1.0f, { 255, 0, 0 });
		}
	}
}

void GameplayState::Input()
{
	SGD::InputManager* input = SGD::InputManager::GetInstance();

	switch (Game::GetInstance()->GetCurrentInput()) {

	case Game::CTRL_KEYBOARD:
#pragma region Keyboard
		if (input->IsKeyPressed(SGD::Key::Escape)){	//	Pause
			m_pause.isPaused = true;
			m_save.isPaused = false;
			m_bInCreatureLog = false;
			m_bInOptionsScreen = false;
			m_pause.m_nCursor = 1;
			m_save.m_nCursor = 1;			
			m_pause.m_PauseScreen = {
				50, 50,
				Game::GetInstance()->GetScreenRes().width - 50,
				Game::GetInstance()->GetScreenRes().height - 50
			};
		}
		if (input->IsKeyPressed(SGD::Key::Enter)) {	//	Pause options
			if (m_pause.isPaused && !m_save.isPaused){
				switch (m_pause.m_nCursor){
				case 1:
					m_pause.isPaused = false;
					m_bInCreatureLog = false;
					m_bInOptionsScreen = false;
					m_save.isPaused = false;
					GetPlayer()->SetCanSave(false);
					break;
				case 2:
					m_bInCreatureLog = true;
					break;
				case 3:
					m_bInOptionsScreen = true;
					break;
				case 4:
					WorldTransition();
					break;
				case 5:
					Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
					break;
				}
			}
			else if (m_save.isPaused && !m_pause.isPaused){

				switch (m_save.m_nCursor){
				case 1:
				{
					AddDialogMsg* msg = new AddDialogMsg("Saving Successful", "Game");
					msg->SendMessageNow();
					delete msg;
					m_SaveSlots->SaveGame("SaveSlot1", GetPlayer(), (int)TimePlayed, m_ObjectManager);
				}
					break;					

				case 2:
				{
					AddDialogMsg* msg = new AddDialogMsg("Saving Successful", "Game");
					msg->SendMessageNow();
					delete msg;
					m_SaveSlots->SaveGame("SaveSlot2", GetPlayer(), (int)TimePlayed, m_ObjectManager);
				}
					break;
					
				case 3:
				{
					AddDialogMsg* msg = new AddDialogMsg("Saving Successful", "Game");
					msg->SendMessageNow();
					delete msg;
					m_SaveSlots->SaveGame("SaveSlot3", GetPlayer(), (int)TimePlayed, m_ObjectManager);
				}
					break;
					
				case 4:
					m_pause.isPaused = true;
					m_pause.m_nCursor = 1;
					m_save.m_nCursor = 1;
					m_pause.m_PauseScreen = {
						50, 50,
						Game::GetInstance()->GetScreenRes().width - 50,
						Game::GetInstance()->GetScreenRes().height - 50
					};
					break;
				}
				m_save.isPaused = false;
				GetPlayer()->SetCanSave(false);
			}
		}

		if (input->IsKeyPressed(SGD::Key::UpArrow)){
			if (!m_bInCreatureLog && !m_bInOptionsScreen && m_pause.isPaused){
				if (m_pause.m_nCursor == 1)
					m_pause.m_nCursor = 5;
				else if (m_pause.m_nCursor == 2)
					m_pause.m_nCursor = 1;
				else if (m_pause.m_nCursor == 3)
					m_pause.m_nCursor = 2;
				else if (m_pause.m_nCursor == 4)
					m_pause.m_nCursor = 3;
				else if (m_pause.m_nCursor == 5){
					if (AnaDex->Scanned >= 2)
						m_pause.m_nCursor = 4;
					else
						m_pause.m_nCursor = 3;
				}
			}
			else if (m_save.isPaused){
				if (m_save.m_nCursor == 1)
					m_save.m_nCursor = 4;
				else if (m_save.m_nCursor == 2)
					m_save.m_nCursor = 1;
				else if (m_save.m_nCursor == 3)
					m_save.m_nCursor = 2;
				else
					m_save.m_nCursor = 3;
			}
		}

		if (input->IsKeyPressed(SGD::Key::DownArrow)){
			if (!m_bInCreatureLog && !m_bInOptionsScreen && m_pause.isPaused){
				if (m_pause.m_nCursor == 1)
					m_pause.m_nCursor = 2;
				else if (m_pause.m_nCursor == 2)
					m_pause.m_nCursor = 3;
				else if (m_pause.m_nCursor == 3){
					if (AnaDex->Scanned >= 2)
						m_pause.m_nCursor = 4;
					else
						m_pause.m_nCursor = 5;
				}
				else if (m_pause.m_nCursor == 4)
					m_pause.m_nCursor = 5;
				else if (m_pause.m_nCursor == 5)
					m_pause.m_nCursor = 1;
			}
			else if (m_save.isPaused){
				if (m_save.m_nCursor == 1)
					m_save.m_nCursor = 2;
				else if (m_save.m_nCursor == 2)
					m_save.m_nCursor = 3;
				else if (m_save.m_nCursor == 3)
					m_save.m_nCursor = 4;
				else
					m_save.m_nCursor = 1;
			}
		}

		if (input->IsKeyPressed(SGD::Key::M))	//	Mini map
			TileEngine::GetInstance()->ToggleMinimap();

#pragma region DebugMode
		/*if (input->IsKeyPressed(SGD::Key::F1))
			cam->GetMapLayer()->SetDebugMode(Layer::DEBUG_BASE);

		else if (input->IsKeyPressed(SGD::Key::F2))
			cam->GetMapLayer()->SetDebugMode(Layer::DEBUG_ENVIRONEMNT);

		else if (input->IsKeyPressed(SGD::Key::F3))
			cam->GetMapLayer()->SetDebugMode(Layer::DEBUG_EMITRECTS);

		else if (input->IsKeyPressed(SGD::Key::F4))
			cam->GetMapLayer()->SetDebugMode(Layer::DEBUG_SENSERECTS);

		else if (input->IsKeyPressed(SGD::Key::F5))
			cam->GetMapLayer()->SetDebugMode(Layer::DEBUG_ALL);

		if (input->IsKeyPressed(SGD::Key::F10))
			WorldTransition();*/
#pragma endregion		
#pragma endregion
		break;

	case Game::CTRL_CONTROLLER:
#pragma region Controller
		if (input->IsButtonPressed(0, 7) && arcadebuffer <= 0){	//	Pause			
			arcadebuffer = .2f;
			m_pause.isPaused = true;
			m_save.isPaused = false;
			m_bInCreatureLog = false;
			m_bInOptionsScreen = false;
			m_pause.m_nCursor = 1;
			m_save.m_nCursor = 1;
			m_pause.m_PauseScreen = {
				50, 50,
				Game::GetInstance()->GetScreenRes().width - 50,
				Game::GetInstance()->GetScreenRes().height - 50
			};
		}
		if (input->IsButtonPressed(0, 1) && arcadebuffer <= 0 && m_pause.isPaused){	//	unPause			
			arcadebuffer = .2f;

			if (!m_bInCreatureLog && !m_bInOptionsScreen)
				m_pause.isPaused = false;

			m_bInCreatureLog = false;
			m_bInOptionsScreen = false;
		}

		if (input->IsButtonPressed(0, 0) && m_pause.isPaused){	//	Pause options
			if (m_pause.isPaused && !m_save.isPaused && !m_bInCreatureLog && !m_bInOptionsScreen){
				switch (m_pause.m_nCursor){
				case 1:
					m_pause.isPaused = false;
					m_bInCreatureLog = false;
					m_bInOptionsScreen = false;
					m_save.isPaused = false;
					GetPlayer()->SetCanSave(false);
					break;
				case 2:
					m_pause.m_nCursor = 1;
					m_bInCreatureLog = true;
					break;
				case 3:
					m_bInOptionsScreen = true;
					break;
				case 4:
					WorldTransition();
					break;
				case 5:
					Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
					break;
				}
			}
			else if (m_save.isPaused && !m_pause.isPaused){
				switch (m_save.m_nCursor){
				case 1:
				{
						  AddDialogMsg* msg = new AddDialogMsg("Saving Successful", "Game");
						  msg->SendMessageNow();
						  delete msg;
						  m_SaveSlots->SaveGame("SaveSlot1", GetPlayer(), (int)TimePlayed, m_ObjectManager);
				}
					break;

				case 2:
				{
						  AddDialogMsg* msg = new AddDialogMsg("Saving Successful", "Game");
						  msg->SendMessageNow();
						  delete msg;
						  m_SaveSlots->SaveGame("SaveSlot2", GetPlayer(), (int)TimePlayed, m_ObjectManager);
				}
					break;

				case 3:
				{
						  AddDialogMsg* msg = new AddDialogMsg("Saving Successful", "Game");
						  msg->SendMessageNow();
						  delete msg;
						  m_SaveSlots->SaveGame("SaveSlot3", GetPlayer(), (int)TimePlayed, m_ObjectManager);
				}
					break;

				case 4:
					m_pause.isPaused = true;
					m_pause.m_nCursor = 1;
					m_save.m_nCursor = 1;
					m_pause.m_PauseScreen = {
						50, 50,
						Game::GetInstance()->GetScreenRes().width - 50,
						Game::GetInstance()->GetScreenRes().height - 50
					};
					break;
				}
				m_save.isPaused = false;
				GetPlayer()->SetCanSave(false);
			}
		}

		if (!m_bInCreatureLog && !m_bInOptionsScreen && m_pause.isPaused && input->IsDPadPressed(0, SGD::DPad::Up)){
			if (!m_bInCreatureLog && !m_bInOptionsScreen && m_pause.isPaused){
				if (m_pause.m_nCursor == 1)
					m_pause.m_nCursor = 5;
				else if (m_pause.m_nCursor == 2)
					m_pause.m_nCursor = 1;
				else if (m_pause.m_nCursor == 3)
					m_pause.m_nCursor = 2;
				else if (m_pause.m_nCursor == 4)
					m_pause.m_nCursor = 3;
				else if (m_pause.m_nCursor == 5){
					if (AnaDex->Scanned >= 2)
						m_pause.m_nCursor = 4;
					else
						m_pause.m_nCursor = 3;
				}
			}
			else if (m_save.isPaused){
				if (m_save.m_nCursor == 1)
					m_save.m_nCursor = 4;
				else if (m_save.m_nCursor == 2)
					m_save.m_nCursor = 1;
				else if (m_save.m_nCursor == 3)
					m_save.m_nCursor = 2;
				else
					m_save.m_nCursor = 3;
			}
		}
		if (!m_bInCreatureLog && !m_bInOptionsScreen && m_pause.isPaused && input->IsDPadPressed(0, SGD::DPad::Down)){
			if (!m_bInCreatureLog && !m_bInOptionsScreen && m_pause.isPaused){
				if (m_pause.m_nCursor == 1)
					m_pause.m_nCursor = 2;
				else if (m_pause.m_nCursor == 2)
					m_pause.m_nCursor = 3;
				else if (m_pause.m_nCursor == 3){
					if (AnaDex->Scanned >= 2)
						m_pause.m_nCursor = 4;
					else
						m_pause.m_nCursor = 5;
				}
				else if (m_pause.m_nCursor == 4)
					m_pause.m_nCursor = 5;
				else if (m_pause.m_nCursor == 5)
					m_pause.m_nCursor = 1;
			}
			else if (m_save.isPaused){
				if (m_save.m_nCursor == 1)
					m_save.m_nCursor = 2;
				else if (m_save.m_nCursor == 2)
					m_save.m_nCursor = 3;
				else if (m_save.m_nCursor == 3)
					m_save.m_nCursor = 4;
				else
					m_save.m_nCursor = 1;
			}
		}

		if (input->IsButtonPressed(0, 5))	//	Mini map
			TileEngine::GetInstance()->ToggleMinimap();

#pragma endregion
		break;

	case Game::CTRL_ARCADE:
#pragma region Arcade
		if (input->IsKeyPressed(SGD::Key::MouseRight) && arcadebuffer <= 0){	//	Pause
			arcadebuffer = .2f;
			m_pause.isPaused = true;
			m_save.isPaused = false;
			m_bInCreatureLog = false;
			m_bInOptionsScreen = false;
			m_pause.m_nCursor = 1;
			m_save.m_nCursor = 1;
			m_pause.m_PauseScreen = {
				50, 50,
				Game::GetInstance()->GetScreenRes().width - 50,
				Game::GetInstance()->GetScreenRes().height - 50
			};
		}
		if (input->IsKeyPressed(SGD::Key::MouseLeft) && arcadebuffer <= 0) {	//	Pause options
			arcadebuffer = .2f;
			if (m_pause.isPaused && !m_save.isPaused){
				switch (m_pause.m_nCursor){
				case 1:
					m_pause.isPaused = false;
					m_bInCreatureLog = false;
					m_bInOptionsScreen = false;
					m_save.isPaused = false;
					GetPlayer()->SetCanSave(false);
					break;
				case 2:
					m_pause.m_nCursor = 1;
					m_bInCreatureLog = true;
					break;
				case 3:
					m_bInOptionsScreen = true;
					break;
				case 4:
					WorldTransition();
					break;
				case 5:
					Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
					break;
				}
			}
			else if (m_save.isPaused && !m_pause.isPaused){

				switch (m_save.m_nCursor){
				case 1:
				{
						  AddDialogMsg* msg = new AddDialogMsg("Saving Successful", "Game");
						  msg->SendMessageNow();
						  delete msg;
						  m_SaveSlots->SaveGame("SaveSlot1", GetPlayer(), (int)TimePlayed, m_ObjectManager);
				}
					break;

				case 2:
				{
						  AddDialogMsg* msg = new AddDialogMsg("Saving Successful", "Game");
						  msg->SendMessageNow();
						  delete msg;
						  m_SaveSlots->SaveGame("SaveSlot2", GetPlayer(), (int)TimePlayed, m_ObjectManager);
				}
					break;

				case 3:
				{
						  AddDialogMsg* msg = new AddDialogMsg("Saving Successful", "Game");
						  msg->SendMessageNow();
						  delete msg;
						  m_SaveSlots->SaveGame("SaveSlot3", GetPlayer(), (int)TimePlayed, m_ObjectManager);
				}
					break;

				case 4:
					m_pause.isPaused = true;
					m_pause.m_nCursor = 1;
					m_save.m_nCursor = 1;
					m_pause.m_PauseScreen = {
						50, 50,
						Game::GetInstance()->GetScreenRes().width - 50,
						Game::GetInstance()->GetScreenRes().height - 50
					};
					break;
				}
				m_save.isPaused = false;
				GetPlayer()->SetCanSave(false);
			}
		}

		if (input->GetLeftJoystick(0).ComputeNormalized() == SGD::Vector(0, -1) && arcadebuffer <= 0){
			arcadebuffer = .2f;
			if (!m_bInCreatureLog && !m_bInOptionsScreen && m_pause.isPaused){
				if (m_pause.m_nCursor == 1)
					m_pause.m_nCursor = 5;
				else if (m_pause.m_nCursor == 2)
					m_pause.m_nCursor = 1;
				else if (m_pause.m_nCursor == 3)
					m_pause.m_nCursor = 2;
				else if (m_pause.m_nCursor == 4)
					m_pause.m_nCursor = 3;
				else if (m_pause.m_nCursor == 5){
					if (AnaDex->Scanned >= 2)
						m_pause.m_nCursor = 4;
					else
						m_pause.m_nCursor = 3;
				}
			}
			else if (m_save.isPaused){
				if (m_save.m_nCursor == 1)
					m_save.m_nCursor = 4;
				else if (m_save.m_nCursor == 2)
					m_save.m_nCursor = 1;
				else if (m_save.m_nCursor == 3)
					m_save.m_nCursor = 2;
				else
					m_save.m_nCursor = 3;
			}
		}

		if (input->GetLeftJoystick(0).ComputeNormalized() == SGD::Vector(0,1) && arcadebuffer <= 0){
			arcadebuffer = .2f;
			if (!m_bInCreatureLog && !m_bInOptionsScreen && m_pause.isPaused){
				if (m_pause.m_nCursor == 1)
					m_pause.m_nCursor = 2;
				else if (m_pause.m_nCursor == 2)
					m_pause.m_nCursor = 3;
				else if (m_pause.m_nCursor == 3){
					if (AnaDex->Scanned >= 2)
						m_pause.m_nCursor = 4;
					else
						m_pause.m_nCursor = 5;
				}
				else if (m_pause.m_nCursor == 4)
					m_pause.m_nCursor = 5;
				else if (m_pause.m_nCursor == 5)
					m_pause.m_nCursor = 1;
			}
			else if (m_save.isPaused){
				if (m_save.m_nCursor == 1)
					m_save.m_nCursor = 2;
				else if (m_save.m_nCursor == 2)
					m_save.m_nCursor = 3;
				else if (m_save.m_nCursor == 3)
					m_save.m_nCursor = 4;
				else
					m_save.m_nCursor = 1;
			}
		}
#pragma endregion
		break;
	}
}

void GameplayState::WorldTransition()
{
	SGD::InputManager* input = SGD::InputManager::GetInstance();

	if (TileEngine::GetInstance()->GetWorldNum() != 5){
		CanWin = false;
		AnaDex->Scanned = 0;

		m_pause.m_nCursor = 0;
		m_pause.isPaused = false;

		m_ObjectManager->RemoveACertainBucket(Entity::ENT_ANIMAL);
		m_ObjectManager->RemoveACertainBucket(Entity::ENT_BUSH);

		TileEngine::GetInstance()->SetWorldNum(TileEngine::GetInstance()->GetWorldNum() + 1);

#pragma region Environment
		switch (TileEngine::GetInstance()->GetWorldNum()) {

		case 1:
			TileEngine::GetInstance()->LoadMap(1);
			for (int x = 0; x < 8; ++x) {
				Entity* rook = m_AnimalFactory->CreateRook();
				m_ObjectManager->AddEntity(rook, Entity::ENT_ANIMAL);
			}
			for (int x = 0; x < 10; ++x){
				Entity* pawn = m_AnimalFactory->CreatePawn();
				m_ObjectManager->AddEntity(pawn, Entity::ENT_ANIMAL);
			}
			for (int x = 0; x < 2; ++x){
				Entity* prancer = m_AnimalFactory->CreatePrancer();
				m_ObjectManager->AddEntity(prancer, Entity::ENT_ANIMAL);
			}
			break;

		case 2:
			TileEngine::GetInstance()->LoadMap(2);			
			for (int x = 0; x < 1; ++x){
				Entity* castle = m_AnimalFactory->CreateCastle();
				m_ObjectManager->AddEntity(castle, Entity::ENT_ANIMAL);
			}
			for (int x = 0; x < 4; ++x){
				Entity* prancer = m_AnimalFactory->CreateBumbles();
				m_ObjectManager->AddEntity(prancer, Entity::ENT_ANIMAL);
			}			
			for (int x = 0; x < 3; ++x) {
				Entity* rook = m_AnimalFactory->CreateRook();
				m_ObjectManager->AddEntity(rook, Entity::ENT_ANIMAL);
			}
			for (int x = 0; x < 4; ++x){
				Entity* pawn = m_AnimalFactory->CreatePawn();
				m_ObjectManager->AddEntity(pawn, Entity::ENT_ANIMAL);
			}
			break;

		case 3:
			TileEngine::GetInstance()->LoadMap(3);
			for (int x = 0; x < 7; ++x){
				Entity* rook = m_AnimalFactory->CreateRook();
				m_ObjectManager->AddEntity(rook, Entity::ENT_ANIMAL);
			}
			for (int x = 0; x < 1; ++x){
				Entity* queen = m_AnimalFactory->CreateQueen();
				m_ObjectManager->AddEntity(queen, Entity::ENT_ANIMAL);
			}
			for (int x = 0; x < 3; ++x){
				Entity* pawn = m_AnimalFactory->CreatePawn();
				m_ObjectManager->AddEntity(pawn, Entity::ENT_ANIMAL);
			}		
			for (int x = 0; x < 4; ++x){
				Entity* snake = m_AnimalFactory->CreateBishop();
				m_ObjectManager->AddEntity(snake, Entity::ENT_ANIMAL);
			}
			break;

		case 4:
			TileEngine::GetInstance()->LoadMap(4);
			for (int x = 0; x < 8; ++x){
				Entity* rook = m_AnimalFactory->CreatePassant();
				m_ObjectManager->AddEntity(rook, Entity::ENT_ANIMAL);
			}
			for (int x = 0; x < 3; ++x){
				Entity* dog = m_AnimalFactory->CreateKnight();
				m_ObjectManager->AddEntity(dog, Entity::ENT_ANIMAL);
			}
			for (int x = 0; x < 1; ++x){
				Entity* pawn = m_AnimalFactory->CreatePawn();
				m_ObjectManager->AddEntity(pawn, Entity::ENT_ANIMAL);
			}
			for (int x = 0; x < 1; ++x){
				Entity* rook = m_AnimalFactory->CreateRook();
				m_ObjectManager->AddEntity(rook, Entity::ENT_ANIMAL);
			}
			break;

		case 5:
			TileEngine::GetInstance()->LoadMap(5);
			for (int x = 0; x < 1; ++x){
				Entity* rook = m_AnimalFactory->CreatePassant();
				m_ObjectManager->AddEntity(rook, Entity::ENT_ANIMAL);
			}
			for (int x = 0; x < 1; ++x){
				Entity* king = m_AnimalFactory->CreateKing();
				m_ObjectManager->AddEntity(king, Entity::ENT_ANIMAL);
			}
			for (int x = 0; x < 2; ++x){
				Entity* dog = m_AnimalFactory->CreateKnight();
				m_ObjectManager->AddEntity(dog, Entity::ENT_ANIMAL);
			}
			for (int x = 0; x < 1; ++x){
				Entity* rook = m_AnimalFactory->CreateRook();
				m_ObjectManager->AddEntity(rook, Entity::ENT_ANIMAL);
			}
			for (int x = 0; x < 1; ++x){
				Entity* castle = m_AnimalFactory->CreateCastle();
				m_ObjectManager->AddEntity(castle, Entity::ENT_ANIMAL);
			}
			for (int x = 0; x < 1; ++x){
				Entity* pawn = m_AnimalFactory->CreatePawn();
				m_ObjectManager->AddEntity(pawn, Entity::ENT_ANIMAL);
			}
			break;
		}
#pragma endregion

#pragma region Load bushes
		for (unsigned int i = 0; i < cam->GetMapLayer()->GetBushPoints().size(); i++) {
			TileEngine::GetInstance()->UpdateRender();
			StaticObject* Bush = m_AnimalFactory->CreateBrush(cam->GetMapLayer()->GetBushPoints()[i]);
			m_ObjectManager->AddEntity(Bush, Entity::ENT_BUSH);
		}
#pragma endregion

#pragma region Load Events
		std::vector<SGD::Rectangle> tmp = cam->GetMapLayer()->FindEventTiles("Poison");
		for (unsigned int i = 0; i < tmp.size(); i++){
			TileEngine::GetInstance()->UpdateRender();
			m_ParticleManager->CreatePoisonEmitter(tmp[i].GetBottomRight(), 1.2f, 120, SGD::Color(100, 0, 100));
		}

		tmp.clear();
		tmp = cam->GetMapLayer()->FindEventTiles("SaveEmit");
		for (unsigned int i = 0; i < tmp.size(); i++){
			TileEngine::GetInstance()->UpdateRender();
			m_ParticleManager->CreatePoisonEmitter(tmp[i].GetBottomRight(), 0.5f, 20, SGD::Color(200, 0, 255));
		}
#pragma endregion

		//	Restart Player
		if (TileEngine::GetInstance()->GetWorldNum() != 1)
			GetPlayer()->Trans();
	}
}

void GameplayState::AddToManager(Entity* ent) 
{
	if (ent->GetType() == Entity::ENT_PLAYER){
		Player* p = (Player*)ent;
		SetPlayer(p);
	}
	m_ObjectManager->AddEntity(ent, ent->GetType());
}

string GameplayState::CalculateFps(float elapsedTime)
{
	m_Frames++;
	m_FpsTimer += elapsedTime;

	if (m_FpsTimer >= 1.0f){
		m_FPS = m_Frames;
		m_Frames = 0;
		m_FpsTimer = 0.0f;
	}

	string Result = "FPS ";

	ostringstream convert;

	convert << m_FPS;

	Result += convert.str();

	return Result;
}

void GameplayState::MessageProc(const SGD::Message* pMsg)
{

	// What type of message?
	switch (pMsg->GetMessageID()) {
	case MessageID::MSG_ADD_TO_CREATURE_LOG:
	{
											   const CreatureLogMessage* pCreatureMsg = dynamic_cast<const CreatureLogMessage*>(pMsg);

											   assert(pCreatureMsg != nullptr
												   && "Game::MessageProc - MSG_ADD_TO_CREATURE_LOG is not actually a CreatureLogMessage");

											   if (pCreatureMsg->GetCreatureToAdd() != nullptr)
												   GameplayState::GetInstance()->GetCreatureLogs()->ScanCreature(pCreatureMsg->GetCreatureToAdd()->GetCreature());
											   else{
												   assert(pCreatureMsg->GetCreatureToAdd() != nullptr
													   && "Game::MessageProc - Entity pointer is NULL");
											   }
											   break;
	}

	case MessageID::MSG_DESTROY_ENTITY:
	{

										  const DestroyEntityMessage* pDestroyMsg =
											  dynamic_cast<const DestroyEntityMessage*>(pMsg);
										  GameplayState* gS = GameplayState::GetInstance();
										  assert(pDestroyMsg != nullptr
											  && "Game::MessageProc - MSG_DESTROY_ENTITY is not actually a DestroyEntityMessage");

										  Entity* ptr = pDestroyMsg->GetEntity();

										  for (size_t i = 0; i < GameplayState::GetInstance()->m_ObjectManager->m_tEntities[Entity::ENT_ANIMAL].size(); i++) {

											  Entity* temp = (Entity*)GameplayState::GetInstance()->m_ObjectManager->m_tEntities[Entity::ENT_ANIMAL][i];

											  if (temp->GetTarget() == ptr) {
												  if (temp->GetDataWareHouse()->m_vecLocated.size() > 0) {
													  auto iter = temp->GetDataWareHouse()->m_vecLocated.begin();
													  for (; iter != temp->GetDataWareHouse()->m_vecLocated.end();) {
														  if ((*iter) == ptr) {

															  temp->GetDataWareHouse()->m_vecLocated.erase(iter);
															  break;
														  }
														  iter++;
													  }
												  }
												  temp->SetTarget(nullptr);
											  }
										  }



										  for (size_t i = 0; i < GameplayState::GetInstance()->m_ObjectManager->m_tEntities[Entity::ENT_PACK].size(); i++)
										  {

											  PackComposite* temp = (PackComposite*)GameplayState::GetInstance()->m_ObjectManager->m_tEntities[Entity::ENT_PACK][i];
											  if (temp->GetTarget() == ptr)
												  temp->SetTarget(nullptr);

											  for (size_t j = 0; j < temp->m_vecPack.size(); j++)
											  {
												  Entity* wolf = temp->m_vecPack[j];
												  if (wolf->GetTarget() == ptr)
												  {
													  if (wolf->GetDataWareHouse()->m_vecLocated.size())
													  {
														  auto iter = wolf->GetDataWareHouse()->m_vecLocated.begin();

														  for (; iter != wolf->GetDataWareHouse()->m_vecLocated.end();)
														  {
															  if ((*iter) == ptr)
															  {
																  wolf->GetDataWareHouse()->m_vecLocated.erase(iter);
																  break;
															  }
															  iter++;
														  }
													  }
													  wolf->SetTarget(nullptr);
												  }

											  }

										  }

										  if (ptr->GetType() == Entity::ENT_PLAYER)
										  {

											  GameplayState::GetInstance()->DeadPlayerCount += 1;
											  GameplayState::GetInstance()->DeadPlayer = ptr->GetName();
											  GameplayState::GetInstance()->deadTimer = 8.0f;



											  if (ptr->GetName() == "Brock")
											  {
												  CharacterSelect::GetInstance()->BrockLife = false;
											  }
											  else if (ptr->GetName() == "Ash")
											  {
												  CharacterSelect::GetInstance()->AshLife = false;
											  }
											  else if (ptr->GetName() == "Jenny")
											  {
												  CharacterSelect::GetInstance()->JennyLife = false;
											  }
											  else if (ptr->GetName() == "Todd")
											  {
												  CharacterSelect::GetInstance()->ToddLife = false;
											  }


											  GameplayState::GetInstance()->SetPlayer(nullptr);
											  Camera::GetInstance()->SetCamFocus(nullptr);
											  gS->m_ObjectManager->RemoveACertainBucket(Entity::ENT_PLAYER);

											  CharacterSelect::GetInstance()->ResetCharacterInstantiation();

											  Game::GetInstance()->ChangeState(CharacterSelect::GetInstance());																					  											  
											  break;
										  }
										  break;
	}
	case MessageID::MSG_PASSANT_TURN:
	{

										const PassantTurnMessage* pDestroyMsg =
											dynamic_cast<const PassantTurnMessage*>(pMsg);

										assert(pDestroyMsg != nullptr
											&& "Game::MessageProc - MSG_PASSANT_TURN is not actually a Passant Turn Message");


										if (pDestroyMsg->GetOther())
										{
											Passant* p = (Passant*)pDestroyMsg->GetEntity();
											if (p->SecTimeStamp)
												delete p->SecTimeStamp;
											p->SetTarget(nullptr);
											p->SecTimeStamp = pDestroyMsg->GetOther();
										}
										break;
	}
	case MessageID::MSG_SET_TARGET_PLAYER:
	{
											 const SetPlayerTargetMessage * pCheck = dynamic_cast<const SetPlayerTargetMessage*>(pMsg);

											 assert(pCheck != nullptr
												 && "Not A SET_TARGET_MESSAGE!!!!");

											 GameplayState* GameplayState = GameplayState::GetInstance();
											 if (pCheck->m_pCustomer->GetType() != Entity::ENT_PLAYER)
											 {
												 if (GameplayState::GetInstance()->m_player)
												 {
													 Entity* ptr = pCheck->m_pCustomer;
													 ptr->SetTarget(GameplayState::GetInstance()->m_player);
												 }
											 }

											 break;
	}
	case MessageID::MSG_ADD_DIALOG:
	{
		const AddDialogMsg * pCheck = dynamic_cast<const AddDialogMsg*>(pMsg);
		GameplayState* g = GameplayState::GetInstance();
		g->GetDialogSystem()->AddDialogToQueue(pCheck->dialog, pCheck->sender);
	}
		break;

	case MessageID::MSG_PAUSE_GAME:
		GameplayState::GetInstance()->m_pause.isPaused = true;
		GameplayState::GetInstance()->m_pause.m_nCursor = 1;
		GameplayState::GetInstance()->m_bInOptionsScreen = false;
		GameplayState::GetInstance()->m_pause.m_PauseScreen = {
			50, 50,
			Game::GetInstance()->GetScreenRes().width - 50,
			Game::GetInstance()->GetScreenRes().height - 50
		};
		break;

	default:
		break;
	}
}

void GameplayState::PlayDeathDialogue(std::string person) {
	BitmapFont* pFont = Game::GetInstance()->GetFont();
	SGD::Rectangle screen = cam->GetCameraBounds();
	SGD::Point off = cam->GetCameraOffset();

	string g = "Alas poor ";
	g += person;
	g += ", A great scientist, but a better Friend, There is no one else like ";

	if (person == "Jenny")
		g += "her";
	else
		g += "him";


	// Display the level options centered at 1x scale
	if (!screen.IsEmpty())
		pFont->Draw(g, "Test", { screen.left + 256 - off.x, screen.top + 45 - off.y },
		1.0f, SGD::Color(155, 0, 155));
	if (Dead1PeopleDialogueBool == false)
	{
		string death = "I cannot believe ";
		death += person;
		death += " died just like that...";
		Dead1PeopleDialogueBool = true;
		GetDialogSystem()->AddDialogToQueue(death, GameplayState::GetInstance()->GetPlayer()->GetName());
	}
	if (DeadPlayerCount == 2 && Dead2PeopleDialogueBool == false)
	{
		//I can't believe half of our team is gone
		string twoDead = "I cannot believe that half of our team is gone...";
		GetDialogSystem()->AddDialogToQueue(twoDead, GameplayState::GetInstance()->GetPlayer()->GetName());

		string Response = "We can do this, even without them";
		if (true == CharacterSelect::GetInstance()->BrockLife && GameplayState::GetInstance()->GetPlayer()->GetName() != "Brock" && person != "Brock")
		{
			GetDialogSystem()->AddDialogToQueue(Response, "Brock");
		}
		else if (true == CharacterSelect::GetInstance()->JennyLife && GameplayState::GetInstance()->GetPlayer()->GetName() != "Jenny" && person != "Jenny")
		{
			GetDialogSystem()->AddDialogToQueue(Response, "Jenny");
		}
		else if (true == CharacterSelect::GetInstance()->ToddLife && GameplayState::GetInstance()->GetPlayer()->GetName() != "Todd" && person != "Todd")
		{
			GetDialogSystem()->AddDialogToQueue(Response, "Todd");
		}
		else if (true == CharacterSelect::GetInstance()->AshLife && GameplayState::GetInstance()->GetPlayer()->GetName() != "Ash" && person != "Ash")
		{
			GetDialogSystem()->AddDialogToQueue(Response, "Ash");
		}

		string twoLeft = "Only two of us left...";
		GetDialogSystem()->AddDialogToQueue(twoLeft, GameplayState::GetInstance()->GetPlayer()->GetName());

		Dead2PeopleDialogueBool = true;

	}
	if (DeadPlayerCount == 3 && Dead3PeopleDialogueBool == false)
	{
		//I can't believe I'm the only one left
		string threeDead = "I am the only survivor...";
		GetDialogSystem()->AddDialogToQueue(threeDead, GameplayState::GetInstance()->GetPlayer()->GetName());

		string alone = "All alone in this hostile world...";
		GetDialogSystem()->AddDialogToQueue(alone, GameplayState::GetInstance()->GetPlayer()->GetName());

		string wish = "I do not want to be alone any more";
		GetDialogSystem()->AddDialogToQueue(wish, GameplayState::GetInstance()->GetPlayer()->GetName());

		Dead3PeopleDialogueBool = true;
	}
}