#include "HUD.h"

#include "Player.h"
#include "Game.h"

#include "../SGD Wrappers/SGD_String.h"


HUD::HUD() {
		//Load the textures for the HUD and the images
	topHUD = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/Nightengale_TopHUD.png"); 
	bottomHUD = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/Nightengale_BottomHUD.png"); 
	baitImg = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/Nightengale_Bait.png"); 
	repellentImg = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/Nightengale_Repel.png"); 
	analyzerImg = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/Nightengale_Analyzer.png"); 
}

HUD::~HUD() {
		//Unload the textures for the HUD and the images
	SGD::GraphicsManager::GetInstance()->UnloadTexture(topHUD); 
	SGD::GraphicsManager::GetInstance()->UnloadTexture(bottomHUD);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(baitImg); 
	SGD::GraphicsManager::GetInstance()->UnloadTexture(repellentImg); 
	SGD::GraphicsManager::GetInstance()->UnloadTexture(analyzerImg); 
}

void HUD::Render() {

	BitmapFont* pFont = Game::GetInstance()->GetFont();
	SGD::Point camoffset = Camera::GetInstance()->GetCameraOffset();
	SGD::Size screenres = Game::GetInstance()->GetScreenRes();


	SGD::Size scale = screenres;
	scale.width /= 1200;
	scale.height /= 1200;


		//	Render Hud textures
	SGD::GraphicsManager::GetInstance()->DrawTexture(topHUD, SGD::Point(screenres.width / 4, screenres.height - (60 * scale.height) - 18), 0.0f, {}, {}, {1.38f,1.0f});
	SGD::GraphicsManager::GetInstance()->DrawTexture(bottomHUD, SGD::Point(screenres.width / 4, screenres.height - (60 * scale.height) - 2), 0.0f, {}, {}, scale);

		//	Render Selected Rect
	if (m_icurrSel == 0)
		SGD::GraphicsManager::GetInstance()->DrawRectangle(SGD::Rectangle{ SGD::Point(screenres.width / 4 + (20 * scale.width), screenres.height - (45 * scale.height)), SGD::Size{ (32 * scale.width), (32 * scale.height) } }, {}, { 255, 255, 0 }, 3);

	else if (m_icurrSel == 1)
		SGD::GraphicsManager::GetInstance()->DrawRectangle(SGD::Rectangle{ SGD::Point(screenres.width / 4 + (123 * scale.width), screenres.height - (45 * scale.height)), SGD::Size{ (32 * scale.width), (32 * scale.height) } }, {}, { 255, 255, 0 }, 3);

	else if (m_icurrSel == 2)
		SGD::GraphicsManager::GetInstance()->DrawRectangle(SGD::Rectangle{ SGD::Point(screenres.width / 4 + (228 * scale.width), screenres.height - (45 * scale.height)), SGD::Size{ (32 * scale.width), (32 * scale.height) } }, {}, { 255, 255, 0 }, 3);

		//	Render Item Image
	SGD::GraphicsManager::GetInstance()->DrawTexture(analyzerImg, SGD::Point(screenres.width / 4 + (228 * scale.width), screenres.height - (45 * scale.height)), 0.0f, {}, {}, scale);
	SGD::GraphicsManager::GetInstance()->DrawTexture(repellentImg, SGD::Point(screenres.width / 4 + (123 * scale.width), screenres.height - (45 * scale.height)), 0.0f, {}, {}, scale);
	SGD::GraphicsManager::GetInstance()->DrawTexture(baitImg, SGD::Point(screenres.width / 4 + (20 * scale.width), screenres.height - (45 * scale.height)), 0.0f, {}, {}, scale);

		//	Render the number of baits 
	SGD::OStringStream osBaits;
	osBaits << GetCurrentPlayer()->GetNumBaits();
	pFont->Draw(osBaits.str().c_str(), "Game", SGD::Point(screenres.width / 4 + (65 * scale.width), screenres.height - (28 * scale.height)), scale.width * 0.4f, SGD::Color(0, 0, 0));

		//	Render the number of repellents
	SGD::OStringStream osRepels;
	osRepels << GetCurrentPlayer()->GetNumRepellents();
	pFont->Draw(osRepels.str().c_str(), "Game", SGD::Point(screenres.width / 4 + (167 * scale.width), screenres.height - (28 * scale.height)), scale.width * 0.4f, SGD::Color(0, 0, 0));

		//	Render the health & Stamina bar
	if (GetCurrentPlayer()->GetHealth() > 0) 
		SGD::GraphicsManager::GetInstance()->DrawRectangle(SGD::Rectangle(screenres.width / 4 + 5,
		screenres.height - (60 * scale.height) - 15, screenres.width / 4 + GetCurrentPlayer()->GetHealth() + 5, screenres.height - (60 * scale.height) -10), SGD::Color(255, 0, 0));
	
	if (GetCurrentPlayer()->GetStamina() > 0)
		SGD::GraphicsManager::GetInstance()->DrawRectangle(SGD::Rectangle(screenres.width / 4 + 5,
		screenres.height - (60 * scale.height) - 10, screenres.width / 4 + GetCurrentPlayer()->GetStamina() + 5, screenres.height - (60 * scale.height) -5 ), SGD::Color(0, 0, 255));
}