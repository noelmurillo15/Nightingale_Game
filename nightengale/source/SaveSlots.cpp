#include "SaveSlots.h"

#include "tinyxml.h"
#include "TileEngine.h"
#include "Player.h"
#include "CharacterSelect.h"
#include "EntityManager.h"
#include "CreatureProfile.h"
#include "CreatureFactory.h"


SaveSlots::SaveSlots() {

}

SaveSlots::~SaveSlots() {

}

void SaveSlots::SaveGame(std::string Filename, Player* player, int gameTime, EntityManager* m_manager){
	std::string xml = "resource/XMLFiles/SaveFiles/" + Filename + ".xml";

	TiXmlDocument doc;

	TiXmlElement* data;

	TiXmlElement* root = new TiXmlElement(Filename.c_str());


	{	//	Save environment data
		data = new TiXmlElement("Environment");
		data->SetAttribute("World", TileEngine::GetInstance()->GetWorldNum());
		data->SetAttribute("AshLife", CharacterSelect::GetInstance()->AshLife);
		data->SetAttribute("BrockLife", CharacterSelect::GetInstance()->BrockLife);
		data->SetAttribute("JennyLife", CharacterSelect::GetInstance()->JennyLife);
		data->SetAttribute("ToddLife", CharacterSelect::GetInstance()->ToddLife);
		data->SetAttribute("ElapsedTime", gameTime);
		root->LinkEndChild(data);
	}

	{	//	Save Player data
		data = new TiXmlElement("Player");
		data->SetAttribute("Name", player->GetName().c_str());
		data->SetDoubleAttribute("PositionX", (int)player->GetPosition().x);
		data->SetDoubleAttribute("PositionY", (int)player->GetPosition().y);
		data->SetAttribute("Health", player->GetHealth());
		data->SetAttribute("Repellents", player->GetNumRepellents());
		data->SetAttribute("Baits", player->GetNumBaits());
		data->SetAttribute("Saves", player->GetNumSaves());


		root->LinkEndChild(data);
	}

	{	//	Animal data
		data = new TiXmlElement("Animal");
		std::vector<IEntity*> Anavec = m_manager->m_tEntities[Entity::ENT_ANIMAL];
		data->SetAttribute("CreatureCount", Anavec.size());
		for (size_t i = 0; i < Anavec.size(); i++)
		{
			Entity* temp = (Entity*)Anavec[i];
			TiXmlElement* duta = new TiXmlElement("Creature");
			duta->SetAttribute("Type", temp->GetCreature());
			duta->SetAttribute("PositionX", (int)temp->GetPosition().x);
			duta->SetAttribute("PositionY", (int)temp->GetPosition().y);
			duta->SetAttribute("ProfileCount", temp->GetDataWareHouse()->CreatureIndex.size());
			auto iter = temp->GetDataWareHouse()->CreatureIndex.begin();
			while (iter != temp->GetDataWareHouse()->CreatureIndex.end())
			{
				TiXmlElement* dota = new TiXmlElement("Profile");
				dota->SetAttribute("Name", iter->first.c_str());
				dota->SetAttribute("DangerLevel", iter->second->DangerLevel);
				dota->SetAttribute("FirstEnounterState", iter->second->FirstEnounterState);
				dota->SetAttribute("isPredator", iter->second->isPredator);
				dota->SetAttribute("isPrey", iter->second->isPrey);
				dota->SetAttribute("PreviousEncounterState", iter->second->PreviousEncounterState);
				duta->LinkEndChild(dota);
				iter++;
			}
			data->LinkEndChild(duta);
		}
	}
	root->LinkEndChild(data);
	doc.LinkEndChild(root);
	doc.SaveFile(xml.c_str());
}

