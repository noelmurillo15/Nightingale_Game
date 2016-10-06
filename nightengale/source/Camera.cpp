#include "Camera.h"

#include "Game.h"

#include "../SGD Wrappers/SGD_InputManager.h"


Camera* Camera::GetInstance(void) {
	static Camera s_Instance;	// stored in global memory once
	return &s_Instance;
}

void Camera::Enter(void) {
	m_ScreenSize = Game::GetInstance()->GetScreenRes();
	m_CameraBounds = { 0, 0, m_ScreenSize.width, m_ScreenSize.height };
	m_CamPos = { m_CameraBounds.right - m_ScreenSize.width / 2, m_CameraBounds.bottom - m_ScreenSize.height / 2 };

	m_mapLayer = new Layer();
}

void Camera::Exit(void) {
	delete Focus;
	Focus = nullptr;
	delete m_mapLayer;
	m_mapLayer = nullptr;
}

void Camera::InitializeLayer(bool bush, bool collider, SGD::Point point){
	if (collider)
		m_mapLayer->AddCollisionPoints(point);

	if (bush)
		m_mapLayer->AddBushPoints(point);
}

void Camera::Update(float elapsedtime){

	SGD::Vector nextmove = Focus->GetPrevPosition().ComputeDifference(Focus->GetPosition());

	Focus->SetPrevPosition(Focus->GetPosition());

	if (Focus->GetPosition().x > m_ScreenSize.width / 2 && Focus->GetPosition().x < m_WorldSize.width - m_ScreenSize.width / 2){
		this->SetCameraOffset(SGD::Point(this->GetCameraOffset().x + nextmove.x, this->GetCameraOffset().y));
		this->SetCameraPos(SGD::Point(this->GetCameraPosition().x + nextmove.x, this->GetCameraPosition().y));
	}
	if (Focus->GetPosition().y > m_ScreenSize.height / 2 && Focus->GetPosition().y < m_WorldSize.height - m_ScreenSize.height / 2){
		this->SetCameraOffset(SGD::Point(this->GetCameraOffset().x, this->GetCameraOffset().y + nextmove.y));
		this->SetCameraPos(SGD::Point(this->GetCameraPosition().x, this->GetCameraPosition().y + nextmove.y));
	}

	m_CameraBounds = { this->GetCameraPosition().x - (m_ScreenSize.width / 2) - 32,
		this->GetCameraPosition().y - (m_ScreenSize.height / 2) - 32,
		this->GetCameraPosition().x + (m_ScreenSize.width / 2) + 32,
		this->GetCameraPosition().y + (m_ScreenSize.height / 2) + 32 };
}

void Camera::SetCamFocus(Entity* f){
	if (Focus != nullptr)
		Focus = nullptr;

	Focus = f;

	ResetCamera();
}

void Camera::ResetCamera(){

	m_ScreenSize = Game::GetInstance()->GetScreenRes();

	if (Focus != nullptr){
		if (Focus->GetPosition().x < m_ScreenSize.width / 2)
			m_CamPos.x = m_ScreenSize.width / 2;
		else if (Focus->GetPosition().x > m_WorldSize.width - m_ScreenSize.width / 2)
			m_CamPos.x = m_WorldSize.width - m_ScreenSize.width / 2;
		else
			m_CamPos.x = Focus->GetPosition().x;

		if (Focus->GetPosition().y < m_ScreenSize.height / 2)
			m_CamPos.y = m_ScreenSize.height / 2;
		else if (Focus->GetPosition().y > m_WorldSize.height - m_ScreenSize.height / 2)
			m_CamPos.y = m_WorldSize.height - m_ScreenSize.height / 2;
		else
			m_CamPos.y = Focus->GetPosition().y;

		SGD::Point newOffset = { m_CamPos.x - m_ScreenSize.width / 2, m_CamPos.y - m_ScreenSize.height / 2 };
		m_Offset = newOffset;		
	}
}