#include "Kao.h"


Kao::Kao(){
	SetName("Kao");
	SetSize(SGD::Size(50, 50));
	this->LoadData({ 0, 0 }, { 0, 0 }, { 0, 0 }, {50*32, 5*32});
	AnimationSystem::GetInstance()->Load("Kao", "Kao");
}

Kao::~Kao(){	

}

void Kao::Update(float elapsedTime){
	UpdateBaseRect(Camera::GetInstance()->GetCameraOffset());
}

void Kao::Render(SGD::Point bawlz) {
	AnimationSystem::GetInstance()->Render(this->GetTimestamp(), GetPosition().x - bawlz.x, GetPosition().y - bawlz.y, { 1.25, 1.25 }, { 255, 255, 255 });
}

void Kao::HandleCollision(const IEntity* pOther){

}

void Kao::PreformItemBehavaior(const IEntity* pOther){

}