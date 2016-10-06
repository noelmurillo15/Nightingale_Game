#include "CharacterSelect.h"

#include "CreatureFactory.h"
#include "GameplayState.h"
#include "MainMenuState.h"



CharacterSelect* CharacterSelect::GetInstance(void) {
	static CharacterSelect s_Instance;
	return &s_Instance;
}

bool CharacterSelect::Initialize(void) {		
	AshLife = BrockLife = JennyLife = ToddLife = true;
	return true;
}

void CharacterSelect::Terminate(void) {
	GameplayState::GetInstance()->Terminate();	
}

void CharacterSelect::Enter(void) {

	if (CharacterInstantiated){
		Game::GetInstance()->ChangeState(GameplayState::GetInstance());
		return;
	}

	SGD::GraphicsManager* graphics = SGD::GraphicsManager::GetInstance();

	m_Ash = graphics->LoadTexture(L"resource/graphics/Red.png");
	m_Brock = graphics->LoadTexture(L"resource/graphics/brock.png");
	m_Jenny = graphics->LoadTexture(L"resource/graphics/Jennifer.png");
	m_Todd = graphics->LoadTexture(L"resource/graphics/GaryFuckinOak.png");

	PlayerMaker = new CreatureFactory;

	m_selection = ENT_PROTAG;
	if (m_selection == ENT_PROTAG && !AshLife)
		++m_selection;
	if (m_selection == ENT_WILDCARD && !BrockLife)
		++m_selection;
	if (m_selection == ENT_POPO && !JennyLife)
		++m_selection;
	if (m_selection == ENT_BONES && !ToddLife)
		++m_selection;


	m_hCharacterSwitch = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/NightengaleMenuOptionSwitch.wav");
	m_hOptionSelect = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/NightengaleMenuSelect.wav");

}

void CharacterSelect::Exit(void) {
	SGD::GraphicsManager* graphics = SGD::GraphicsManager::GetInstance();

	graphics->UnloadTexture(m_Ash);
	graphics->UnloadTexture(m_Brock);
	graphics->UnloadTexture(m_Jenny);
	graphics->UnloadTexture(m_Todd);	

	SGD::AudioManager::GetInstance()->UnloadAudio(m_hCharacterSwitch);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hOptionSelect);

	delete PlayerMaker;
	PlayerMaker = nullptr;
}

bool CharacterSelect::Update(float elapsedTime) {
	if (!m_bGameOver && AshLife == false && BrockLife == false && JennyLife == false && ToddLife == false)
		m_bGameOver = true; 

	if (arcadebuffer >= 0)
		arcadebuffer -= elapsedTime;
	
	return Input();
}

