#include "CreatureLogs.h"

#include "Entity.h"
#include "Analyzer.h"
#include "BitmapFont.h"
#include "GameplayState.h"

#include "../SGD Wrappers/SGD_InputManager.h"
#include "../resource.h"

#include <atlstr.h>


CreatureLogs::CreatureLogs()
{
	SGD::GraphicsManager* graphics = SGD::GraphicsManager::GetInstance(); 

	m_hKnight = graphics->LoadTexture(L"resource/graphics/Knight.png"); 
	m_hKing = graphics->LoadTexture(L"resource/graphics/King.png");
	m_hQueen = graphics->LoadTexture(L"resource/graphics/Queen.png");
	m_hPawn = graphics->LoadTexture(L"resource/graphics/Pawn.png");
	m_hPrancer = graphics->LoadTexture(L"resource/graphics/Prancer.png");
	m_hBishop = graphics->LoadTexture(L"resource/graphics/BishopIMG.png");
	m_hRook = graphics->LoadTexture(L"resource/graphics/Rook.png");
	m_hCastle = graphics->LoadTexture(L"resource/graphics/Castle.png");
	m_hPassant = graphics->LoadTexture(L"resource/graphics/Passant.png");
	m_hBumbles = graphics->LoadTexture(L"resource/graphics/Bumbles.png");
	m_hHUDFrame = graphics->LoadTexture(L"resource/graphics/CreatureLogBox.png");

	m_bShowingLog = false; 
	for (int i = 0; i < 10; i++)
		m_aScannedCreatures[i] = false; 
	//m_aScannedCreatures[3] = true; 
	m_fHardcodedTimer = 0.5; 
}


CreatureLogs::~CreatureLogs()
{
	SGD::GraphicsManager* graphics = SGD::GraphicsManager::GetInstance(); 

	graphics->UnloadTexture(m_hHUDFrame);
	graphics->UnloadTexture(m_hBumbles); 
	graphics->UnloadTexture(m_hKing); 
	graphics->UnloadTexture(m_hQueen); 
	graphics->UnloadTexture(m_hPawn); 
	graphics->UnloadTexture(m_hRook);
	graphics->UnloadTexture(m_hCastle);
	graphics->UnloadTexture(m_hKnight);
	graphics->UnloadTexture(m_hPassant);
	graphics->UnloadTexture(m_hBishop);
	graphics->UnloadTexture(m_hPrancer);
}

