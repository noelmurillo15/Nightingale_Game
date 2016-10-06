#include "Analyzer.h"

#include "Entity.h"
#include "EntityManager.h"
#include "PackComposite.h"
#include "CreatureLogMessage.h"
#include "Game.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "DestroyEntityMessage.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

Analyzer::Analyzer()
{
	victoria = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/AnalysisSuccessful.wav");
	m_hFailure = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/AnalyzerFail.wav");

	m_hAnalyzerSuccessful = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/ScanSuccessful.png"); 
	m_hCanAnalyze = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/CanScan.png");
	m_hCurrAnalyzing1 = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/scanning1.png");
	m_hCurrAnalyzing2 = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/scanning2.png");
	m_hScanFailed = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/ScanFailed.png");

}


Analyzer::~Analyzer()
{
	SGD::AudioManager::GetInstance()->UnloadAudio(victoria);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hAnalyzerSuccessful); 
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hCanAnalyze);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hCurrAnalyzing1);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hCurrAnalyzing2);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hScanFailed);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hFailure); 
}

void Analyzer::Update(float elapsedTime, Entity* creature, Entity* Player){

	SGD::InputManager* input = SGD::InputManager::GetInstance();

	Cursor = input->GetCursorPosition();

	CanTarget.clear();

	if (input->IsKeyDown(SGD::Key::MouseLeft) || input->IsButtonDown(0, 4) || input->IsButtonDown(0,3)) {

		float DistMod = DISTANCE_CONTROL;

		SGD::Vector distance = Player->GetPosition() - creature->GetPosition();

		if (distance.ComputeLength() < DistMod) {
			DistMod = distance.ComputeLength();
			Target = creature;
		}


		if (Target != nullptr) {

			Entity* TempTarget = (Entity*)Target;
			float DegreeBetween = Player->CheckLoS(TempTarget);
			if (DegreeBetween <= SGD::PI / 4)
			{
				inprogress = true;
				Analysis += elapsedTime;
				BarRenderColor = (Analysis / 3.0f * 255);
				BarRenderLength = (Analysis / 3.0f * 300);
			}
			else
			{
				Ready = false;
				Analysis = 0;
				m_bScanfailed = true;
			}


			if (Analysis >= 3)
			{
				m_bSuccess = true;
				inprogress = false;
				Analysis = 0;

				SGD::AudioManager::GetInstance()->PlayAudio(victoria);

				Target = nullptr;
				Ready = false;
			}
		}
	}
	else{
		Analysis = 0;
		BarRenderColor = 0;
		BarRenderLength = 0;
	}
}

