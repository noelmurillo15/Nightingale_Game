#include "MainMenuState.h"

#include "HowtoplayState.h"
#include "OptionsState.h"
#include "CharacterSelect.h"
#include "CreditsState.h"
#include "LoadMenuState.h"
#include "ControllerState.h"

#include <iostream>
#include <fstream>
#include "tinyxml.h"
using namespace std;

#include <atlstr.h>
#include "../resource.h"


MainMenuState* MainMenuState::GetInstance(void) {
	static MainMenuState s_Instance;
	return &s_Instance;
}

bool MainMenuState::Initialize(void)
{
		//	Initialize all states
	CreditsState::GetInstance()->Initialize();
	LoadMenuState::GetInstance()->Initialize();
	OptionsState::GetInstance()->Initialize();
	HowtoplayState::GetInstance()->Initialize();
	ControllerState::GetInstance()->Initialize();
	CharacterSelect::GetInstance()->Initialize();	

		//	Load Master Volumes from sound file
	ifstream fin;
	fin.open("resource/XMLFiles/sound.txt");
	if (fin.is_open()) {
		int musicvol, sfxvol;

		fin >> musicvol;
		fin.ignore(INT_MAX, '\t');
		fin >> sfxvol;

		SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::Music, musicvol);
		SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::SoundEffects, sfxvol);

		fin.close();
	}
	return true;
}

void MainMenuState::Terminate(void)
{
	CharacterSelect::GetInstance()->Terminate();
	CreditsState::GetInstance()->Terminate();
	LoadMenuState::GetInstance()->Terminate();
	OptionsState::GetInstance()->Terminate();
	ControllerState::GetInstance()->Terminate();
	HowtoplayState::GetInstance()->Terminate();	
}

void MainMenuState::Enter(void) {

	m_MainScreen = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/Wallpaper.png");
	m_hMainMenuMusic = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/NightengaleMainMenuMusic.xwm");
	m_hMenuChangeSFX = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/NightengaleMenuOptionSwitch.wav");
	m_hMenuSelectionSFX = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/NightengaleMenuSelect.wav");

	SGD::AudioManager::GetInstance()->PlayAudio(m_hMainMenuMusic, true);
}

void MainMenuState::Exit(void) {
	SGD::AudioManager::GetInstance()->StopAudio(m_hMainMenuMusic);

	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_MainScreen);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hMainMenuMusic);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hMenuChangeSFX);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hMenuSelectionSFX);
}

bool MainMenuState::Update(float elapsedTime) {
	if (arcadebuffer >= 0)
		arcadebuffer -= elapsedTime;

	return Input();
}

void MainMenuState::Render(float elapsedTime) {

	SGD::GraphicsManager* graphics = SGD::GraphicsManager::GetInstance();
	BitmapFont* pFont = Game::GetInstance()->GetFont();
	SGD::Size screenres = Game::GetInstance()->GetScreenRes();

	SGD::Size scale = screenres;
	scale.width /= 1000;
	scale.height /= 1000;

	CString play, howToPlay, Options, Credits, Load, Exit;
	play.LoadStringW(IDS_STRINGPLAY);
	howToPlay.LoadStringW(IDS_STRINGHTP);
	Options.LoadStringW(IDS_STRINGOPTIONS);
	Credits.LoadStringW(IDS_STRINGCREDITS);
	Load.LoadStringW(IDS_STRINGLOAD);
	Exit.LoadStringW(IDS_STRINGEXIT);

	graphics->DrawTexture(m_MainScreen, SGD::Point(0, 0), 0, {}, {}, scale);

		//	Render menu options
	pFont->Draw(play, "Game", { (screenres.width - (4 * 32)) / 2, (screenres.height / 3.75f) },
		1.0f, { 255, 0, 0 });
	pFont->Draw(howToPlay, "Game", { (screenres.width - (4 * 32)) / 2, (screenres.height / 3.75f) + 50 },
		1.0f, { 255, 0, 0 });
	pFont->Draw(Options, "Game", { (screenres.width - (4 * 32)) / 2, (screenres.height / 3.75f) + 100 },
		1.0f, { 255, 0, 0 });
	pFont->Draw(Credits, "Game", { (screenres.width - (4 * 32)) / 2, (screenres.height / 3.75f) + 150 },
		1.0f, { 255, 0, 0 });
	pFont->Draw(Load, "Game", { (screenres.width - (4 * 32)) / 2, (screenres.height / 3.75f) + 200 },
		1.0f, { 255, 0, 0 });
	pFont->Draw(Exit, "Game", { (screenres.width - (4 * 32)) / 2, (screenres.height / 3.75f) + 250 },
		1.0f, { 255, 0, 0 });


	if (m_nCursor == 0)
		pFont->Draw(play, "Game", { (screenres.width - (4 * 32)) / 2, (screenres.height / 3.75f) },
		1.0f, { 255, 155, 0 });

	else if (m_nCursor == 1)
		pFont->Draw(howToPlay, "Game", { (screenres.width - (4 * 32)) / 2, (screenres.height / 3.75f) + 50 },
		1.0f, { 255, 155, 0 });

	else if (m_nCursor == 2)
		pFont->Draw(Options, "Game", { (screenres.width - (4 * 32)) / 2, (screenres.height / 3.75f) + 100 },
		1.0f, { 255, 155, 0 });

	else if (m_nCursor == 3)
		pFont->Draw(Credits, "Game", { (screenres.width - (4 * 32)) / 2, (screenres.height / 3.75f) + 150 },
		1.0f, { 255, 155, 0 });

	else if (m_nCursor == 4)
		pFont->Draw(Load, "Game", { (screenres.width - (4 * 32)) / 2, (screenres.height / 3.75f) + 200 },
		1.0f, { 255, 155, 0 });

	else if (m_nCursor == 5)
		pFont->Draw(Exit, "Game", { (screenres.width - (4 * 32)) / 2, (screenres.height / 3.75f) + 250 },
		1.0f, { 255, 155, 0 });
}