void CharacterSelect::Render(float elapsedTime) {
	SGD::GraphicsManager* graphics = SGD::GraphicsManager::GetInstance();
	BitmapFont* pFont = Game::GetInstance()->GetFont();

		//	Set bakground color to black
	graphics->SetClearColor(SGD::Color(0, 0, 0));

	// Align text based on window screenres.width
	SGD::Size screenres = Game::GetInstance()->GetScreenRes();

	SGD::Size scale = screenres;
	scale.width /= 1400;
	scale.height /= 1400;

	if (m_bGameOver)
		pFont->Draw("The Nightingale research team has failed in their mission. \nWithout proper analysis of the planet, the colonizers \nfrom earth were all killed and the human race went extinct. \n\nGAME OVER", "Test", { (screenres.width / 2) - 600, screenres.height / 2 },
			1.0f, { 255, 0, 0 });
	else {

			// Display the game title centered at 2x scale
		pFont->Draw("Select your Character", "Game", { (screenres.width / 2) - 256, 50 },
			1.0f, { 255, 0, 0 });

			//Display Characters
		graphics->DrawTexture(m_Ash, SGD::Point(15, (screenres.height / 2) - (screenres.height / 6)), {}, {}, {}, SGD::Size(scale.width, scale.height));
		graphics->DrawTexture(m_Brock, SGD::Point(screenres.width / 4 - 32, (screenres.height / 2) - (screenres.height / 6)), {}, {}, {}, SGD::Size(scale.width, scale.height));
		graphics->DrawTexture(m_Jenny, SGD::Point((screenres.width / 2 + (screenres.width / 10)), (screenres.height / 2) - (screenres.height / 6)), {}, {}, {}, SGD::Size(scale.width, scale.height));
		graphics->DrawTexture(m_Todd, SGD::Point((screenres.width - (screenres.width / 6) - 32), (screenres.height / 2) - (screenres.height / 6)), {}, {}, {}, SGD::Size(scale.width, scale.height));

			// Display the menu options centered at 1x scale
		if (AshLife)
			pFont->Draw("Ash", "Game", { 48, screenres.height - 128 },
			1.0f, { 255, 0, 0 });
		else
			pFont->Draw("Deceased", "Game", { 48, screenres.height - 128 },
			1.0f, { 255, 0, 0 });

		if (BrockLife)
			pFont->Draw("Brock", "Game", { (screenres.width / 4), screenres.height - 128 },
			1.0f, { 255, 0, 0 });
		else
			pFont->Draw("Deceased", "Game", { (screenres.width / 4), screenres.height - 128 },
			1.0f, { 255, 0, 0 });

		if (JennyLife)
			pFont->Draw("Jennifer", "Game", { (screenres.width / 2 + (screenres.width / 8)), screenres.height - 128 },
			1.0f, { 255, 0, 0 });
		else
			pFont->Draw("Deceased", "Game", { (screenres.width / 2 + (screenres.width / 8)), screenres.height - 128 },
			1.0f, { 255, 0, 0 });

		if (ToddLife)
			pFont->Draw("Todd", "Game", { (screenres.width - (screenres.width / 8)), screenres.height - 128 },
			1.0f, { 255, 0, 0 });
		else
			pFont->Draw("Deceased", "Game", { (screenres.width - (screenres.width / 8)), screenres.height - 128 },
			1.0f, { 255, 0, 0 });


		pFont->Draw("Back", "Game", { (screenres.width / 2 - 64), screenres.height - 64 },
			1.0f, { 255, 0, 0 });		

		switch (m_selection) {
		case ENT_PROTAG:
			pFont->Draw("Ashe", "Game", { 48, screenres.height - 126 },
				1.0f, { 155, 155, 0 });
			break;
		case ENT_WILDCARD:
			pFont->Draw("Brock", "Game", { (screenres.width / 4), screenres.height - 126 },
				1.0f, { 155, 155, 0 });
			break;
		case ENT_POPO:
			pFont->Draw("Jennyxox", "Game", { (screenres.width / 2 + (screenres.width / 8)), screenres.height - 126 },
				1.0f, { 155, 155, 0 });
			break;
		case ENT_BONES:
			pFont->Draw("Todd", "Game", { (screenres.width - (screenres.width / 8)), screenres.height - 126 },
				1.0f, { 155, 155, 0 });
			break;
		case ENT_QUIT:
			pFont->Draw("Back", "Game", { (screenres.width / 2) - 64, screenres.height - 62 },
				1.0f, { 155, 155, 0 });
			break;
		}
	}
}

