#include "LoadMenuState.h"

#include "CharacterSelect.h"
#include "MainMenuState.h"
#include "GameplayState.h"
#include "SaveSlots.h"


LoadMenuState* LoadMenuState::GetInstance() {
	static LoadMenuState s_Instance;
	return &s_Instance;
}

bool LoadMenuState::Initialize() {
	return true;
}

void LoadMenuState::Terminate() {

}

void LoadMenuState::Enter() {
	SGD::AudioManager* audio = SGD::AudioManager::GetInstance();
	SGD::GraphicsManager::GetInstance()->SetClearColor({ 50, 50, 50 });

	m_fScreenWidth = Game::GetInstance()->GetScreenRes().width / 2;
	m_fScreenHeight = Game::GetInstance()->GetScreenRes().height / 2;

	m_nCursor = 0;

	m_svSlot1 = SGD::Rectangle(SGD::Point(150, 50), SGD::Size(500, 100));
	m_svSlot2 = SGD::Rectangle(SGD::Point(150, 170), SGD::Size(500, 100));
	m_svSlot3 = SGD::Rectangle(SGD::Point(150, 290), SGD::Size(500, 100));
	m_svExit = SGD::Rectangle(SGD::Point(250, 410), SGD::Size(300, 100));

	m_hLoadScreenBGM = audio->LoadAudio(L"resource/audio/NightengaleLoadMenuMusic.xwm"); 
	m_hLoadOptionSwitchSFX = audio->LoadAudio(L"resource/audio/NightengaleMenuOptionSwitch.wav"); 
	m_hLoadOptionSelectionSFX = audio->LoadAudio(L"resource/audio/NightengaleMenuSelect.wav"); 

	audio->PlayAudio(m_hLoadScreenBGM, true);

	save1 = new SaveSlotInfo;
	save2 = new SaveSlotInfo;
	save3 = new SaveSlotInfo;

	SaveSlots* loadvars = new SaveSlots;
	loadvars->LoadGameInfo("SaveSlot1.xml", save1);
	loadvars->LoadGameInfo("SaveSlot2.xml", save2);
	loadvars->LoadGameInfo("SaveSlot3.xml", save3);

	delete loadvars;
	loadvars = nullptr;
}

void LoadMenuState::Exit(void) {
	SGD::AudioManager* audio = SGD::AudioManager::GetInstance();

	audio->StopAudio(m_hLoadScreenBGM);
	audio->UnloadAudio(m_hLoadScreenBGM); 
	audio->UnloadAudio(m_hLoadOptionSwitchSFX); 
	audio->UnloadAudio(m_hLoadOptionSelectionSFX); 

	delete save1;
	delete save2;
	delete save3;
}

bool LoadMenuState::Update(float elapsedTime) {
	if (arcadebuffer >= 0)
		arcadebuffer -= elapsedTime;

	return Input();
}