void CreatureLogs::Update(float elapsedTime)
{
	m_fArcadeBuffer -= elapsedTime; 
	SGD::InputManager* input = SGD::InputManager::GetInstance();
	m_fHardcodedTimer -= elapsedTime;
	if (true == m_bShowingLog)//Otherwise, only escape works to bring you out of the log 
	{
		if (input->IsKeyPressed(SGD::Key::Escape) && m_fArcadeBuffer <= 0 ||
			true == input->IsControllerConnected(0) && input->IsButtonPressed(0, 1) && m_fArcadeBuffer <= 0)
		{
			GameplayState::GetInstance()->SetIsInCreatureLog(true);
			m_bShowingLog = false;
			m_fArcadeBuffer = 0.2f; 
		}
		//if (input->IsKeyPressed(SGD::Key::Enter) ||
		//	true == input->IsControllerConnected(0) && input->IsButtonPressed(0, 0))
		//{
		//	GameplayState::GetInstance()->SetIsInCreatureLog(true);
		//	m_bShowingLog = false;
		//}
	}
	else if (false == m_bShowingLog)//If not showing a log, menu functions normally
	{
		//	Arrow keys at creature log
		if (input->IsKeyPressed(SGD::Key::UpArrow) ||
			input->IsControllerConnected(0) && input->IsDPadPressed(0, SGD::DPad::Up) ||
			input->IsControllerConnected(0) && input->GetLeftJoystick(0).ComputeNormalized() == SGD::Vector(0, -1) && m_fArcadeBuffer <= 0)
		{
			m_fArcadeBuffer = 0.2f; 
			if (m_iCursor == 1)
				m_iCursor = 11;
			else if (m_iCursor == 2)
				m_iCursor = 1;
			else if (m_iCursor == 3)
				m_iCursor = 2;
			else if (m_iCursor == 4)
				m_iCursor = 3;
			else if (m_iCursor == 5)
				m_iCursor = 4;
			else if (m_iCursor == 6)
				m_iCursor = 5;
			else if (m_iCursor == 7)
				m_iCursor = 6;
			else if (m_iCursor == 8)
				m_iCursor = 7;
			else if (m_iCursor == 9)
				m_iCursor = 8;
			else if (m_iCursor == 10)
				m_iCursor = 9;
			else if (m_iCursor == 11)
				m_iCursor = 10;
		}
		else if (input->IsKeyPressed(SGD::Key::DownArrow) ||
			input->IsControllerConnected(0) && input->IsDPadPressed(0, SGD::DPad::Down) ||
			input->IsControllerConnected(0) && input->GetLeftJoystick(0).ComputeNormalized() == SGD::Vector(0, 1) && m_fArcadeBuffer <= 0)
		{
			m_fArcadeBuffer = 0.2f; 
			if (m_iCursor == 1)
				m_iCursor = 2;
			else if (m_iCursor == 2)
				m_iCursor = 3;
			else if (m_iCursor == 3)
				m_iCursor = 4;
			else if (m_iCursor == 4)
				m_iCursor = 5;
			else if (m_iCursor == 5)
				m_iCursor = 6;
			else if (m_iCursor == 6)
				m_iCursor = 7;
			else if (m_iCursor == 7)
				m_iCursor = 8;
			else if (m_iCursor == 8)
				m_iCursor = 9;
			else if (m_iCursor == 9)
				m_iCursor = 10;
			else if (m_iCursor == 10)
				m_iCursor = 11;
			else if (m_iCursor == 11)
				m_iCursor = 1;
		}
		if (input->IsKeyPressed(SGD::Key::Escape) ||
			true == input->IsControllerConnected(0) && input->IsButtonPressed(0, 1) && m_fArcadeBuffer <= 0)
		{
			m_fHardcodedTimer = 0.5f;
			m_fArcadeBuffer = 0.2f; 
			GameplayState::GetInstance()->SetIsInCreatureLog(false);
		}
		if (m_fHardcodedTimer <= 0 && true == GameplayState::GetInstance()->GetIsInCreatureLog() && input->IsKeyPressed(SGD::Key::Enter) ||
			m_fHardcodedTimer <= 0 && true == GameplayState::GetInstance()->GetIsInCreatureLog() && input->IsControllerConnected(0) && input->IsButtonPressed(0, 0)){
			{
				m_fHardcodedTimer = 0.5f;
				switch (m_iCursor)
				{
				case 1:
				{
				GetLog(Entity::CRT_BISHOP);
				m_bShowingLog = true;
				break;
				}
				case 2:
				{
				GetLog(Entity::CRT_BUMBLES);
				m_bShowingLog = true;
				break;
				}
				case 3:
				{
				GetLog(Entity::CRT_CASTLE);
				m_bShowingLog = true;
				break;
				}
				case 4:
				{
				GetLog(Entity::CRT_KING);
				m_bShowingLog = true;
				break;
				}
				case 5:
				{
				GetLog(Entity::CRT_KNIGHT);
				m_bShowingLog = true;
				break;
				}
				case 6:
				{
				GetLog(Entity::CRT_PASSANT);
				m_bShowingLog = true;
				break;
				}
				case 7:
				{
				GetLog(Entity::CRT_PAWN);
				m_bShowingLog = true;
				break;
				}
				case 8:
				{
				GetLog(Entity::CRT_PRANCER);
				m_bShowingLog = true;
				break;
				}
				case 9:
				{
				GetLog(Entity::CRT_QUEEN);
				m_bShowingLog = true;
				break;
				}
				case 10:
				{
				GetLog(Entity::CRT_ROOK);
				m_bShowingLog = true;
				break;
				}
				case 11:
				{
				m_bShowingLog = false;				
				GameplayState::GetInstance()->SetIsInCreatureLog(false);
				break;
				}
				default:
				break;
				}
			}
		}
	}
}

