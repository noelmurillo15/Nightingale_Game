#include "DialogSystem.h"

#include "Game.h"
#include <string>


DialogSystem::DialogSystem() {
	dialogBox = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/DialogBox.png");
	questLogBox = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/QuestLogBox.png");
	m_DisplayDialogTimer = 0.0f;	
	m_renderQuest = true;
}

DialogSystem::~DialogSystem() {
	m_DialogQueue.clear();
	SGD::GraphicsManager::GetInstance()->UnloadTexture(dialogBox);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(questLogBox);
}

void DialogSystem::Update(float elapsedTime){
	SGD::InputManager* input = SGD::InputManager::GetInstance();

	if ( input->IsKeyPressed(SGD::Key::Enter) || 
		(input->IsControllerConnected(0) && input->IsButtonPressed(0,0)) ||
		(input->IsControllerConnected(0) && input->IsButtonPressed(0, 1)))
		m_DisplayDialogTimer = -1.0f;

	if (input->IsKeyPressed(SGD::Key::T) || input->IsButtonPressed(0, 6) || Game::GetInstance()->GetCurrentInput() == Game::CTRL_ARCADE && input->IsButtonPressed(0, 4))
		ToggleRenderQuest();

	if (m_DisplayDialogTimer > 0)
		m_DisplayDialogTimer -= elapsedTime;	
	else{
			//	If dialog is not empty.. empty it
		if (!m_currDialog.m_Text.IsEmpty())
			m_currDialog.m_Text.Empty();

			//	If queue is not empty.. get next dialog
		if (!m_DialogQueue.empty() && m_currDialog.m_Text.IsEmpty()){
			m_currDialog = m_DialogQueue.front();
			m_DisplayDialogTimer = 10.0f;

			auto iter = m_DialogQueue.begin();
			m_DialogQueue.erase(iter);
		}

		if (m_DialogQueue.empty() && m_currDialog.m_Text.IsEmpty())
			m_empty = true;
	}
}

void DialogSystem::Render(SGD::Rectangle m_CamBounds, SGD::Point offset){	

	if (!m_currDialog.m_Text.IsEmpty()){
		BitmapFont* pFont = Game::GetInstance()->GetFont();
		SGD::Color color = { 255, 255, 255 };

		SGD::Size screenscale = Game::GetInstance()->GetScreenRes();
		screenscale.width /= 1500;
		screenscale.height /= 1200;

		float scale = screenscale.height;

		if (Game::GetInstance()->GetCurrentRes() == Game::RES_ARCADE)
			scale = 0.48f;

		if (m_currDialog.m_Sender == "Jenny")
			color = { 0, 0, 255 };
		else if (m_currDialog.m_Sender == "Brock")
			color = { 255, 0, 0 };
		else if (m_currDialog.m_Sender == "Ash")
			color = { 0, 255, 0 };
		else if (m_currDialog.m_Sender == "Todd")
			color = { 0, 0, 0 };


		SGD::GraphicsManager::GetInstance()->DrawTexture(dialogBox, { m_CamBounds.left - offset.x + 40, m_CamBounds.bottom - offset.y - (460 * screenscale.height) }, 0.0f, {}, {}, { screenscale.width, screenscale.height });

		pFont->Draw(m_currDialog.m_Sender, "Test", { m_CamBounds.left - offset.x + 50, m_CamBounds.bottom - offset.y - (420 * screenscale.height) },
			scale, color);

		pFont->Draw(m_currDialog.m_Text, "Test", { m_CamBounds.left - offset.x + 75, m_CamBounds.bottom - offset.y - (380 * screenscale.height) },
			scale, color);
	}
}

void DialogSystem::RenderQuest(SGD::Rectangle m_CamBounds, SGD::Point offset, unsigned int scanned){
	if (m_renderQuest){
		SGD::GraphicsManager* g = SGD::GraphicsManager::GetInstance();
		BitmapFont* pFont = Game::GetInstance()->GetFont();

		SGD::Color color = { 0, 255, 255 };

		SGD::Size screenscale = Game::GetInstance()->GetScreenRes();
		screenscale.width /= 2000;
		screenscale.height /= 1200;

		float scale = screenscale.width;

		g->DrawTexture(questLogBox, { (m_CamBounds.right - offset.x) - ((m_CamBounds.right - offset.x) / 4.0f), m_CamBounds.top - offset.y + 40 }, 0.0f, {}, {}, { screenscale.width, screenscale.height });

		std::string name = "Quest : ";
		pFont->Draw(name, "Test", { (m_CamBounds.right - offset.x) - ((m_CamBounds.right - offset.x) / 4.2f), m_CamBounds.top - offset.y + 60 },
			scale, color);

		std::string quest;
		if (scanned < 2)
			quest = "Analyzed " + std::to_string(scanned) + " of 2";
		else
			quest = "All creatures scanned \n proceed to next level \n via your anadex menu";

		pFont->Draw(quest, "Game", { (m_CamBounds.right - offset.x) - ((m_CamBounds.right - offset.x) / 4.2f), m_CamBounds.top - offset.y + 120 },
			scale, color);
	}
}

void DialogSystem::AddDialogToQueue(CString text, std::string sender){
	if (m_currDialog.m_Text.IsEmpty()){
		m_currDialog.m_Text = text;
		m_currDialog.m_Sender = sender;
		m_DisplayDialogTimer = 10.0f;
	}	
	else{
		DialogMsg temp;
		temp.m_Text = text;
		temp.m_Sender = sender;
		m_DialogQueue.push_back(temp);		
	}	
	m_empty = false;
}

void DialogSystem::AddDialogToQueue(int stringtablenum, std::string sender){
	if (m_currDialog.m_Text.IsEmpty()){
		m_currDialog.m_Text.LoadStringW(stringtablenum);
		m_currDialog.m_Sender = sender;
		m_DisplayDialogTimer = 10.0f;
	}
	else{
		DialogMsg temp;
		temp.m_Text.LoadStringW(stringtablenum);
		temp.m_Sender = sender;
		m_DialogQueue.push_back(temp);
	}
	m_empty = false;
}

void DialogSystem::AddDialogToQueue(std::string text, std::string sender){
	if (m_currDialog.m_Text.IsEmpty()){
		m_currDialog.m_Text.Format(_T("%S"), text.c_str());
		m_currDialog.m_Sender = sender;
		m_DisplayDialogTimer = 10.0f;	
	}
	else{
		DialogMsg temp;
		temp.m_Text.Format(_T("%S"), text.c_str());
		temp.m_Sender = sender;
		m_DialogQueue.push_back(temp);
	}
	m_empty = false;
}