void LoadMenuState::Render(float elapsedTime) {
	BitmapFont* pFont = Game::GetInstance()->GetFont();

#pragma region Render Load Slots
	SGD::Point load1, load2, load3;

	load1 = m_svSlot1.ComputeCenter();
	load2 = m_svSlot2.ComputeCenter();
	load3 = m_svSlot3.ComputeCenter();

	SGD::GraphicsManager::GetInstance()->DrawRectangle(m_svSlot1, SGD::Color(100, 0, 0, 0));
	SGD::GraphicsManager::GetInstance()->DrawRectangle(m_svSlot2, SGD::Color(100, 0, 0, 0));
	SGD::GraphicsManager::GetInstance()->DrawRectangle(m_svSlot3, SGD::Color(100, 0, 0, 0));
	SGD::GraphicsManager::GetInstance()->DrawRectangle(m_svExit, SGD::Color(100, 0, 0, 0));

	pFont->Draw(save1->charName, "Game", { load1.x, load1.y - 20 }, 1.2f, { 255, 0, 0 });
	pFont->Draw(save2->charName, "Game", { load2.x, load2.y - 20 }, 1.2f, { 255, 0, 0 });
	pFont->Draw(save3->charName, "Game", { load3.x, load3.y - 20 }, 1.2f, { 255, 0, 0 });
	pFont->Draw("Exit", "Game", { m_svExit.ComputeCenter().x - 75, m_svExit.ComputeCenter().y }, 2, { 255, 0, 0 });
#pragma endregion

#pragma region Render_Selection
	if (m_nCursor == 0){
		SGD::GraphicsManager::GetInstance()->DrawRectangle(m_svSlot1, SGD::Color(100, 0, 0, 0), SGD::Color(255, 0, 0));
		if (save1->charName == "")
			pFont->Draw("Empty", "Game", { load1.x, load1.y }, 1.5f, { 255, 0, 255 });
		else {
			std::string worldnum = "World " + std::to_string(save1->Worldnum);
			std::string gameTime = "Time Played " + std::to_string(save1->elapsedTime) + " sec";

			pFont->Draw(save1->charName, "Game", { load1.x, load1.y - 20 }, 1.2f, { 255, 0, 255 });
			pFont->Draw(worldnum, "Game", { m_svSlot1.left + 5, load1.y + 2 }, .8f, { 255, 0, 0 });
			pFont->Draw(gameTime, "Game", { m_svSlot1.right - 350, load1.y + 25 }, .8f, { 255, 0, 0 });
		}
	}
	else if (m_nCursor == 1){
		SGD::GraphicsManager::GetInstance()->DrawRectangle(m_svSlot2, SGD::Color(100, 0, 0, 0), SGD::Color(255, 0, 0));
		if (save2->charName == "")
			pFont->Draw("Empty", "Game", { load2.x, load2.y }, 1.5f, { 255, 0, 255 });
		else{
			std::string worldnum = "World " + std::to_string(save2->Worldnum);
			std::string gameTime = "Time Played " + std::to_string(save2->elapsedTime) + " sec";

			pFont->Draw(save2->charName, "Game", { load2.x, load2.y - 20 }, 1.2f, { 255, 0, 255 });
			pFont->Draw(worldnum, "Game", { m_svSlot2.left + 5, load2.y + 2 }, .8f, { 255, 0, 0 });
			pFont->Draw(gameTime, "Game", { m_svSlot2.right - 350, load2.y + 25 }, .8f, { 255, 0, 0 });
		}
	}
	else if (m_nCursor == 2){
		SGD::GraphicsManager::GetInstance()->DrawRectangle(m_svSlot3, SGD::Color(100, 0, 0, 0), SGD::Color(255, 0, 0));
		if (save3->charName == "")
			pFont->Draw("Empty", "Game", { load3.x, load3.y }, 1.5f, { 255, 0, 255 });
		else{
			std::string worldnum = "World " + std::to_string(save3->Worldnum);
			std::string gameTime = "Time Played " + std::to_string(save3->elapsedTime) + " sec";

			pFont->Draw(save3->charName, "Game", { load3.x, load3.y - 20 }, 1.2f, { 255, 0, 255 });
			pFont->Draw(worldnum, "Game", { m_svSlot3.left + 5, load3.y + 2 }, .8f, { 255, 0, 0 });
			pFont->Draw(gameTime, "Game", { m_svSlot3.right - 350, load3.y + 25 }, .8f, { 255, 0, 0 });
		}
	}
	else {
		SGD::GraphicsManager::GetInstance()->DrawRectangle(m_svExit, SGD::Color(100, 0, 0, 0), SGD::Color(255, 0, 0));
		pFont->Draw("Exit", "Game", { m_svExit.ComputeCenter().x - 75, m_svExit.ComputeCenter().y }, 2, { 255, 0, 255 });
	}
#pragma endregion
}

