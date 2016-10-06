#include "TileEngine.h"

#include "Camera.h"
#include "tinyxml.h"

#include <string>

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"


TileEngine* TileEngine::GetInstance(void) {
	static TileEngine s_Instance;
	return &s_Instance;
}

void TileEngine::Enter(void) {

		//	Load textures		
	_World0 = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/XMLFiles/World0/World0.jpg");
	_World1 = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/XMLFiles/World1/World1.jpg");
	_World2 = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/XMLFiles/World2/World2.jpg");
	_World3 = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/XMLFiles/World3/World3.jpg");
	_World4 = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/XMLFiles/World4/World4.jpg");
	_World5 = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/XMLFiles/World5/World5.jpg");

	Background = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/loading_screen.png");
	Icon = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/loadingIcon.png");
	m_LoadingBGM = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/NightengaleLoadMenuMusic.xwm");

		//	Initialize Camera
	Camera::GetInstance()->Enter();

		//	Start up load screen
	UpdateRender();

	red = 0.0f;
	blue = 0.0f;
	green = 0.0f;

	_red = false;
	_blue = false;
	_green = false;

	m_World = E_BASEWORLD;
}

void TileEngine::Exit(void) {

		//	Unload Textures
	SGD::GraphicsManager::GetInstance()->UnloadTexture(_World0);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(_World1);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(_World2);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(_World3);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(_World4);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(_World5);

		//	delete each tile

	for (unsigned int x = 0; x < m_tiles.size(); ++x)
		delete m_tiles[x];

	m_tiles.clear();

	SGD::GraphicsManager::GetInstance()->UnloadTexture(Background);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(Icon);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_LoadingBGM); 

	Camera::GetInstance()->Exit();
}

void TileEngine::Update(float elapsedTime){
	Camera::GetInstance()->Update(elapsedTime);
}

void TileEngine::Render(){

	SGD::GraphicsManager* graphics = SGD::GraphicsManager::GetInstance();
	Camera* m_camera = Camera::GetInstance();

	for (unsigned int x = 0; x < m_tiles.size(); ++x){

			//	Get World Pos of tile
		SGD::Point p = m_tiles.at(x)->GetWorldPosition();				

			//	Tile Culling
		if (p.IsPointInRectangle(m_camera->GetCameraBounds())){

			//	Auto scale tiles based on width & height
			SGD::Size tileScale = SGD::Size(
				32.0f / m_TileSize.width,
				32.0f / m_TileSize.height);

				//	Set has explored if in camera bounds
			if (!m_tiles.at(x)->GetHasExplored())
				m_tiles.at(x)->ToggleHasExplored();

			SGD::Rectangle z = SGD::Rectangle();
			{	//	Convert and offset tile

				SGD::Size type = Convert1d_2d(m_tiles.at(x)->GetTileType());

				z.left = type.width * m_TileSize.width;
				z.top = type.height * m_TileSize.height;
				z.right = z.left + m_TileSize.width;
				z.bottom = z.top + m_TileSize.height;

				p.x -= m_camera->GetCameraOffset().x;
				p.y -= m_camera->GetCameraOffset().y;
			}

			switch (m_World){

			case E_BASEWORLD:
				graphics->DrawTextureSection(_World0, p, z, 0.0f, {}, { 255, 255, 255, 255 }, tileScale);
				break;
			case E_WORLD1:
				graphics->DrawTextureSection(_World1, p, z, 0.0f, {}, { 255, 110, 110, 110 }, tileScale);
				break;
			case E_WORLD2:
				graphics->DrawTextureSection(_World2, p, z, 0.0f, {}, { 255, 180, 180, 180 }, tileScale);
				break;
			case E_WORLD3:
				graphics->DrawTextureSection(_World3, p, z, 0.0f, {}, { 255, 180, 180, 180 }, tileScale);
				break;
			case E_WORLD4:
				graphics->DrawTextureSection(_World4, p, z, 0.0f, {}, { 255, 180, 180, 180 }, tileScale);
				break;
			case E_WORLD5:
				graphics->DrawTextureSection(_World5, p, z, 0.0f, {}, { 255, 110, 110, 110 }, tileScale);
				break;

			default: 
				break;
			}					
		}		
	}
}

