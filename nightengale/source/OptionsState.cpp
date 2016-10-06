#include "OptionsState.h"

#include "MainMenuState.h"

#include "../SGD Wrappers/SGD_String.h"

#include <iostream>
#include <fstream>
using namespace std;


OptionsState* OptionsState::GetInstance(void) {
	static OptionsState s_Instance;
	return &s_Instance;
}

bool OptionsState::Initialize(void) {
	return true;
}

void OptionsState::Terminate(void) {

}

void OptionsState::Enter(void) {
	SGD::AudioManager* audio = SGD::AudioManager::GetInstance();

		// Set background color
	SGD::GraphicsManager::GetInstance()->SetClearColor({ 0, 0, 0 });	

		//	Load audio
	m_background = audio->LoadAudio(L"resource/audio/NightengaleOptionsMenuMusic.xwm");
	m_hOptionsMenuOptionSwitchSFX = audio->LoadAudio(L"resource/audio/NightengaleMenuOptionSwitch.wav");
	m_hOptionsMenuOptionSelectedSFX = audio->LoadAudio(L"resource/audio/NightengaleMenuSelect.wav");

		//	Load Rects
	ReSizeRects();

	audio->PlayAudio(m_background, true); 	
}

void OptionsState::Exit(void) {
	SGD::AudioManager* audio = SGD::AudioManager::GetInstance();

		//	Save Sound Settings
	ofstream fout;
	fout.open("resource/XMLFiles/sound.txt", ios_base::trunc);
	if (fout.is_open() && fout.good()){
		fout << audio->GetMasterVolume(SGD::AudioGroup::Music)
			<< '\t' << audio->GetMasterVolume(SGD::AudioGroup::SoundEffects);
		fout.close();
	}

		//	unload sound
	audio->StopAudio(m_background);
	audio->UnloadAudio(m_background);
	audio->UnloadAudio(m_hOptionsMenuOptionSwitchSFX); 
	audio->UnloadAudio(m_hOptionsMenuOptionSelectedSFX); 
}

bool OptionsState::Update(float elapsedTime) {

	if (arcadebuffer >= 0)
		arcadebuffer -= elapsedTime;

	ReSizeRects();

	return Input();		//	keep looping
}

void OptionsState::Render(float elapsedTime) {
	SGD::GraphicsManager* g = SGD::GraphicsManager::GetInstance();
	BitmapFont* pFont = Game::GetInstance()->GetFont();
	SGD::Size screenres = Game::GetInstance()->GetScreenRes();

	//	Draw options rect
	g->DrawRectangle(m_optionRect, SGD::Color(50, 50, 50), SGD::Color(0, 0, 150));

	//	Set rects for volume levels
	SGD::Rectangle m_MastervolLevelRect = { m_MastervolRect.left, 200.0f,
		m_MastervolRect.left + (SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::Music) * 2.0f) + 1.0f,
		220.0f
	};
	SGD::Rectangle m_SFXvolLevelRect = { m_SFXvolRect.left, 250.0f,
		m_SFXvolRect.left + (SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::SoundEffects) * 2.0f) + 1.0f,
		270.0f
	};

	//	OString display volume levels
	SGD::OStringStream musiclevel;
	SGD::OStringStream sfxlevel;

	musiclevel << SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::Music);
	sfxlevel << SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::SoundEffects);