bool CharacterSelect::Input() {
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	switch (Game::GetInstance()->GetCurrentInput()){

	case Game::CTRL_KEYBOARD:
#pragma region Char_Select
		if (m_bGameOver) {
			if (pInput->IsAnyKeyPressed()) {
				m_bGameOver = false;
				AshLife = true;
				BrockLife = true;
				JennyLife = true;
				ToddLife = true;
				Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
			}
		}
		if (pInput->IsKeyPressed(SGD::Key::Escape)) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hOptionSelect, false);
			m_selection = 4;			
		}
		if (pInput->IsKeyPressed(SGD::Key::A) || pInput->IsKeyPressed(SGD::Key::LeftArrow)) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hCharacterSwitch, false);
			--m_selection;
			while (true){
				bool h = true;
				if (m_selection < ENT_PROTAG) {
					m_selection = ENT_QUIT;
					h = false;
				}
				if (m_selection <= ENT_PROTAG && AshLife == false) {
					m_selection = ENT_QUIT;
					h = false;
				}
				if (m_selection == ENT_PROTAG && AshLife == false) {
					--m_selection;
					h = false;
				}
				if (m_selection == ENT_WILDCARD && BrockLife == false) {
					--m_selection;
					h = false;
				}
				if (m_selection == ENT_POPO && JennyLife == false) {
					--m_selection;
					h = false;
				}
				if (m_selection == ENT_BONES && ToddLife == false) {
					--m_selection;
					h = false;
				}
				if (h == true)
					break;
			}
		}
		else if (pInput->IsKeyPressed(SGD::Key::RightArrow) || pInput->IsKeyPressed(SGD::Key::D)) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hCharacterSwitch, false);
			++m_selection;
			while (true) {
				if (m_selection > ENT_QUIT)
					m_selection = ENT_PROTAG;
				if (m_selection <= ENT_PROTAG && AshLife == false)
					++m_selection;
				if (m_selection == ENT_WILDCARD && BrockLife == false)
					++m_selection;
				if (m_selection == ENT_POPO && JennyLife == false)
					++m_selection;
				if (m_selection == ENT_BONES && ToddLife == false)
					++m_selection;
				else
					break;
			}
		}
		if (pInput->IsKeyPressed(SGD::Key::Up) || pInput->IsKeyPressed(SGD::Key::W)) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hCharacterSwitch, false);

			if (m_selection >= 4) {
				m_selection = _previous;
				_previous = ENT_QUIT;
			}
			else {
				_previous = m_selection;
				m_selection = ENT_QUIT;
			}
		}
		else if (pInput->IsKeyPressed(SGD::Key::Down) || pInput->IsKeyPressed(SGD::Key::S)) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hCharacterSwitch, false);
			if (m_selection >= 4) {
				m_selection = _previous;
				_previous = ENT_QUIT;
			}
			else {
				_previous = m_selection;
				m_selection = ENT_QUIT;
			}
		}	
		if (pInput->IsKeyPressed(SGD::Key::Enter) && m_selection != 4) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hOptionSelect, false);

			if (!GameplayState::GetInstance()->GetAlreadyInstantiated())
				GameplayState::GetInstance()->Initialize();
			
			GameplayState::GetInstance()->AddToManager(PlayerMaker->CreatePlayer(m_selection));

			CharacterInstantiated = true;

			Game::GetInstance()->ChangeState(GameplayState::GetInstance());			
		}
		else if (pInput->IsKeyPressed(SGD::Key::Enter) && m_selection == 4) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hOptionSelect, false);
			Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
		}
#pragma endregion
		break;

	case Game::CTRL_CONTROLLER:
#pragma region Char_Select
		if (m_bGameOver) {
			if (pInput->IsButtonPressed(0, 1)) {
				m_bGameOver = false;
				AshLife = true;
				BrockLife = true;
				JennyLife = true;
				ToddLife = true;
				Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
			}
		}
		if (pInput->IsDPadPressed(0, SGD::DPad::Left)) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hCharacterSwitch, false);

			--m_selection;

			while (true) {
				bool h = true;
				if (m_selection < ENT_PROTAG) {
					m_selection = ENT_QUIT;
					h = false;
				}
				if (m_selection <= ENT_PROTAG && AshLife == false) {
					m_selection = ENT_QUIT;
					h = false;
				}
				if (m_selection == ENT_PROTAG && AshLife == false) {
					--m_selection;
					h = false;
				}
				if (m_selection == ENT_WILDCARD && BrockLife == false) {
					--m_selection;
					h = false;
				}
				if (m_selection == ENT_POPO && JennyLife == false) {
					--m_selection;
					h = false;
				}
				if (m_selection == ENT_BONES && ToddLife == false) {
					--m_selection;
					h = false;
				}
				if (h == true)
					break;
			}
		}
		else if (pInput->IsDPadPressed(0, SGD::DPad::Right)) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hCharacterSwitch, false);
			++m_selection;
			while (true) {
				if (m_selection > ENT_QUIT)
					m_selection = ENT_PROTAG;
				if (m_selection <= ENT_PROTAG && AshLife == false)
					++m_selection;
				if (m_selection == ENT_WILDCARD && BrockLife == false)
					++m_selection;
				if (m_selection == ENT_POPO && JennyLife == false)
					++m_selection;
				if (m_selection == ENT_BONES && ToddLife == false)
					++m_selection;
				else
					break;
			}
		}
		if (pInput->IsDPadPressed(0, SGD::DPad::Up)) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hCharacterSwitch, false);
			if (m_selection >= 4) {
				m_selection = _previous;
				_previous = ENT_QUIT;
			}
			else {
				_previous = m_selection;
				m_selection = ENT_QUIT;
			}
		}
		else if (pInput->IsDPadPressed(0, SGD::DPad::Down)) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hCharacterSwitch, false);
			if (m_selection >= 4) {
				m_selection = _previous;
				_previous = ENT_QUIT;
			}
			else {
				_previous = m_selection;
				m_selection = ENT_QUIT;
			}
		}	
		if (pInput->IsButtonPressed(0, 0) && m_selection != 4) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hOptionSelect, false);

			if (!GameplayState::GetInstance()->GetAlreadyInstantiated())
				GameplayState::GetInstance()->Initialize();

			GameplayState::GetInstance()->AddToManager(PlayerMaker->CreatePlayer(m_selection));

			CharacterInstantiated = true;

			Game::GetInstance()->ChangeState(GameplayState::GetInstance());
		}
		else if (pInput->IsButtonPressed(0, 0) && m_selection == 4) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hOptionSelect, false);
			Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
		}		