bool MainMenuState::Input(){
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	switch (Game::GetInstance()->GetCurrentInput()){

	case Game::CTRL_KEYBOARD:
#pragma region Main Menu
		if (pInput->IsKeyPressed(SGD::Key::Alt) && pInput->IsKeyPressed(SGD::Key::Tab))
		{
			int x = 0;
		}
		if (pInput->IsKeyPressed(SGD::Key::Escape)) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hMenuChangeSFX, false);
			m_nCursor = 5;
		}
		if (pInput->IsKeyPressed(SGD::Key::Down) || pInput->IsKeyPressed(SGD::Key::S)) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hMenuChangeSFX, false);

			++m_nCursor;

			if (m_nCursor > 5)
				m_nCursor = 0;
		}
		else if (pInput->IsKeyPressed(SGD::Key::Up) || pInput->IsKeyPressed(SGD::Key::W)) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hMenuChangeSFX, false);

			--m_nCursor;

			if (m_nCursor < 0)
				m_nCursor = 5;
		}
		if (pInput->IsKeyPressed(SGD::Key::Enter)) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hMenuSelectionSFX, false);

			switch (m_nCursor){
			case 0:
				Game::GetInstance()->ChangeState(CharacterSelect::GetInstance());
				return true;
				break;

			case 1:
				Game::GetInstance()->ChangeState(HowtoplayState::GetInstance());
				return true;
				break;

			case 2:
				Game::GetInstance()->ChangeState(OptionsState::GetInstance());
				return true;
				break;

			case 3:
				Game::GetInstance()->ChangeState(CreditsState::GetInstance());
				return true;
				break;

			case 4:
				Game::GetInstance()->ChangeState(LoadMenuState::GetInstance());
				return true;
				break;

			case 5:
				return false;
				break;

			default:
				break;
			}
		}

#pragma endregion
		break;

	case Game::CTRL_CONTROLLER:
#pragma region Main Menu

		if (pInput->IsButtonPressed(0, 1)) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hMenuChangeSFX, false);
			m_nCursor = 5;
		}
		if (pInput->IsDPadPressed(0, SGD::DPad::Down)) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hMenuChangeSFX, false);

			++m_nCursor;
			if (m_nCursor > 5)
				m_nCursor = 0;
		}
		else if (pInput->IsDPadPressed(0, SGD::DPad::Up)) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hMenuChangeSFX, false);

			--m_nCursor;
			if (m_nCursor < 0)
				m_nCursor = 5;
		}
		if (pInput->IsButtonPressed(0, 0)) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hMenuSelectionSFX, false);

			switch (m_nCursor){

			case 0:
				Game::GetInstance()->ChangeState(CharacterSelect::GetInstance());
				return true;
				break;

			case 1:
				Game::GetInstance()->ChangeState(HowtoplayState::GetInstance());
				break;

			case 2:
				Game::GetInstance()->ChangeState(OptionsState::GetInstance());
				break;

			case 3:
				Game::GetInstance()->ChangeState(CreditsState::GetInstance());
				break;

			case 4:
				Game::GetInstance()->ChangeState(LoadMenuState::GetInstance());
				break;

			case 5:
				return false;
				break;
			}
		}

#pragma endregion
		break;

	case Game::CTRL_ARCADE:
#pragma region Main Menu

		if (pInput->IsKeyPressed(SGD::Key::MouseRight)) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hMenuChangeSFX, false);
			m_nCursor = 5;
		}
		if (pInput->GetLeftJoystick(0).y > 0 && arcadebuffer <= 0) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hMenuChangeSFX, false);
			arcadebuffer = 0.2f;
			++m_nCursor;

			if (m_nCursor > 5)
				m_nCursor = 0;
		}
		else if (pInput->GetLeftJoystick(0).y < 0 && arcadebuffer <= 0) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hMenuChangeSFX, false);
			arcadebuffer = 0.2f;
			--m_nCursor;

			if (m_nCursor < 0)
				m_nCursor = 5;
		}
		if (pInput->IsKeyPressed(SGD::Key::MouseLeft)) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hMenuSelectionSFX, false);

			switch (m_nCursor){
			case 0:
				Game::GetInstance()->ChangeState(CharacterSelect::GetInstance());
				return true;
				break;

			case 1:
				Game::GetInstance()->ChangeState(HowtoplayState::GetInstance());
				break;

			case 2:
				Game::GetInstance()->ChangeState(OptionsState::GetInstance());
				break;

			case 3:
				Game::GetInstance()->ChangeState(CreditsState::GetInstance());
				break;

			case 4:
				Game::GetInstance()->ChangeState(LoadMenuState::GetInstance());
				break;

			case 5:
				return false;
				break;

			default:
				break;
			}
		}

#pragma endregion
		break;

	default:
		break;
	}
	return true;
}