void CreatureLogs::Render(float elapsedTime)
{
	SGD::GraphicsManager* graphics = SGD::GraphicsManager::GetInstance(); 

	BitmapFont* pFont = Game::GetInstance()->GetFont(); 

	SGD::Rectangle screenRekt = {
		256, 200,
		Game::GetInstance()->GetScreenRes().width - 256,
		Game::GetInstance()->GetScreenRes().height - 256
		};	
	//graphics->DrawRectangle(m_pause.m_PauseScreen,
	//	SGD::Color(160, 160, 160), SGD::Color(0, 0, 0));

	SGD::Size screenres = Game::GetInstance()->GetScreenRes(); 

	if (false == m_bShowingLog)
	{
		if (m_iCursor == 1)
		{
			pFont->Draw("-", "Test", { screenres.width / 2 - 170, screenRekt.top + (40 * (m_iCursor -1) ) },
				1.0f, { 0, 255, 0 });
		}
		else if (m_iCursor == 2)
		{
			pFont->Draw("-", "Test", { screenres.width / 2 - 170, screenRekt.top + (40 * (m_iCursor - 1)) },
				1.0f, { 0, 255, 0 });
		}
		else if (m_iCursor == 3)
		{
			pFont->Draw("-", "Test", { screenres.width / 2 - 170, screenRekt.top + (40 * (m_iCursor - 1)) },
				1.0f, { 0, 255, 0 });
		}
		else if (m_iCursor == 4)
		{
			pFont->Draw("-", "Test", { screenres.width / 2 - 170, screenRekt.top + (40 * (m_iCursor - 1)) },
				1.0f, { 0, 255, 0 });
		}
		else if (m_iCursor == 5)
		{
			pFont->Draw("-", "Test", { screenres.width / 2 - 170, screenRekt.top + (40 * (m_iCursor - 1)) },
				1.0f, { 0, 255, 0 });
		}
		else if (m_iCursor == 6)
		{
			pFont->Draw("-", "Test", { screenres.width / 2 - 170, screenRekt.top + (40 * (m_iCursor - 1)) },
				1.0f, { 0, 255, 0 });
		}
		else if (m_iCursor == 7)
		{
			pFont->Draw("-", "Test", { screenres.width / 2 - 170, screenRekt.top + (40 * (m_iCursor - 1)) },
				1.0f, { 0, 255, 0 });
		}
		else if (m_iCursor == 8)
		{
			pFont->Draw("-", "Test", { screenres.width / 2 - 170, screenRekt.top + (40 * (m_iCursor - 1)) },
				1.0f, { 0, 255, 0 });
		}
		else if (m_iCursor == 9)
		{
			pFont->Draw("-", "Test", { screenres.width / 2 - 170, screenRekt.top + (40 * (m_iCursor - 1)) },
				1.0f, { 0, 255, 0 });
		}
		else if (m_iCursor == 10)
		{
			pFont->Draw("-", "Test", { screenres.width / 2 - 170, screenRekt.top + (40 * (m_iCursor - 1)) },
				1.0f, { 0, 255, 0 });
		}
		else if (m_iCursor == 11)
		{
			pFont->Draw("-", "Test", { screenres.width / 2 - 170, screenRekt.top + (40 * (m_iCursor - 1)) },
				1.0f, { 0, 255, 0 });
		}
		for (int numberOfCreatures = 0; numberOfCreatures < 10 /*Total number of creatures*/; numberOfCreatures++)
		{
			if (true == m_aScannedCreatures[numberOfCreatures]) //If the creature is analyzed show its name
			{
				/*
					Order will be alphabetical:
					Bishop
					Bumbles
					Castle
					King
					Knight
					Passant
					Pawn
					Prancer
					Queen
					Rook
					*/
				switch (numberOfCreatures)
				{
				case 0:
				{
					pFont->Draw("Bishop", "Test", { screenres.width / 2 -150, screenRekt.top + 40 * numberOfCreatures },
						0.75f, { 255, 0, 0 });
					break;
				}
				case 1:
				{
					pFont->Draw("Bumbles", "Test", { screenres.width / 2 - 150, screenRekt.top + 40 * numberOfCreatures },
						0.75f, { 255, 0, 0 });
					break;
				}
				case 2:
				{
					pFont->Draw("Castle", "Test", { screenres.width / 2 - 150, screenRekt.top + 40 * numberOfCreatures },
						0.75f, { 255, 0, 0 });
					break;
				}
				case 3:
				{
					pFont->Draw("King", "Test", { screenres.width / 2 - 150, screenRekt.top + 40 * numberOfCreatures },
						0.75f, { 255, 0, 0 });
					break;
				}
				case 4:
				{
					pFont->Draw("Knight", "Test", { screenres.width / 2 - 150, screenRekt.top + 40 * numberOfCreatures },
						0.75f, { 255, 0, 0 });
					break;
				}
				case 5:
				{
					pFont->Draw("Passant", "Test", { screenres.width / 2 - 150, screenRekt.top + 40 * numberOfCreatures },
						0.75f, { 255, 0, 0 });
					break;
				}
				case 6:
				{
					pFont->Draw("Pawn", "Test", { screenres.width / 2 - 150, screenRekt.top + 40 * numberOfCreatures },
						0.75f, { 255, 0, 0 });
					break;
				}
				case 7:
				{
					pFont->Draw("Prancer", "Test", { screenres.width / 2 - 150, screenRekt.top + 40 * numberOfCreatures },
						0.75f, { 255, 0, 0 });
					break;
				}
				case 8:
				{
					pFont->Draw("Queen", "Test", { screenres.width / 2 - 150, screenRekt.top + 40 * numberOfCreatures },
						0.75f, { 255, 0, 0 });
					break;
				}
				case 9:
				{
					pFont->Draw("Rook", "Test", { screenres.width / 2 - 150, screenRekt.top + 40 * numberOfCreatures },
						0.75f, { 255, 0, 0 });
					break;
				}
				default:
					break;
				}
			}
			else //Else if creature isn't analyzed draw question marks 
				pFont->Draw("CREATURE UNAVAILABLE", "Test", { screenres.width / 2 - 150, screenRekt.top + 40 * numberOfCreatures },
				0.75f, { 255, 0, 0 });

			pFont->Draw("Exit Encyclopedia", "Test", { screenres.width / 2 - 150, screenRekt.top + 400 },
				0.75f, { 255, 0, 0 });
		}
	}
	else if (true == m_bShowingLog)
	{
		graphics->DrawTexture(m_hHUDFrame, SGD::Point(screenres.width / 13, screenres.height / 11), 0.0f, {}, {}, SGD::Size(1.6f, 1.0f));
		//if (Game::GetInstance()->GetCurrentRes() == Game::GetInstance()->CTRL_ARCADE)
		//{
		//	pFont->Draw(m_sLogEntry, "Test", { screenres.width / 10, screenRekt.top }, 0.40f, SGD::Color(255, 255, 255)); 
		//	if (m_hCurrImg != nullptr)
		//		graphics->DrawTexture(*m_hCurrImg, { screenres.width / 2, screenres.height / 1.75f }, 0.0f, {}, {}, SGD::Size(0.4f, 0.4f));
		//}
		//else
		//{
			pFont->Draw(m_sLogEntry, "Test", { screenres.width / 10, screenRekt.top }, 0.75f, SGD::Color(255, 255, 255));
			if (m_hCurrImg != nullptr)
				graphics->DrawTexture(*m_hCurrImg, { screenres.width / 2, screenres.height / 1.75f }, 0.0f, {}, {}, SGD::Size(0.7f, 0.7f));
		//}

	}


}

