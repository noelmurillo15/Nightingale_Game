#include "Layer.h"

#include "Player.h"


Layer::Layer() {

}

Layer::~Layer() {
	m_BushPoints.clear();
	m_CollisionPoints.clear();
	m_SpawnPoints.clear();
	m_Events.clear();
}

void Layer::AddCollisionPoints(SGD::Point t){
	m_CollisionPoints.push_back(t);
}

void Layer::AddBushPoints(SGD::Point t){
	m_BushPoints.push_back(t);
}

void Layer::AddSpawnPoints(SGD::Point wpos, std::string name){

	for (unsigned int x = 0; x < m_SpawnPoints.size(); ++x){
		if (m_SpawnPoints[x].m_name == name){
			wpos.x *= 32.0f;
			wpos.y *= 32.0f;
			m_SpawnPoints[x].m_worldPos.push_back(wpos);
			return;
		}
	}

	SpawnPts tmp;

	wpos.x *= 32.0f;
	wpos.y *= 32.0f;

	tmp.m_name = name;
	tmp.m_worldPos.push_back(wpos);

	m_SpawnPoints.push_back(tmp);
}

void Layer::AddEvents(std::string name, std::vector<SGD::Rectangle> rects){
	Events tmp;

	tmp.m_name = name;
	tmp.m_EventTriggers = rects;

	m_Events.push_back(tmp);
}

SGD::Point Layer::FindSpawnPoint(std::string CreatureName){
	for (unsigned int x = 0; x < m_SpawnPoints.size(); ++x){
		if (m_SpawnPoints[x].m_name == CreatureName){

			int num = rand() % m_SpawnPoints[x].m_worldPos.size();
			return m_SpawnPoints[x].m_worldPos[num];
		}
	}
	return SGD::Point(320.0f, 320.0f);
}

SGD::Point Layer::FindOffScreenSpawnPt(string CreatureName){
	for (unsigned int x = 0; x < m_SpawnPoints.size(); ++x){
		if (m_SpawnPoints[x].m_name == CreatureName){

			SGD::Rectangle cambounds = Camera::GetInstance()->GetCameraBounds();

			int num = rand() % m_SpawnPoints[x].m_worldPos.size();

			while (m_SpawnPoints[x].m_worldPos[num].IsPointInRectangle(cambounds) == true){
				num = rand() % m_SpawnPoints[x].m_worldPos.size();
			}

			return m_SpawnPoints[x].m_worldPos[num];
		}
	}
	return SGD::Point(320.0f, 320.0f);
}

SGD::Point Layer::FindRandomBush(){
	if (m_BushPoints.size() > 0){
		int num = rand() % m_BushPoints.size();
		return m_BushPoints[num];
	}
	return{ 0.0f, 0.0f };
}

SGD::Point Layer::FindCloseBush(SGD::Rectangle ScreenBounds){
	for (unsigned int x = 0; x < m_BushPoints.size(); ++x){
		int num = rand() % m_BushPoints.size();
		if (m_BushPoints[num].x > ScreenBounds.left - 200 && m_BushPoints[num].x < ScreenBounds.right + 200 &&
			m_BushPoints[num].y > ScreenBounds.top - 200 && m_BushPoints[num].y < ScreenBounds.bottom + 200){
			return m_BushPoints[num];
		}
	}
	return FindRandomBush();
}

std::vector<SGD::Rectangle> Layer::FindEventTiles(std::string EventName){
	for (unsigned int x = 0; x < m_Events.size(); ++x){
		if (m_Events[x].m_name == EventName)
			return m_Events[x].m_EventTriggers;
	}
	std::vector<SGD::Rectangle> tmp;
	return tmp;
}

bool Layer::CheckCollision(Entity* p, SGD::Point offset, SGD::Point next){

	SGD::Rectangle tempRect = p->GetRect();
	tempRect.left += next.x + 5;
	tempRect.top += next.y + 5;
	tempRect.right += next.x - 5;
	tempRect.bottom += next.y -5;


	for (unsigned int x = 0; x < m_CollisionPoints.size(); ++x){
		//if (m_CollisionPoints.at(x).IsPointInRectangle(Camera::GetInstance()->GetCameraBounds())){
			SGD::Rectangle tmp = SGD::Rectangle(m_CollisionPoints.at(x).x - offset.x,
				m_CollisionPoints.at(x).y - offset.y, (m_CollisionPoints.at(x).x - offset.x) + 32, (m_CollisionPoints.at(x).y - offset.y) + 32);
			
			
		
				
			if (tempRect.IsIntersecting(tmp) == true)
				return true;
		//}
	}
	return false;
}

bool Layer::CheckWater(Entity* p, SGD::Point offset){
	std::vector<SGD::Rectangle> water = FindEventTiles("Water");

	for (unsigned int x = 0; x < water.size(); ++x){
		if (Camera::GetInstance()->GetCameraBounds().IsIntersecting(water[x])){

			SGD::Rectangle tmp = SGD::Rectangle(water.at(x).left - offset.x,
				water.at(x).top - offset.y, 0, 0);
			tmp.right = tmp.left + 32;
			tmp.bottom = tmp.top + 32;

			if (p->GetRect().IsIntersecting(tmp) == true)
				return true;
		}
	}
	return false;
}

void Layer::SetDebugMode(unsigned int mode) {
	if (DEBUG_MODE == mode)
		DEBUG_MODE = DEBUG_OFF;
	else
		DEBUG_MODE = mode;
}

void Layer::ResetLayer(){
	m_BushPoints.clear();
	m_CollisionPoints.clear();
	m_SpawnPoints.clear();
	m_Events.clear();
}