#include "ControllerState.h"

#include "MainMenuState.h"
#include "Controls.h"


ControllerState* ControllerState::GetInstance(void) {
	static ControllerState s_Instance;
	return &s_Instance;
}

bool ControllerState::Initialize(void) {
	return true;
}

void ControllerState::Terminate(void) {

}

void ControllerState::Enter(void) {
	SGD::GraphicsManager::GetInstance()->SetClearColor({ 100, 100, 100 });

	m_Controls = new Controls;
}

void ControllerState::Exit(void) {
	delete m_Controls;
}

bool ControllerState::Update(float elapsedTime) {
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	switch (Game::GetInstance()->GetCurrentInput()){

	case Game::CTRL_KEYBOARD:
		if (pInput->IsKeyPressed(SGD::Key::Escape)) {
			Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
			return true;
		}
		break;

	case Game::CTRL_CONTROLLER:
		if (pInput->IsButtonPressed(0, 1)) {
			Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
			return true;	
		}
		break;

	case Game::CTRL_ARCADE:
		if (pInput->IsKeyPressed(SGD::Key::MouseRight)) {
			Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
			return true;	
		}
		break;
	}
	return true;
}

void ControllerState::Render(float elapsedTime) {
	m_Controls->Render(Game::GetInstance()->GetCurrentInput());
}