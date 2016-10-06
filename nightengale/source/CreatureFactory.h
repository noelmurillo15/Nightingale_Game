#pragma once

#include "../SGD Wrappers/SGD_Geometry.h"

#include <string>

class StaticObject;
class Entity;

class CreatureFactory {

public:
	CreatureFactory();
	~CreatureFactory();

	Entity* CreatePlayer(int _character);

	Entity* CreatePawn();
	Entity* CreatePassant();
	Entity* CreateKing();
	Entity* CreateRook();
	Entity* CreateBumbles();
	Entity* CreatePrancer();
	Entity* CreateQueen();
	Entity* CreateCastle();
	Entity* CreateBishop();
	Entity* CreateKnight();
	Entity* CreatePack();

	StaticObject* CreateBrush(SGD::Point spawn);
	StaticObject* CreateBTrans(SGD::Point _SpawnPoint);
};