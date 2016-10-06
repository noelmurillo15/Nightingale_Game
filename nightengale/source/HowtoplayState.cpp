#include "HowtoplayState.h"

#include "MainMenuState.h"
#include "TileEngine.h"
#include "Player.h"
#include "AnimationSystem.h"
#include "Kao.h"
#include "Pawn.h"
#include "DialogSystem.h"
#include "StaticObject.h"
#include "Analyzer.h"
#include "ControllerState.h"


HowtoplayState* HowtoplayState::GetInstance(void) {
	static HowtoplayState s_Instance;
	return &s_Instance;
}

bool HowtoplayState::Initialize(void) {
	return true;
}

void HowtoplayState::Terminate(void) {

}

void HowtoplayState::Enter(void) {
	SGD::GraphicsManager::GetInstance()->SetClearColor({ 140, 140, 140 });
	AnimationSystem::GetInstance();

	TileEngine::GetInstance()->Enter();
	TileEngine::GetInstance()->LoadMap(0);

	m_Player = new Player("Brock");
	TheMan = new Kao();
	TheMan->SetInitialPosition({40*32, 5 * 32});
	m_Meerkat = new Pawn();
	m_Meerkat->SetInitialPosition({50*32, 5 * 32});
	m_Anadex = new Analyzer();

	m_Dialog = new DialogSystem();

	for (size_t i = 0; i < Camera::GetInstance()->GetMapLayer()->GetBushPoints().size(); i++) {
		StaticObject* Bush = new StaticObject(Camera::GetInstance()->GetMapLayer()->GetBushPoints().at(i));
		m_Bushes.push_back(Bush);
	}


	Camera::GetInstance()->SetCamFocus(m_Player);
}

void HowtoplayState::Exit(void) {

		//	Delete Entities
	delete m_Meerkat;
	m_Meerkat = nullptr;
	delete TheMan;
	TheMan = nullptr;
		//	Camera deletes player -- focus
	m_Player = nullptr;

	delete m_Dialog;
	m_Dialog = nullptr;

	delete m_Anadex;
	m_Anadex = nullptr;

	for (unsigned int x = 0; x < m_Bushes.size(); ++x)
		delete m_Bushes[x];

	m_Bushes.clear();

		//	Destroy TileEngine & Animation System
	TileEngine::GetInstance()->Exit();		
	AnimationSystem::GetInstance()->DeleteInstance();
}