bool LoadMenuState::Input() {
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	SGD::AudioManager* audio = SGD::AudioManager::GetInstance();


	switch (Game::GetInstance()->GetCurrentInput()){

	case Game::CTRL_KEYBOARD:
#pragma region Keyboard
		if (pInput->IsKeyPressed(SGD::Key::Escape)) {
			audio->PlayAudio(m_hLoadOptionSwitchSFX, false);
			m_nCursor = 3;
		}
		if (pInput->IsKeyPressed(SGD::Key::Up) || pInput->IsKeyPressed(SGD::Key::W)) {
			audio->PlayAudio(m_hLoadOptionSwitchSFX, false);
			--m_nCursor;
			if (m_nCursor < 0)
				m_nCursor = 3;
		}
		else if (pInput->IsKeyPressed(SGD::Key::Down) || pInput->IsKeyPressed(SGD::Key::S)) {
			audio->PlayAudio(m_hLoadOptionSwitchSFX, false);
			++m_nCursor;
			if (m_nCursor > 3)
				m_nCursor = 0;
		}
		if (pInput->IsKeyPressed(SGD::Key::Enter)) {
			audio->PlayAudio(m_hLoadOptionSelectionSFX, false);
			GameplayState::GetInstance()->SetLoadFile("");
			Selection();
		}
#pragma endregion
		break;

	case Game::CTRL_CONTROLLER:
#pragma region Controller
		if (pInput->IsButtonPressed(0, 1)) {
			audio->PlayAudio(m_hLoadOptionSwitchSFX, false);
			m_nCursor = 3;
		}
		if (pInput->IsDPadPressed(0, SGD::DPad::Up)) {
			audio->PlayAudio(m_hLoadOptionSwitchSFX, false);
			--m_nCursor;
			if (m_nCursor < 0)
				m_nCursor = 3;
		}
		else if (pInput->IsDPadPressed(0, SGD::DPad::Down)) {
			audio->PlayAudio(m_hLoadOptionSwitchSFX, false);
			++m_nCursor;
			if (m_nCursor > 3)
				m_nCursor = 0;
		}
		if (pInput->IsButtonPressed(0, 0)) {
			audio->PlayAudio(m_hLoadOptionSelectionSFX, false);
			GameplayState::GetInstance()->SetLoadFile("");
			Selection();
		}
#pragma endregion
		break;

	case Game::CTRL_ARCADE:
#pragma region Arcade
		if (pInput->IsKeyPressed(SGD::Key::MouseRight)) {
			audio->PlayAudio(m_hLoadOptionSwitchSFX, false);
			m_nCursor = 3;
		}
		if (pInput->GetLeftJoystick(0).y < 0 && arcadebuffer <= 0) {
			audio->PlayAudio(m_hLoadOptionSwitchSFX, false);
			arcadebuffer = 0.2f;
			--m_nCursor;
			if (m_nCursor < 0)
				m_nCursor = 3;
		}
		else if (pInput->GetLeftJoystick(0).y > 0 && arcadebuffer <= 0) {
			audio->PlayAudio(m_hLoadOptionSwitchSFX, false);
			arcadebuffer = 0.2f;
			++m_nCursor;
			if (m_nCursor > 3)
				m_nCursor = 0;
		}
		if (pInput->IsKeyPressed(SGD::Key::MouseLeft)) {
			audio->PlayAudio(m_hLoadOptionSelectionSFX, false);
			GameplayState::GetInstance()->SetLoadFile("");
			Selection();
		}
#pragma endregion
		break;
	}
	return true;
}

bool LoadMenuState::Selection(){
	switch (m_nCursor){

	case 0:
		if (save1->charName != "")
			GameplayState::GetInstance()->SetLoadFile("SaveSlot1.xml");
		break;

	case 1:
		if (save2->charName != "")
			GameplayState::GetInstance()->SetLoadFile("SaveSlot2.xml");
		break;

	case 2:
		if (save3->charName != "")
			GameplayState::GetInstance()->SetLoadFile("SaveSlot3.xml");
		break;

	case 3:
		SGD::AudioManager::GetInstance()->StopAudio(m_hLoadScreenBGM);
		Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
		return true;
		break;
	}
	if (GameplayState::GetInstance()->GetLoadFile() != ""){
		SGD::AudioManager::GetInstance()->StopAudio(m_hLoadScreenBGM);
		GameplayState::GetInstance()->Initialize();
		CharacterSelect::GetInstance()->CharacterIsInstantiated();
		Game::GetInstance()->ChangeState(GameplayState::GetInstance());
		return true;
	}
	return true;
}