void Analyzer::Update(float elapsedTime, EntityManager* EntityList, Entity* Player)
{
	CanTarget.clear();

	SGD::InputManager* input = SGD::InputManager::GetInstance();
	Cursor = input->GetCursorPosition();
	cooldown -= elapsedTime;

	Cursor = input->GetCursorPosition();
	if (Target != nullptr && Target->GetAlive() == false)
	{
		Target = nullptr;
		Analysis = 0;
		inprogress = false;
		Ready = false;
	}

	if (cooldown < 0)
	if ((input->IsKeyDown(SGD::Key::MouseLeft) || input->IsKeyDown(SGD::Key::V) || input->IsButtonDown(0, 4)) || input->IsButtonDown(0, 3) && cooldown <= 0)
	{
		cooldown = 0;
#pragma region // checks input and makes a vector of creatures within the bounds to be analyzed

		if (Target == nullptr && Player != nullptr)
		{
			for (size_t curr = 0; curr < EntityList->m_tEntities[Entity::ENT_ANIMAL].size(); curr++)
			{
				Entity* m_animal = (Entity*)EntityList->m_tEntities[Entity::ENT_ANIMAL][curr];
				SGD::Vector m_Distance = Player->GetPosition().ComputeDifference(m_animal->GetPosition());

				if (m_Distance.ComputeLength() < DISTANCE_CONTROL) {

					m_bInRange = true;
					float DegreeBetween = Player->CheckLoS(m_animal);

					if (DegreeBetween <= SGD::PI / 4)
					{
						CanTarget.push_back(m_animal);
						break;
					}
				}
				else
					m_bScanfailed = false; 
			}
			for (size_t Currpack = 0; Currpack < EntityList->m_tEntities[Entity::ENT_PACK].size(); Currpack++)
			{
				PackComposite* m_pack = (PackComposite*)EntityList->m_tEntities[Entity::ENT_PACK][Currpack];

				for (size_t curr = 0; curr < m_pack->m_vecPack.size(); curr++)
				{
					//m_pack->m_vecPack[curr]->SetTarget(nullptr);
					SGD::Vector m_Distance = Player->GetPosition().ComputeDifference(m_pack->GetPosition());

					if (m_Distance.ComputeLength() < DISTANCE_CONTROL) 
					{
						float DegreeBetween = Player->CheckLoS(m_pack);
						if (DegreeBetween <= SGD::PI / 4)
						{
							CanTarget.push_back(m_pack);
							break;
						}

					}

				}
			}
#pragma endregion


#pragma region //Checking all packs
			for (size_t curr = 0; curr < EntityList->m_tEntities[Entity::ENT_PACK].size(); curr++)
			{
				PackComposite* pack = (PackComposite*)EntityList->m_tEntities[Entity::ENT_PACK][curr];
				for (size_t i = 0; i < pack->m_vecPack.size(); i++)
				{
					for (size_t i = 0; i < pack->m_vecPack.size(); i++)
					{
						Entity* m_animal = pack->m_vecPack[i];

						SGD::Vector m_Distance = Player->GetPosition().ComputeDifference(m_animal->GetPosition());
						if (m_Distance.ComputeLength() < DISTANCE_CONTROL)
						{
							float DegreeBetween = Player->CheckLoS(m_animal);
							//SGD::Vector MetoCursor = Cursor - Player->GetPosition();
							//SGD::Vector MetoEntity = m_animal->GetPosition() - Player->GetPosition();
							//MetoCursor.Normalize();
							//MetoEntity.Normalize();
							//float DegreeBetween = acos(MetoCursor.ComputeDotProduct(MetoEntity));
							if (DegreeBetween <= SGD::PI / 4)
							{
								CanTarget.push_back(m_animal);
								break;
							}
						}
					}
				}

#pragma endregion
			}
#pragma region //Gets closest entity to player sets him as analyzer's target

			float DistMod = DISTANCE_CONTROL;
			if (CanTarget.size())
			{
				for (unsigned int curr = 0; curr < CanTarget.size(); curr++)
				{
					SGD::Vector distance = Player->GetPosition() - CanTarget[curr]->GetPosition();

					if (distance.ComputeLength() < DistMod)
					{
						DistMod = distance.ComputeLength();
						Target = CanTarget[curr];
					}
				}

				//Makes sure the target hasnt been analyzed already
				if (AnalyzedList.size() > 0 && Target != nullptr)
				{
					for (unsigned int curr = 0; curr < AnalyzedList.size(); curr++)
					{
						//Creature already analyzed
						if (Target->GetCreature() == AnalyzedList[curr])
						{
							Ready = false;
							cooldown = 3.5f;
							//already in list
							m_bAlreadyScanned = true; 
							break;
						}
						else if (curr == AnalyzedList.size() - 1)
						{
							Ready = true;
							//m_bAlreadyScanned = true; 
						}
					}
				}
				else
				{
					Ready = true;
					m_bAlreadyScanned = false; 
				}
			}
#pragma endregion
		}
#pragma region finally starts countdown of analyze
		if (Target != nullptr && Ready == true && Target->GetType() == Entity::ENT_ANIMAL &&Target->GetInHiding() != true)
		{

			Entity* TempTarget = (Entity*)Target;
			float DegreeBetween = Player->CheckLoS(TempTarget);
			if (DegreeBetween <= SGD::PI / 4)
			{
				inprogress = true;
				Analysis += elapsedTime;
				BarRenderColor = (Analysis / 3.0f * 255);
				BarRenderLength = (Analysis / 3.0f * 300);
			}
			else
			{
				Ready = false;
				Analysis = 0;
				m_bScanfailed = true; 
				SGD::AudioManager::GetInstance()->PlayAudio(m_hFailure, false); 
			}

			TestFloatbloat += elapsedTime;
			if (TestFloatbloat >= 1 && inprogress)
			{
			//	DestroyEntityMessage* Test = new DestroyEntityMessage(TempTarget);
			//	Test->QueueMessage();
			}
			//Analysis successful
			if (Analysis >= 3)
			{
				m_bSuccess = true; 
				inprogress = false;
				Analysis = 0;
				Scanned++;
				SGD::AudioManager::GetInstance()->PlayAudio(victoria, false);
				AnalyzedList.push_back(Target->GetCreature());
				cooldown = 10.0f;
				CreatureLogMessage* msg = new CreatureLogMessage(Target);
				msg->SendMessageNow();
				delete msg;
				Target = nullptr;
				Ready = false;
			}
		}
		else
		{
			inprogress = false;
			Analysis = 0;
			//BarRenderColor = 0;
			//BarRenderLength = 0;
			Target = nullptr;
			Ready = false;
			m_bScanfailed = true; 
		}

#pragma endregion
	}
	else
	{
		Analysis = 0.0f;
		Ready = false;
	}
#pragma region Updating analyzer images

	if (true == m_bSuccess)
		m_fSuccessTimer -= elapsedTime; 

	if (m_fSuccessTimer <= 0)
	{
		m_fSuccessTimer = 2.0f; 
		m_bSuccess = false; 
	}

	if (true == inprogress && m_fScanTimer <= 0)
		m_bIsAlternating = !m_bIsAlternating;

	if (false == inprogress && m_fScanTimer <= 0)
		m_fScanTimer = 0.6f;

	if (true == inprogress)
		m_fScanTimer -= elapsedTime; 

	if (true == m_bScanfailed)
		m_fFailTimer -= elapsedTime; 

	if (m_fFailTimer <= 0)
	{
		m_bScanfailed = false; 
		m_fFailTimer = 1.0f; 
	}

	if (true == m_bAlreadyScanned)
		m_fAlreadyScannedTimer -= elapsedTime; 

	if (m_fAlreadyScannedTimer <= 0)
	{
		m_fAlreadyScannedTimer = 2.0f;
		m_bAlreadyScanned = false; 
	}

#pragma endregion

	//Scanned = AnalyzedList.size();
}