void TileEngine::RenderMiniMap(SGD::Rectangle p){
	
	SGD::GraphicsManager* g = SGD::GraphicsManager::GetInstance();
	Camera* cam = Camera::GetInstance();

	SGD::Size screen = Camera::GetInstance()->GetScreenRes();
	SGD::Size minimapScale = SGD::Size(1.0f, 1.0f);
	SGD::Color color;

		//	Mini-map - loop thru all tiles
	for (unsigned int x = 0; x < m_tiles.size(); ++x){		
		
		SGD::Point pos = m_tiles.at(x)->GetWorldPosition();
		
		
		if (m_tiles.at(x)->GetHasExplored())
			color = { 255, 255, 255, 255 };
		else
			color = { 255, 0, 0, 0 };


		SGD::Rectangle rectSection = SGD::Rectangle();

		{	//	Get rectangle section of each tile			
			SGD::Size type = Convert1d_2d(m_tiles.at(x)->GetTileType());
			rectSection.left = type.width * m_TileSize.width;
			rectSection.top = type.height * m_TileSize.height;
			rectSection.right = rectSection.left + m_TileSize.width;
			rectSection.bottom = rectSection.top + m_TileSize.height;
		}		

		{
			SGD::Rectangle tile = { pos.x, pos.y, pos.x + 3, pos.y + 3 };
			SGD::Rectangle tmp;
			tmp.left = p.left + cam->GetCameraOffset().x - 64;
			tmp.top = p.top + cam->GetCameraOffset().y - 64;
			tmp.right = tmp.left + 96;
			tmp.bottom = tmp.top + 96;
			if (tmp.IsIntersecting(tile))
				color = { 0, 0, 255 };
		}

		pos.x /= 8;
		pos.y /= 8;

		pos.x += screen.width / 4;
		pos.y += screen.height / 4;

		if (m_World == E_WORLD1)
			g->DrawTextureSection(_World1, pos, rectSection, 0.0f, {}, color, minimapScale);
		else if (m_World == E_WORLD2)
			g->DrawTextureSection(_World2, pos, rectSection, 0.0f, {}, color, minimapScale);
		else if (m_World == E_WORLD3)
			g->DrawTextureSection(_World3, pos, rectSection, 0.0f, {}, color, minimapScale);
		else if (m_World == E_WORLD4)
			g->DrawTextureSection(_World4, pos, rectSection, 0.0f, {}, color, minimapScale);
	}		
}

void TileEngine::LoadMap(unsigned int worldNum){
	
	std::string xmll = "resource/XMLFiles/World" + std::to_string(worldNum) + "/testmap.xml";

	red = (float)(rand() % 254);
	blue = (float)(rand() % 254);
	green = (float)(rand() % 254);

	barLength = 0;
	UpdateRender();
	
	m_World = worldNum;
	

	for (unsigned int x = 0; x < m_tiles.size(); ++x) {		
		delete m_tiles[x];		
	}		
	m_tiles.clear();

	Camera::GetInstance()->GetMapLayer()->ResetLayer();	

		//	Initialize and load Doc
	TiXmlDocument doc(xmll.c_str());
	if (!doc.LoadFile()) return;
		//	Doc's handle
	TiXmlHandle hDoc(&doc);
		//	Root of the xml doc
	TiXmlElement* root;
	root = hDoc.ChildElement(0).Element();
	if (!root) return;
	//	Root's child
	TiXmlNode* element = root->LastChild();

	{	//	Tile Engine Settings		

		float tmpCreatCount = 3;
		float tilewidth = 0;
		float tileheight = 0;
		float tilesetwidth = 0;
		float tilesetheight = 0;
		float tmpwidth = 0;
		float tmpheight = 0;

		element->ToElement()->QueryFloatAttribute("TileWidth", &tilewidth);
		element->ToElement()->QueryFloatAttribute("TileHeight", &tileheight);
		element->ToElement()->QueryFloatAttribute("TileSetWidth", &tilesetwidth);
		element->ToElement()->QueryFloatAttribute("TileSetHeight", &tilesetheight);
		element->ToElement()->QueryFloatAttribute("WorldWidth", &tmpwidth);
		element->ToElement()->QueryFloatAttribute("WorldHeight", &tmpheight);
			
		SetTileSize(SGD::Size(tilewidth, tileheight));
		SetTileSetSize(SGD::Size(tilesetwidth, tilesetheight));
		Camera::GetInstance()->SetWorldSize(SGD::Size(tmpwidth, tmpheight));
	}

	element = root->FirstChildElement();
	while (element != root->LastChild()) {		
		float tmprow = 0;
		float tmpcol = 0;
		int tmptype = 0;
		bool col = false;
		bool bush = false;

		element->ToElement()->QueryFloatAttribute("Row", &tmprow);
		element->ToElement()->QueryFloatAttribute("Col", &tmpcol);
		element->ToElement()->QueryIntAttribute("Type", &tmptype);
		element->ToElement()->QueryBoolAttribute("Collision", &col);
		element->ToElement()->QueryBoolAttribute("Bush", &bush);


		SGD::Point worldpos = SGD::Point(tmpcol * 32, tmprow * 32);
		Tile* tmp = new Tile(worldpos, tmptype);
		Camera::GetInstance()->InitializeLayer(bush, col, tmp->GetWorldPosition());

		m_tiles.push_back(tmp);

		UpdateRender();
		element = element->NextSiblingElement();		
	}	
	LoadSpawnPts("/testspawn.xml", m_World);
}

