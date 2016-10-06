#include "CreditsState.h"

#include "MainMenuState.h"


CreditsState* CreditsState::GetInstance(void) {
	static CreditsState s_Instance;
	return &s_Instance;
}

bool CreditsState::Initialize(void) {
	return true;
}

void CreditsState::Terminate(void) {

}

void CreditsState::Enter(void) {
	SGD::GraphicsManager::GetInstance()->SetClearColor({ 0, 0, 0 });

	m_hCreditsBGM = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/NightengaleCreditsMusic.xwm"); 
	m_hLeaveCreditsSFX = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/NightengaleMenuSelect.wav"); 

	SGD::AudioManager::GetInstance()->PlayAudio(m_hCreditsBGM, true); 
}

void CreditsState::Exit(void) {
	SGD::AudioManager::GetInstance()->StopAudio(m_hCreditsBGM);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hCreditsBGM); 
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hLeaveCreditsSFX); 
}

bool CreditsState::Update(float elapsedTime) {
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	switch (Game::GetInstance()->GetCurrentInput()){

	case Game::CTRL_KEYBOARD:
		if (pInput->IsKeyPressed(SGD::Key::Escape)) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hLeaveCreditsSFX, false);
			Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
			return true;
		}
		break;

	case Game::CTRL_CONTROLLER:
		if (pInput->IsButtonPressed(0, 1)) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hLeaveCreditsSFX, false);
			Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
			return true;	// keep playing in the new state
		}
		break;

	case Game::CTRL_ARCADE:
		if (pInput->IsKeyPressed(SGD::Key::MouseRight)) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hLeaveCreditsSFX, false);
			Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
			return true;	// keep playing in the new state
		}
		break;
	}
	return true;
}

void CreditsState::Render(float elapsedTime) {
	BitmapFont* pFont = Game::GetInstance()->GetFont();
	float scale = Game::GetInstance()->GetScreenRes().height;
	scale /= 700;
	float half = Game::GetInstance()->GetScreenRes().width / 2;

	pFont->Draw("Credits  ", "Game", { half - 10, 50 },
		scale, SGD::Color(155, 0, 255));

	if (Game::GetInstance()->GetCurrentRes() != Game::RES_ARCADE){
		pFont->Draw("Executive Producer: ", "Game", { 300, 175 }, scale, SGD::Color(155, 0, 255));
		pFont->Draw("John O Leske\n", "Game", { 350, 225 }, scale, SGD::Color(155, 0, 255));

		pFont->Draw("Associate Producer:", "Game", { 300, 275 }, scale, SGD::Color(155, 0, 255));
		pFont->Draw("Sean Hathaway\n", "Game", { 350, 325 }, scale, SGD::Color(155, 0, 255));

		pFont->Draw("Art:", "Game", { 300, 375 }, scale, SGD::Color(155, 0, 255));
		pFont->Draw("Greg Bey\n", "Game", { 350, 425 }, scale, SGD::Color(155, 0, 255));

		pFont->Draw("Programmers:", "Game", { 300, 475 }, scale, SGD::Color(155, 0, 255));
		pFont->Draw("Allan Murillo \nTheo Kalloo \nMike Naulls \nJacob Evans", "Game", { 350, 525 }, scale, SGD::Color(155, 0, 255));

		pFont->Draw("Special Thanks:", "Game", { 900, 300 }, scale, SGD::Color(155, 0, 255));
		pFont->Draw("String Cheese", "Game", { 950, 350 }, scale, SGD::Color(155, 0, 255));
	} else {
		pFont->Draw("Executive Producer: ", "Game", { 100, 175 }, scale, SGD::Color(155, 0, 255));
		pFont->Draw("John O Leske\n", "Game", { 120, 225 }, scale, SGD::Color(155, 0, 255));

		pFont->Draw("Associate Producer:", "Game", { 100, 275 }, scale, SGD::Color(155, 0, 255));
		pFont->Draw("Sean Hathaway\n", "Game", { 120, 325 }, scale, SGD::Color(155, 0, 255));

		pFont->Draw("Art:", "Game", { 100, 375 }, scale, SGD::Color(155, 0, 255));
		pFont->Draw("Greg Bey\n", "Game", { 120, 425 }, scale, SGD::Color(155, 0, 255));

		pFont->Draw("Programmers:", "Game", { 500, 200 }, scale, SGD::Color(155, 0, 255));
		pFont->Draw("Allan Murillo \nTheo Kalloo \nMike Naulls \nJacob Evans", "Game", { 520, 250 }, scale, SGD::Color(155, 0, 255));

		pFont->Draw("Special Thanks:", "Game", { 500, 450 }, scale, SGD::Color(155, 0, 255));
		pFont->Draw("String Cheese", "Game", { 520, 500 }, scale, SGD::Color(155, 0, 255));
	}
}