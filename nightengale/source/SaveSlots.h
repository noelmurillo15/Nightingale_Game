#pragma once

#include <string>
#include "LoadMenuState.h"

class Player;
class EntityManager;
class CreatureFactory;


class SaveSlots {


public:

	SaveSlots();
	~SaveSlots();

	void SaveGame(std::string Filename, Player* player, int gameTime, EntityManager*);
	bool LoadGame(std::string Filename, CreatureFactory*, EntityManager*);

	void LoadGameInfo(std::string Filename, LoadMenuState::SaveSlotInfo*);
};