#pragma region Render OptionText
	pFont->Draw("Options", "Game", { 200, 50 },
		1.5f, { 0, 0, 150 });

	if (m_nCursor == 0){
		pFont->Draw("- Music Volume : ", "Game", { screenres.width / 2 - 100, 200 }, 0.5f, SGD::Color(0, 0, 255));
		pFont->Draw(musiclevel.str().c_str(), "Game", { screenres.width / 2 + 100, 200 }, 0.5f, SGD::Color(0, 0, 255));
	}
	else{
		pFont->Draw("Music Volume : ", "Game", { screenres.width / 2 - 100, 200 }, 0.5f, SGD::Color(0, 0, 0));
		pFont->Draw(musiclevel.str().c_str(), "Game", { screenres.width / 2 + 100, 200 }, 0.5f, SGD::Color(0, 0, 0));
	}
	if (m_nCursor == 1){
		pFont->Draw("- Sfx Volume : ", "Game", { screenres.width / 2 - 100, 250 }, 0.5f, SGD::Color(0, 0, 255));
		pFont->Draw(sfxlevel.str().c_str(), "Game", { screenres.width / 2 + 100, 250 }, 0.5f, SGD::Color(0, 0, 255));
	}
	else{
		pFont->Draw("Sfx Volume : ", "Game", { screenres.width / 2 - 100, 250 }, 0.5f, SGD::Color(0, 0, 0));
		pFont->Draw(sfxlevel.str().c_str(), "Game", { screenres.width / 2 + 100, 250 }, 0.5f, SGD::Color(0, 0, 0));
	}

	if (m_nCursor == 2){
		pFont->Draw("- FullScreen : ", "Game", { screenres.width / 2 - 100, 300 }, 0.5f, SGD::Color(0, 0, 255));
		if (Game::GetInstance()->GetCurrentRes() == Game::RES_FULLSCREEN)
			pFont->Draw("Enabled", "Game", { screenres.width / 2 + 100, 300 }, 0.5f, SGD::Color(0, 0, 255));
		else
			pFont->Draw("Disabled", "Game", { screenres.width / 2 + 100, 300 }, 0.5f, SGD::Color(0, 0, 255));
	}
	else{
		pFont->Draw("FullScreen : ", "Game", { screenres.width / 2 - 100, 300 }, 0.5f, SGD::Color(0, 0, 0));
		if (Game::GetInstance()->GetCurrentRes() == Game::RES_FULLSCREEN)
			pFont->Draw("Enabled", "Game", { screenres.width / 2 + 100, 300 }, 0.5f, SGD::Color(0, 0, 0));
		else
			pFont->Draw("Disabled", "Game", { screenres.width / 2 + 100, 300 }, 0.5f, SGD::Color(0, 0, 0));
	}
#pragma endregion

	//	Draw Music Rect
	g->DrawRectangle(m_MastervolRect, SGD::Color(255, 0, 0, 0),
		SGD::Color(0, 0, 0), 1);

	g->DrawRectangle(m_MastervolLevelRect, SGD::Color(100, 0, 0, 255));

	//	Draw Sfx Rect
	g->DrawRectangle(m_SFXvolRect, SGD::Color(255, 0, 0, 0),
		SGD::Color(0, 0, 0), 1);

	g->DrawRectangle(m_SFXvolLevelRect, SGD::Color(100, 0, 0, 255));
}

