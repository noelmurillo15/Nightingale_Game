#include "Controls.h"

#include "Game.h"


Controls::Controls() {
	m_hControllerControls = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/Controls/ControllerControls.png");
	m_hArcadeControls = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/Controls/ArcadeControls.png");
	m_hKeyboardControls = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/Controls/KeyboardControls.png");
}

Controls::~Controls() {
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hControllerControls); 
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hArcadeControls);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hKeyboardControls);
}

void Controls::Update(float elapsedTime) {

}

void Controls::Render(unsigned int input) {
	SGD::GraphicsManager* graphics = SGD::GraphicsManager::GetInstance();
	SGD::Size screenres = Game::GetInstance()->GetScreenRes();

	SGD::Rectangle screenRekt = {
		256, 256,
		Game::GetInstance()->GetScreenRes().width - 50,
		Game::GetInstance()->GetScreenRes().height - 50
	};

	float scale = screenres.height / 1200;

	switch (input){

	case Game::CTRL_KEYBOARD:
		graphics->DrawTexture(m_hKeyboardControls, SGD::Point(screenRekt.left, screenRekt.top), 0.0f, {}, {}, { scale, scale });
		break;

	case Game::CTRL_CONTROLLER:
		graphics->DrawTexture(m_hControllerControls, SGD::Point(screenRekt.left, screenRekt.top), 0.0f, {}, {}, { scale, scale });
		break;

	case Game::CTRL_ARCADE:
		graphics->DrawTexture(m_hArcadeControls, SGD::Point(screenRekt.left, screenRekt.top), 0.0f, {}, {}, { scale, scale });
		break;

	default:
		break;
	}
}