void Analyzer::Render()
{
	SGD::Size TempScreen = SGD::Size(Game::GetInstance()->GetScreenRes());
	SGD::Color Shift = SGD::Color((unsigned char)(255 - BarRenderColor), (unsigned char)(0 + BarRenderColor), 0);
	SGD::Rectangle Rect = SGD::Rectangle(SGD::Point(50, TempScreen.height - 50), SGD::Size(BarRenderLength + 1, 50.0f));

	//if (!Rect.IsEmpty())
	//	SGD::GraphicsManager::GetInstance()->DrawRectangle(Rect, Shift); 
if (inprogress)
	{
		SGD::Color Shift = SGD::Color((unsigned char)(255 - BarRenderColor), (unsigned char)(0 + BarRenderColor), 0);
		SGD::Rectangle Rect = SGD::Rectangle(SGD::Point(50, TempScreen.height - 50), SGD::Size(BarRenderLength + 1, 50.0f));
		SGD::GraphicsManager::GetInstance()->DrawRectangle(Rect, Shift);
	}
	else
	{
		SGD::Color Shift = SGD::Color(0, (unsigned char)(255 - BarRenderColor), (unsigned char)(0 + BarRenderColor), 0);
		SGD::Rectangle Rect = SGD::Rectangle(SGD::Point(50, TempScreen.height - 50), SGD::Size(BarRenderLength + 1, 50.0f));
		SGD::GraphicsManager::GetInstance()->DrawRectangle(Rect, Shift);
	}


#pragma region Cursor icons
	if (inprogress && !m_bIsAlternating)
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hCurrAnalyzing1, Cursor); 
	else if (inprogress && m_bIsAlternating)
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hCurrAnalyzing2, Cursor);

	if (true == m_bScanfailed)
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hScanFailed, Cursor); 

	if (true == m_bAlreadyScanned)
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hAnalyzerSuccessful, Cursor);
#pragma endregion
}