bool OptionsState::Input(){
	SGD::InputManager* input = SGD::InputManager::GetInstance();
	SGD::AudioManager* audio = SGD::AudioManager::GetInstance();
	SGD::Point cursor = SGD::InputManager::GetInstance()->GetCursorPosition();
	SGD::Rectangle cursorrect = { cursor.x, cursor.y, cursor.x + 1, cursor.y + 1 };

	switch (Game::GetInstance()->GetCurrentInput()){

	case Game::CTRL_KEYBOARD:
#pragma region Keyboard
		if (input->IsKeyPressed(SGD::Key::Escape)) {
			Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
			return true;
		}
		if (input->IsKeyPressed(SGD::Key::UpArrow) || input->IsKeyPressed(SGD::Key::W)){
			if (m_nCursor == 0)
				m_nCursor = 2;
			else if (m_nCursor == 1)
				m_nCursor = 0;
			else if (m_nCursor == 2)
				m_nCursor = 1;
		}
		else if (input->IsKeyPressed(SGD::Key::DownArrow) || input->IsKeyPressed(SGD::Key::S)){
			if (m_nCursor == 0)
				m_nCursor = 1;
			else if (m_nCursor == 1)
				m_nCursor = 2;
			else if (m_nCursor == 2)
				m_nCursor = 0;
		}
		if (input->IsKeyPressed(SGD::Key::LeftArrow) || input->IsKeyPressed(SGD::Key::A)) {
			if (m_nCursor == 0){
				audio->SetMasterVolume(SGD::AudioGroup::Music,
					audio->GetMasterVolume(SGD::AudioGroup::Music) - 5);
			}
			else if (m_nCursor == 1){
				audio->SetMasterVolume(SGD::AudioGroup::SoundEffects,
					audio->GetMasterVolume(SGD::AudioGroup::SoundEffects) - 5);
				audio->PlayAudio(m_hOptionsMenuOptionSwitchSFX, false);
			}
			else if (m_nCursor == 2){
				if (Game::GetInstance()->GetCurrentRes() != Game::RES_FULLSCREEN)
					Game::GetInstance()->SetCurrentRes(Game::RES_FULLSCREEN);
				else
					Game::GetInstance()->SetCurrentRes(Game::RES_BASE);
			}
		}
		else if (input->IsKeyPressed(SGD::Key::RightArrow) || input->IsKeyPressed(SGD::Key::D)) {
			if (m_nCursor == 0){
				audio->SetMasterVolume(SGD::AudioGroup::Music,
					audio->GetMasterVolume(SGD::AudioGroup::Music) + 5);
			}
			else if (m_nCursor == 1){
				audio->SetMasterVolume(SGD::AudioGroup::SoundEffects,
					audio->GetMasterVolume(SGD::AudioGroup::SoundEffects) + 5);
				audio->PlayAudio(m_hOptionsMenuOptionSwitchSFX, false);
			}
			else if (m_nCursor == 2){
				if (Game::GetInstance()->GetCurrentRes() != Game::RES_FULLSCREEN)
					Game::GetInstance()->SetCurrentRes(Game::RES_FULLSCREEN);
				else
					Game::GetInstance()->SetCurrentRes(Game::RES_BASE);
			}
		}
		if (cursorrect.IsIntersecting(m_MastervolRect) && input->IsKeyPressed(SGD::Key::MouseLeft) == true)
			audio->SetMasterVolume(SGD::AudioGroup::Music, (int)((cursor.x - m_MastervolRect.left) / 2));
		
		else if (cursorrect.IsIntersecting(m_SFXvolRect) && input->IsKeyPressed(SGD::Key::MouseLeft) == true)
			audio->SetMasterVolume(SGD::AudioGroup::SoundEffects, (int)((cursor.x - m_SFXvolRect.left) / 2));

		if (m_nCursor == 2 && input->IsKeyPressed(SGD::Key::Enter)){
			if (Game::GetInstance()->GetCurrentRes() != Game::RES_FULLSCREEN)
				Game::GetInstance()->SetCurrentRes(Game::RES_FULLSCREEN);
			else
				Game::GetInstance()->SetCurrentRes(Game::RES_BASE);
		}
#pragma endregion
		break;

	case Game::CTRL_CONTROLLER:
#pragma region Controller
		if (input->IsButtonPressed(0, 1)) {
			Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
			return true;
		}
		if (input->IsDPadPressed(0, SGD::DPad::Up)){
			if (m_nCursor == 0)
				m_nCursor = 2;
			else if (m_nCursor == 1)
				m_nCursor = 0;
			else if (m_nCursor == 2)
				m_nCursor = 1;
		}
		else if (input->IsDPadPressed(0, SGD::DPad::Down)){
			if (m_nCursor == 0)
				m_nCursor = 1;
			else if (m_nCursor == 1)
				m_nCursor = 2;
			else if (m_nCursor == 2)
				m_nCursor = 0;
		}
		if (input->IsDPadPressed(0, SGD::DPad::Left)) {
			if (m_nCursor == 0){
				audio->SetMasterVolume(SGD::AudioGroup::Music,
					audio->GetMasterVolume(SGD::AudioGroup::Music) - 5);
			}
			else if (m_nCursor == 1){
				audio->SetMasterVolume(SGD::AudioGroup::SoundEffects,
					audio->GetMasterVolume(SGD::AudioGroup::SoundEffects) - 5);
				audio->PlayAudio(m_hOptionsMenuOptionSwitchSFX, false);
			}
			else if (m_nCursor == 2){
				if (Game::GetInstance()->GetCurrentRes() != Game::RES_FULLSCREEN)
					Game::GetInstance()->SetCurrentRes(Game::RES_FULLSCREEN);
				else
					Game::GetInstance()->SetCurrentRes(Game::RES_BASE);
			}
		}
		else if (input->IsDPadPressed(0, SGD::DPad::Right)) {
			if (m_nCursor == 0){
				audio->SetMasterVolume(SGD::AudioGroup::Music,
					audio->GetMasterVolume(SGD::AudioGroup::Music) + 5);
			}
			else if (m_nCursor == 1){
				audio->SetMasterVolume(SGD::AudioGroup::SoundEffects,
					audio->GetMasterVolume(SGD::AudioGroup::SoundEffects) + 5);
				audio->PlayAudio(m_hOptionsMenuOptionSwitchSFX, false);
			}
			else if (m_nCursor == 2){
				if (Game::GetInstance()->GetCurrentRes() != Game::RES_FULLSCREEN)
					Game::GetInstance()->SetCurrentRes(Game::RES_FULLSCREEN);
				else
					Game::GetInstance()->SetCurrentRes(Game::RES_BASE);
			}
		}
		if (m_nCursor == 2 && input->IsButtonPressed(0, 0)){
			if (Game::GetInstance()->GetCurrentRes() != Game::RES_FULLSCREEN)
				Game::GetInstance()->SetCurrentRes(Game::RES_FULLSCREEN);
			else
				Game::GetInstance()->SetCurrentRes(Game::RES_BASE);
		}
#pragma endregion
		break;

	case Game::CTRL_ARCADE:
#pragma region Arcade
		if (input->IsKeyPressed(SGD::Key::MouseRight)) {
			Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
			return true;
		}
		if (input->GetLeftJoystick(0).y > 0 && arcadebuffer <= 0){
			arcadebuffer = 0.2f;
			if (m_nCursor == 0)
				m_nCursor = 1;
			else if (m_nCursor == 1)
				m_nCursor = 0;
		}
		else if (input->GetLeftJoystick(0).y < 0 && arcadebuffer <= 0){
			arcadebuffer = 0.2f;
			if (m_nCursor == 0)
				m_nCursor = 1;
			else if (m_nCursor == 1)
				m_nCursor = 0;
		}
		if (input->GetLeftJoystick(0).x < 0 && arcadebuffer <= 0) {
			arcadebuffer = 0.2f;
			if (m_nCursor == 0){
				audio->SetMasterVolume(SGD::AudioGroup::Music,
					audio->GetMasterVolume(SGD::AudioGroup::Music) - 5);
			}
			else if (m_nCursor == 1){
				audio->SetMasterVolume(SGD::AudioGroup::SoundEffects,
					audio->GetMasterVolume(SGD::AudioGroup::SoundEffects) - 5);
				audio->PlayAudio(m_hOptionsMenuOptionSwitchSFX, false);
			}
		}
		else if (input->GetLeftJoystick(0).x > 0 && arcadebuffer <= 0) {
			arcadebuffer = 0.2f;
			if (m_nCursor == 0){
				audio->SetMasterVolume(SGD::AudioGroup::Music,
					audio->GetMasterVolume(SGD::AudioGroup::Music) + 5);
			}
			else if (m_nCursor == 1){
				audio->SetMasterVolume(SGD::AudioGroup::SoundEffects,
					audio->GetMasterVolume(SGD::AudioGroup::SoundEffects) + 5);
				audio->PlayAudio(m_hOptionsMenuOptionSwitchSFX, false);
			}
		}
#pragma endregion
		break;
	}
	return true;
}

void OptionsState::ReSizeRects(){

	SGD::Size screenres = Game::GetInstance()->GetScreenRes();

	m_optionRect = { screenres.width / 10, screenres.height / 10, screenres.width - (screenres.width / 10), screenres.height - (screenres.height / 10) };
	m_MastervolRect = { screenres.width / 2 - 100, 200.0f, screenres.width / 2 + 100, 220.0f };
	m_SFXvolRect = { screenres.width / 2 - 100, 250.0f, screenres.width / 2 + 100, 270.0f };
}