bool SaveSlots::LoadGame(std::string Filename, CreatureFactory* m_factory, EntityManager* m_manager){
	std::string xml = "resource/XMLFiles/SaveFiles/" + Filename;

	TiXmlDocument doc(xml.c_str());

	if (!doc.LoadFile())
		return false;


	TiXmlHandle hDoc(&doc);

	TiXmlElement* Root = hDoc.ChildElement(0).Element();

	TiXmlElement* Environment = Root->FirstChildElement();

	TiXmlElement* Playaw = Environment->NextSiblingElement();

	TiXmlElement* Creature = Playaw->NextSiblingElement();

	{	//	Load Environment
		int Map, gameTime;
		Environment->QueryIntAttribute("World", &Map);
		Environment->QueryBoolAttribute("AshLife", &CharacterSelect::GetInstance()->AshLife);
		Environment->QueryBoolAttribute("BrockLife", &CharacterSelect::GetInstance()->BrockLife);
		Environment->QueryBoolAttribute("JennyLife", &CharacterSelect::GetInstance()->JennyLife);
		Environment->QueryBoolAttribute("ToddLife", &CharacterSelect::GetInstance()->ToddLife);
		Environment->QueryIntAttribute("ElapsedTime", &gameTime);
		TileEngine::GetInstance()->LoadMap(Map);		
	}

	{	//	Load Player

		Entity* player;

		int posY;
		int posX;
		int Repel;
		int Baits;
		int Health;
		std::string name = Playaw->Attribute("Name");


		if (name == "Ash")
			player = m_factory->CreatePlayer(0);
		else if (name == "Brock")
			player = m_factory->CreatePlayer(1);
		else if (name == "Jenny")
			player = m_factory->CreatePlayer(2);
		else
			player = m_factory->CreatePlayer(3);


		Playaw->QueryIntAttribute("PositionX", &posX);
		Playaw->QueryIntAttribute("PositionY", &posY);
		Playaw->QueryIntAttribute("Repellents", &Repel);
		Playaw->QueryIntAttribute("Baits", &Baits);
		Playaw->QueryIntAttribute("Health", &Health);

		Player* P = (Player*)player;

		player->SetInitialPosition(SGD::Point{ (float)posX, (float)posY });
		P->SetNumRepellents(Repel);
		P->SetNumBaits(Baits);
		P->SetHealth(Health);

		m_manager->AddEntity(player, player->GetType());	

		Camera::GetInstance()->SetCamFocus(player);
	}

	{	//	Animal Data
		int count;
		Creature->QueryIntAttribute("CreatureCount", &count);

		TiXmlElement* data = Creature->FirstChildElement();


		for (int i = 0; i < count; i++) {

			Entity* Temp = nullptr;
			int Type; 
			int PCount;
			double posX;
			double posY;

			data->QueryIntAttribute("Type", &Type);
			data->QueryDoubleAttribute("PositionX", &posX);
			data->QueryDoubleAttribute("PositionY", &posY);			
			data->QueryIntAttribute("ProfileCount", &PCount);			

			switch (Type){

			case Entity::CRT_BISHOP:
				Temp = m_factory->CreateBishop();
				break;

			case Entity::CRT_BUMBLES:
				Temp = m_factory->CreateBumbles();
				break;

			case Entity::CRT_CASTLE:
				Temp = m_factory->CreateCastle();
				break;

			case Entity::CRT_KING:
				Temp = m_factory->CreateKing();
				break;

			case Entity::CRT_KNIGHT:
				Temp = m_factory->CreateKnight();
				break;

			case Entity::CRT_PASSANT:
				Temp = m_factory->CreatePassant();
				break;

			case Entity::CRT_PAWN:
				Temp = m_factory->CreatePawn();
				break;

			case Entity::CRT_PRANCER:
				Temp = m_factory->CreatePrancer();
				break;

			case Entity::CRT_QUEEN:
				Temp = m_factory->CreateQueen();
				break;

			case Entity::CRT_ROOK:
				Temp = m_factory->CreateRook();
				break;


			default:
				break;
			}

				Temp->SetPosition(SGD::Point{ (float)posX, (float)posY });

			TiXmlElement* duta = data->FirstChildElement();
			for (int i = 0; i < PCount; i++) {
				CreatureProfile* Yo = new CreatureProfile;
				duta->QueryIntAttribute("DangerLevel", &Yo->DangerLevel);
				duta->QueryIntAttribute("FirstEnounterState", &Yo->FirstEnounterState);
				duta->QueryBoolAttribute("isPredator", &Yo->isPredator);
				duta->QueryBoolAttribute("isPrey", &Yo->isPrey);
				duta->QueryIntAttribute("PreviousEncounterState", &Yo->PreviousEncounterState);
				Temp->GetDataWareHouse()->AddNewIndex(duta->Attribute("Name"), Yo);
			}
			m_manager->AddEntity(Temp, Temp->GetType());
			data = data->NextSiblingElement();
		}
	}
	
	Camera::GetInstance()->ResetCamera();
	return true;
}

void SaveSlots::LoadGameInfo(std::string Filename, LoadMenuState::SaveSlotInfo* saveinfo){
	std::string xml = "resource/XMLFiles/SaveFiles/" + Filename;

	TiXmlDocument doc(xml.c_str());

	if (!doc.LoadFile())
		return;

	TiXmlHandle hDoc(&doc);
	TiXmlElement* Root = hDoc.ChildElement(0).Element();
	TiXmlElement* Environment = Root->FirstChildElement();
	TiXmlElement* Playaw = Environment->NextSiblingElement();

	int Map = 0, gameTime = 0;
	Environment->QueryIntAttribute("World", &Map);
	Environment->QueryIntAttribute("ElapsedTime", &gameTime);

	saveinfo->charName = Playaw->Attribute("Name");
	saveinfo->elapsedTime = gameTime;
	saveinfo->Worldnum = Map;
}