#pragma endregion
		break;

	case Game::CTRL_ARCADE:
#pragma region Char_Select
		if (m_bGameOver) {
			if (pInput->IsKeyPressed(SGD::Key::MouseLeft)) {
				m_bGameOver = false;
				AshLife = true;
				BrockLife = true;
				JennyLife = true;
				ToddLife = true;
				Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
			}
		}
		if (pInput->IsKeyPressed(SGD::Key::MouseRight)) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hOptionSelect, false);
			m_selection = 4;			
		}
		if (pInput->GetLeftJoystick(0).x < 0 && arcadebuffer <= 0) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hCharacterSwitch, false);
			arcadebuffer = 0.2f;
			--m_selection;

			while (true) {
				bool h = true;
				if (m_selection < ENT_PROTAG) {
					m_selection = ENT_QUIT;
					h = false;
				}
				if (m_selection <= ENT_PROTAG && AshLife == false) {
					m_selection = ENT_QUIT;
					h = false;
				}
				if (m_selection == ENT_PROTAG && AshLife == false) {
					--m_selection;
					h = false;
				}
				if (m_selection == ENT_WILDCARD && BrockLife == false) {
					--m_selection;
					h = false;
				}
				if (m_selection == ENT_POPO && JennyLife == false) {
					--m_selection;
					h = false;
				}
				if (m_selection == ENT_BONES && ToddLife == false) {
					--m_selection;
					h = false;
				}
				if (h == true)
					break;
			}
		}
		if (pInput->GetLeftJoystick(0).x > 0 && arcadebuffer <= 0) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hCharacterSwitch, false);
			arcadebuffer = 0.2f;
			++m_selection;
			while (true) {
				if (m_selection > ENT_QUIT)
					m_selection = ENT_PROTAG;
				if (m_selection <= ENT_PROTAG && AshLife == false)
					++m_selection;
				if (m_selection == ENT_WILDCARD && BrockLife == false)
					++m_selection;
				if (m_selection == ENT_POPO && JennyLife == false)
					++m_selection;
				if (m_selection == ENT_BONES && ToddLife == false)
					++m_selection;
				else
					break;
			}
		}
		if (pInput->GetLeftJoystick(0).y > 0 && arcadebuffer <= 0) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hCharacterSwitch, false);
			arcadebuffer = 0.2f;
			if (m_selection >= 4) {
				m_selection = _previous;
				_previous = ENT_QUIT;
			}
			else {
				_previous = m_selection;
				m_selection = ENT_QUIT;
			}
		}
		if (pInput->GetLeftJoystick(0).y < 0 && arcadebuffer <= 0) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hCharacterSwitch, false);
			arcadebuffer = 0.2f;
			if (m_selection >= 4) {
				m_selection = _previous;
				_previous = ENT_QUIT;
			}
			else {
				_previous = m_selection;
				m_selection = ENT_QUIT;
			}
		}
		if (pInput->IsKeyPressed(SGD::Key::MouseLeft) && m_selection != 4) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hOptionSelect, false);

			if (!GameplayState::GetInstance()->GetAlreadyInstantiated())
				GameplayState::GetInstance()->Initialize();

			GameplayState::GetInstance()->AddToManager(PlayerMaker->CreatePlayer(m_selection));

			CharacterInstantiated = true;

			Game::GetInstance()->ChangeState(GameplayState::GetInstance());
		}
		else if (pInput->IsKeyPressed(SGD::Key::MouseLeft) && m_selection == 4) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hOptionSelect, false);
			Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
		}
#pragma endregion
		break;
	}
	return true;
}