bool HowtoplayState::Update(float elapsedTime) {
	SGD::InputManager* input = SGD::InputManager::GetInstance();
	Camera* cam = Camera::GetInstance();

	if (input->IsKeyPressed(SGD::Key::Escape) == true) {
		Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
		return true;
	}			
	
	m_Dialog->Update(elapsedTime);
	m_Anadex->Update(elapsedTime, m_Meerkat, m_Player);


	TileEngine::GetInstance()->Update(elapsedTime);

	std::vector<SGD::Point> bushpts = Camera::GetInstance()->GetMapLayer()->GetBushPoints();
	for (unsigned int x = 0; x < bushpts.size(); ++x){
		if (m_Player->GetRect().IsPointInRectangle(bushpts[x])){
			m_Player->SetInHiding(true);
			break;
		}
		m_Player->SetInHiding(false);
	}

	switch (GetProgress()){

	case T_START:				
		if (TheMan->GetPosition().x > 8 * 32){
			SGD::Point nextmove = { 0.0f, 0.0f };
			nextmove.x = -200 * elapsedTime;
			TheMan->SetPosition(SGD::Point(TheMan->GetPosition().x + nextmove.x, TheMan->GetPosition().y + nextmove.y));
			if (!TheMan->MsgSent){
				std::string msg = "HEY!";
				m_Dialog->AddDialogToQueue(msg, "Kao");
				msg = "You're late to training yet again " + m_Player->GetName() + "!" ;
				m_Dialog->AddDialogToQueue(msg, "Kao");
				if (Game::GetInstance()->GetCurrentInput() == Game::CTRL_KEYBOARD)
					msg = "Quickly, use the WASD keys and follow \nme this way.";
				else
					msg = "Quickly, use your joystick and follow \nme this way";
				m_Dialog->AddDialogToQueue(msg, "Kao");
				TheMan->MsgSent = true;
			}
		}
		else if (m_Dialog->GetIsEmpty()){			
			TheMan->MsgSent = false;
			cam->SetCamFocus(m_Player);
			NextProgress();
		}
		break;

	case T_WASD:
		if (m_Player->GetPosition().x < 640) {
			if (TheMan->GetPosition().x < 22 * 32){
				SGD::Point nextmove = { 0.0f, 0.0f };
				nextmove.x = 200 * elapsedTime;
				TheMan->SetPosition(SGD::Point(TheMan->GetPosition().x + nextmove.x, TheMan->GetPosition().y + nextmove.y));
			}
			m_Player->Update(elapsedTime);
		}
		else{
			if (!TheMan->MsgSent){
				std::string msg = "Try not to goof off unless you want to \n end up dead! \nYou really need this training!";
				m_Dialog->AddDialogToQueue(msg, "Kao");
				msg = "The creatures in Narnia are some of the \n deadliest we've ever seen.";
				m_Dialog->AddDialogToQueue(msg, "Kao");
				msg = "To survive, you must learn these basic skills";
				m_Dialog->AddDialogToQueue(msg, "Kao");
				msg = "We'll start out by hiding. \n Move towards a bush and stay put.";
				m_Dialog->AddDialogToQueue(msg, "Kao");
				TheMan->MsgSent = true;
			}
			NextProgress();
			TheMan->MsgSent = false;
		}
		break;

	case T_HIDING:

		if (!m_Player->GetInHiding()){
			m_Player->Update(elapsedTime);
		}
		else{
			if (!TheMan->MsgSent){
				std::string msg = "Good, these bushes provide adequeate cover \n from the monsters in Narnia but if those monster \n get too close they will notice \n you immediatly!";
				m_Dialog->AddDialogToQueue(msg, "Kao");
				TheMan->MsgSent = true;
			}
			TheMan->MsgSent = false;
			NextProgress();
		}
		break;

	case T_PAWN:
	{
		   int xdifference = (int)abs(TheMan->GetPosition().ComputeDifference(m_Meerkat->GetPosition()).x);
		   int ydifference = (int)abs(TheMan->GetPosition().ComputeDifference(m_Meerkat->GetPosition()).y);
		
		   SGD::Vector  TempVec = { 0, 0 };
		   SGD::Point NextPos = TheMan->GetPosition();
		
		   if (xdifference >= 128 || ydifference >= 128){
			   TempVec = m_Meerkat->GetPosition() - TheMan->GetPosition();
			   TempVec = TempVec.ComputeNormalized();
			   TempVec = TempVec * (float)(200) * elapsedTime;
		
			   NextPos.x += TempVec.x;
			   NextPos.y += TempVec.y;
			   TheMan->SetPosition(NextPos);
		
			   if (!TheMan->MsgSent ){
				   std::string msg = "Quickly follow me! I see a pawn just over yonder";
				   m_Dialog->AddDialogToQueue(msg, "Kao");
				  
				   if (Game::GetInstance()->GetCurrentInput() == Game::CTRL_KEYBOARD)
					   msg = "If you face the pawn and use the left \nmouse button youre analyzer will scan it";
				   else if (Game::GetInstance()->GetCurrentInput() == Game::CTRL_CONTROLLER)
					   msg = "If you face the pawn and use the \n left button on your controller \n youre analyzer will scan it";
				   else
					   msg = "If you face the pawn and use the \n fourth button on the arcade machine \n youre analyzer will scan it";
				   m_Dialog->AddDialogToQueue(msg, "Kao");
				   TheMan->MsgSent = true;
			   }
		   }
		   else{
			   TheMan->MsgSent = false;
		
			   xdifference = (int)abs(TheMan->GetPosition().ComputeDifference(m_Player->GetPosition()).x);
			   ydifference = (int)abs(TheMan->GetPosition().ComputeDifference(m_Player->GetPosition()).y);		
		
			   if (xdifference <= 128 || ydifference <= 128)
				   NextProgress();
		   }
		   m_Player->Update(elapsedTime);
	}
		break;

	case T_ANALYZING:
		

		if (m_Anadex->GetSuccess()){			
			TheMan->MsgSent = false;
			m_bTutorialScanFinished = true; 
			NextProgress();
		}

		m_Player->Update(elapsedTime);
		break;

	case T_BAIT:
		if (!TheMan->MsgSent){
			std::string msg;
			if (Game::GetInstance()->GetCurrentInput() == Game::CTRL_KEYBOARD)
				msg = "Great! Now trying using a bait \n Press the Q button on your keyboard";
			else if (Game::GetInstance()->GetCurrentInput() == Game::CTRL_CONTROLLER)
				msg = "Great! Now trying using a bait \n Press the X button on your controller";
			else
				msg = "Great! Now trying using a bait \n Press the second button on your \n arcade machine";
			
			m_Dialog->AddDialogToQueue(msg, "Kao");
			TheMan->MsgSent = true;
		}

		if (input->IsKeyPressed(SGD::Key::Q) || input->IsButtonPressed(0, 2) || input->IsButtonPressed(0,1)){			
			TheMan->MsgSent = false;
			if (!TheMan->MsgSent){
				std::string msg = "Using baits will sometimes distract \n a deadly creature away from you";
				m_Dialog->AddDialogToQueue(msg, "Kao");
				TheMan->MsgSent = true;
			}
			TheMan->MsgSent = false;
			NextProgress();
		}

		m_Player->Update(elapsedTime);
		break;

	case T_REPEL:
		if (!TheMan->MsgSent){
			std::string msg;
			if (Game::GetInstance()->GetCurrentInput() == Game::CTRL_KEYBOARD)
				msg = "Now trying using a repel \n Press the E button on your keyboard";
			else if (Game::GetInstance()->GetCurrentInput() == Game::CTRL_CONTROLLER)
				msg = "Now trying using a repel \n Press the Y button on your controller";
			else
				msg = "Now trying using a repel \n Press the third button on your \n arcade machine";
			m_Dialog->AddDialogToQueue(msg, "Kao");
			TheMan->MsgSent = true;
		}

		if (input->IsKeyPressed(SGD::Key::E) || input->IsButtonPressed(0, 3) || input->IsButtonPressed(0, 2)){
			TheMan->MsgSent = false;
			if (!TheMan->MsgSent){
				std::string msg = "Sick! Using repel will scare away any \n creature unless of course they just \n want to eat you";
				m_Dialog->AddDialogToQueue(msg, "Kao");
			}
			TheMan->MsgSent = false;
			NextProgress();
		}

		m_Player->Update(elapsedTime);
		break;

	case T_SAVING:
		NextProgress();
		m_Player->Update(elapsedTime);
		break;

	case T_LOG:
		if (!TheMan->MsgSent){
			std::string msg = "That is the end of this tutorial \n See you in Narnia!";
			m_Dialog->AddDialogToQueue(msg, "Kao");
		}
		TheMan->MsgSent = false;
		NextProgress();
		break;

	case T_END:

		if (m_Dialog->GetIsEmpty()){
			Game::GetInstance()->ChangeState(ControllerState::GetInstance());
			return true;
		}
		break;

	default:
		break;
	}

	TheMan->Update(elapsedTime);
	m_Meerkat->UpdateBaseRect(cam->GetCameraOffset());

	for (unsigned int x = 0; x < m_Bushes.size(); ++x)
		m_Bushes[x]->Update(elapsedTime);

	for (unsigned int x = 0; x < m_Bushes.size(); ++x){
		m_Bushes[x]->HandleCollision(m_Player);
	}
	
	return true;
}

void HowtoplayState::Render(float elapsedTime) {
	SGD::GraphicsManager* g = SGD::GraphicsManager::GetInstance();
	SGD::Point bawlz = Camera::GetInstance()->GetCameraOffset();
	
	TileEngine::GetInstance()->Render();	

	for (unsigned int x = 0; x < m_Bushes.size(); ++x)
		m_Bushes[x]->Render(bawlz);

	m_Dialog->Render(Camera::GetInstance()->GetCameraBounds(), bawlz);

	if (false == m_bTutorialScanFinished)
		m_Anadex->Render();


	m_Meerkat->Render(bawlz);
	TheMan->Render(bawlz);
	m_Player->Render(bawlz);	
}