void CreatureLogs::ScanCreature(int _creatureID)
{
	switch (_creatureID)
	{
	case Entity::CRT_BISHOP:
	{
		m_aScannedCreatures[0] = true;
		break;
	}
	case Entity::CRT_BUMBLES:
	{
		m_aScannedCreatures[1] = true;
		break;
	}
	case Entity::CRT_CASTLE:
	{
		m_aScannedCreatures[2] = true;
		break;
	}
	case Entity::CRT_KING:
	{
		m_aScannedCreatures[3] = true;
		break;
	}
	case Entity::CRT_KNIGHT:
	{
		m_aScannedCreatures[4] = true;
		break;
	}
	case Entity::CRT_PASSANT:
	{
		m_aScannedCreatures[5] = true;
		break;
	}
	case Entity::CRT_PAWN:
	{
		m_aScannedCreatures[6] = true;
		break;
	}
	case Entity::CRT_PRANCER:
	{
		m_aScannedCreatures[7] = true;
		break;
	}
	case Entity::CRT_QUEEN:
	{
		m_aScannedCreatures[8] = true;
		break;
	}
	case Entity::CRT_ROOK:
	{
		m_aScannedCreatures[9] = true;
		break;
	}
	default:
		break;
	}
}

bool CreatureLogs::GetLog(int _ID)
{
	switch (_ID)
	{
	case Entity::CRT_BISHOP:
	{
		if (true == m_aScannedCreatures[0])
		{
			m_hCurrImg = &m_hBishop;
			m_sLogEntry.LoadStringW(IDS_STRINGBISHOPLOG);
			return true;
		}
		else
		{
			m_hCurrImg = nullptr;
			m_sLogEntry.LoadStringW(IDS_STRINGNOLOG);
			return true;
			break;
		}
		break;
	}
	case Entity::CRT_BUMBLES:
	{
		if (m_aScannedCreatures[1] == true)
		{
			m_hCurrImg = &m_hBumbles;
			m_sLogEntry.LoadStringW(IDS_STRINGBUMBLESLOG);
			return true;
		}
		else
		{
			m_hCurrImg = nullptr;
			m_sLogEntry.LoadStringW(IDS_STRINGNOLOG);
			return true;
			break;
		}
		break;
	}
	case Entity::CRT_CASTLE:
	{
		if (m_aScannedCreatures[2] == true)
		{
			m_hCurrImg = &m_hCastle;
			m_sLogEntry.LoadStringW(IDS_STRINGCASTLELOG);
			return true;
		}
		else
		{
			m_hCurrImg = nullptr;
			m_sLogEntry.LoadStringW(IDS_STRINGNOLOG);
			return true;
			break;
		}
		break;
	}
	case Entity::CRT_KING:
	{
		if (m_aScannedCreatures[3] == true)
		{
			m_hCurrImg = &m_hKing;
			m_sLogEntry.LoadStringW(IDS_STRINGKINGLOG);
			return true;
		}
		else
		{
			m_hCurrImg = nullptr;
			m_sLogEntry.LoadStringW(IDS_STRINGNOLOG);
			return true;
			break;
		}
		break;

	}
	case Entity::CRT_KNIGHT:
	{
		if (true == m_aScannedCreatures[4])
		{
			m_hCurrImg = &m_hKnight;
			m_sLogEntry.LoadStringW(IDS_STRINGKNIGHTLOG);
			return true;
		}
		else
		{
			m_hCurrImg = nullptr;
			m_sLogEntry.LoadStringW(IDS_STRINGNOLOG);
			return true;
			break;
		}
		break;
	}
	case Entity::CRT_PASSANT:
	{
		if (m_aScannedCreatures[5] == true)
		{
			m_hCurrImg = &m_hPassant;
			m_sLogEntry.LoadStringW(IDS_STRINGPASSANTLOG);
			return true;
		}
		else
		{
			m_hCurrImg = nullptr;
			m_sLogEntry.LoadStringW(IDS_STRINGNOLOG);
			return true;
			break;
		}
		break;
	}
	case Entity::CRT_PAWN:
	{
		if (m_aScannedCreatures[6] == true)
		{
			m_hCurrImg = &m_hPawn;
			m_sLogEntry.LoadStringW(IDS_STRINGPAWNLOG);
			return true;
		}
		else
		{
			m_hCurrImg = nullptr;
			m_sLogEntry.LoadStringW(IDS_STRINGNOLOG);
			return true;
			break;
		}
		break;
	}
	case Entity::CRT_PRANCER:
	{
		if (m_aScannedCreatures[7] == true)
		{
			m_hCurrImg = &m_hPrancer;
			m_sLogEntry.LoadStringW(IDS_STRINGPRANCERLOG);
			return true;
		}
		else
		{
			m_hCurrImg = nullptr;
			m_sLogEntry.LoadStringW(IDS_STRINGNOLOG);
			return true;
			break;
		}
		break;
	}
	case Entity::CRT_QUEEN:
	{
		if (m_aScannedCreatures[8] == true)
		{
			m_hCurrImg = &m_hQueen;
			m_sLogEntry.LoadStringW(IDS_STRINGQUEENLOG);
			return true;
		}
		else
		{
			m_hCurrImg = nullptr;
			m_sLogEntry.LoadStringW(IDS_STRINGNOLOG);
			return true;
			break;
		}
		break;
	}
	case Entity::CRT_ROOK:
	{
		if (m_aScannedCreatures[9] == true)
		{
			m_hCurrImg = &m_hRook;
			m_sLogEntry.LoadStringW(IDS_STRINGROOKLOG);
			return true;
		}
		else
		{
			m_hCurrImg = nullptr;
			m_sLogEntry.LoadStringW(IDS_STRINGNOLOG);
			return true;
			break;
		}
		break;
	}
	default:
	{
		m_hCurrImg = nullptr; 
		m_sLogEntry.LoadStringW(IDS_STRINGNOLOG);
		return true; 
		break;
	}
	}
//	return false; 
}

bool CreatureLogs::IsScanned(int _ID)
{
	if (_ID >= 0 && _ID < 9)
		return m_aScannedCreatures[_ID];
	else
		return false; 
}

int CreatureLogs::GetNumAnalyzed(void)
{
	int x = 0; 
	for (unsigned int i = 0; i < 10; i++)
	{
		if (true == m_aScannedCreatures[i])
			x++;
	}
	return x; 
}