void TileEngine::LoadSpawnPts(string filename, unsigned int worldNum){

	std::string xmll = "resource/XMLFiles/World" + std::to_string(worldNum) + filename;	

	TiXmlDocument doc(xmll.c_str());
	if (!doc.LoadFile()) return;

	TiXmlHandle hDoc(&doc);
	TiXmlElement* root;
	root = hDoc.ChildElement(0).Element();
	if (!root) return;

	TiXmlNode* element = root->FirstChild();


	while (element != NULL) {		
		TiXmlAttribute att;

		float worldposx = 0;
		float worldposy = 0;
		string name;

		element->ToElement()->QueryFloatAttribute("WorldPosX", &worldposx);
		element->ToElement()->QueryFloatAttribute("WorldPosY", &worldposy);
		name = element->ToElement()->Attribute("Creature");		

		SGD::Point tmppt = SGD::Point(worldposx, worldposy);
		Camera::GetInstance()->GetMapLayer()->AddSpawnPoints(tmppt, name);

		element = element->NextSibling();	

		UpdateRender();
	}
	LoadEvents("/testevent.xml", m_World);
}

void TileEngine::LoadEvents(string filename, unsigned int worldNum){
	std::string xmll = "resource/XMLFiles/World" + std::to_string(worldNum) + filename;

	TiXmlDocument doc(xmll.c_str());
	if (!doc.LoadFile()) return;

	TiXmlHandle hDoc(&doc);
	TiXmlElement* root;
	root = hDoc.ChildElement(0).Element();
	if (!root) return;

	TiXmlNode* element = root->FirstChild();


	while (element != NULL) {

		UpdateRender();

		TiXmlAttribute att;
		string name;
		SGD::Point leftop;
		std::vector<SGD::Rectangle> tmprects;
		unsigned int count;

		name = element->ToElement()->Attribute("Name");
		element->ToElement()->QueryUnsignedAttribute("Count", &count);


		for (unsigned int x = 0; x < count; ++x){			
			string att1 = "WorldPosX";
			string att2 = "WorldPosY";

			string s = std::to_string(x);

			att1 = att1 + s;
			att2 = att2 + s;

			element->ToElement()->QueryFloatAttribute(att1.c_str(), &leftop.x);
			element->ToElement()->QueryFloatAttribute(att2.c_str(), &leftop.y);

			leftop.x *= 32;
			leftop.y *= 32;

			SGD::Point botright = SGD::Point(leftop.x + 32, leftop.y + 32);

			SGD::Rectangle tmp = SGD::Rectangle(leftop, botright);

			tmprects.push_back(tmp);
			UpdateRender();
		}

		Camera::GetInstance()->GetMapLayer()->AddEvents(name, tmprects);
		UpdateRender();
		
		element = element->NextSibling();
	}
}

int TileEngine::Convert2d_1d(int x, int y, int width) {
	int type = 0;
	type = x + (y * (width - 1)) + y;
	return type;
}

SGD::Size TileEngine::Convert1d_2d(int i) {
	int y = 0;
	int w = (int)m_TileSetSize.width;

	y = (int)(i / w);
	i -= (y * w);

	SGD::Size tmp = SGD::Size((float)i, (float)y);
	return tmp;
}

float TileEngine::RotateIcon()
{
	return Rotation += 0.005f;
}

void TileEngine::UpdateColor(){
	if (red > 0 && _red)
		red -= 0.0925f;
	else if (red < 254)
		red += 0.0925f;

	if (red > 252 && red < 255 && !_red)
		_red = true;
	else if (red > 0 && red < 1 && _red)
		_red = false;

	if (blue > 0 && _blue)
		blue -= 0.0225f;
	else if (blue < 254)
		blue += 0.0225f;

	if (blue > 252 && blue < 255 && !_blue)
		_blue = true;
	else if (blue > 0 && blue < 1 && _blue)
		_blue = false;

	if (green > 0 && _green)
		green -= 0.0525f;
	else if (green < 254)
		green += 0.0525f;

	if (green > 252 && green < 255 && !_green)
		_green = true;
	else if (green > 0 && green < 1 && _green)
		_green = false;
}

void TileEngine::UpdateRender()
{
	SGD::GraphicsManager* G_Manager = SGD::GraphicsManager::GetInstance();

	SGD::Size screenres = Camera::GetInstance()->GetScreenRes();
	
	SGD::Size scale = screenres;
	scale.width /= 1000;
	scale.height /= 1000;


	barLength = barLength + (0.045f * scale.width);

	SGD::Rectangle TempRct = SGD::Rectangle(SGD::Point(50, 150), SGD::Size(barLength, 600));

	G_Manager->DrawRectangle(TempRct, SGD::Color((char)red, (char)blue, (char)green));
	G_Manager->DrawTexture(Background, { 0, 0 }, {}, {}, SGD::Color((char)red, (char)blue, (char)green), { scale.width, scale.height });

	UpdateColor();
	RotateIcon();

	if (screenres == SGD::Size(800, 600))
		G_Manager->DrawTexture(Icon, { screenres.width - 100, screenres.height - 100 }, Rotation, { 256 / 2, 256 / 2 }, SGD::Color((char)red, (char)blue, (char)green), { .8f, .8f });
	else	
		G_Manager->DrawTexture(Icon, { screenres.width / 2 - 100, screenres.height - 200 }, Rotation, { 256 / 2, 256 / 2 }, SGD::Color((char)red, (char)blue, (char)green), { .8f, .8f });

	G_Manager->Update();
}