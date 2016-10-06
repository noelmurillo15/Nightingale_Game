#include "StaticObject.h"

#include "AnimationSystem.h"
#include "Entity.h"
#include "Camera.h"

#include "../SGD Wrappers/SGD_AudioManager.h"


SGD::HAudio StaticObject::m_aSound = SGD::INVALID_HANDLE;

StaticObject::StaticObject(SGD::Point Pos) {
	m_ptPosition = Pos;
	m_Rect.Resize(this->m_sSize);
	m_TimeStamp = new AnimationTimestamp("Brush", "Bush");

	AnimationSystem::GetInstance()->Load("Brush", m_TimeStamp->m_SCurrentObject);

	if (m_aSound == SGD::INVALID_HANDLE)
		m_aSound = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/Twig.wav");
}

StaticObject::~StaticObject() {
	if (m_aSound != SGD::INVALID_HANDLE) {
		SGD::AudioManager::GetInstance()->UnloadAudio(m_aSound);
		m_aSound = SGD::INVALID_HANDLE;
	}
	delete m_TimeStamp;
}

void StaticObject::Update(float elapsedTime) {

	if (m_Occupied)
		AnimationSystem::GetInstance()->Update(m_TimeStamp, elapsedTime);
	else
		m_TimeStamp->m_uICurrentFrame = 0;
	
	m_Occupied = false;
}

void StaticObject::Render(SGD::Point bawlz) {

	
	m_Rect.MoveTo({ m_ptPosition.x, m_ptPosition.y });

	if (m_Rect.IsIntersecting(Camera::GetInstance()->GetCameraBounds()))
		AnimationSystem::GetInstance()->Render(m_TimeStamp, m_ptPosition.x - bawlz.x, m_ptPosition.y - bawlz.y + 25, { 1, 1 }, {});
	
	m_Rect.MoveTo({ m_ptPosition.x - bawlz.x, m_ptPosition.y - bawlz.y - m_sSize.height / 2 + 15 });
}

void StaticObject::HandleCollision(const IEntity* pOther) {

	if (pOther->GetRect().IsIntersecting(this->GetRect())){

		if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_aSound))
		{
			SGD::Vector H = Camera::GetInstance()->GetCameraPosition() - m_ptPosition;
			float g = ((abs(H.x) + abs(H.y)) / 15);
			float b = 50 - g;


			SGD::AudioManager::GetInstance()->SetAudioVolume(m_aSound, (int)b);
			SGD::AudioManager::GetInstance()->PlayAudio(m_aSound);
		}
		m_Occupied = true;
	}
}

int StaticObject::GetType() const {
	return Entity::ENT_BUSH;
}