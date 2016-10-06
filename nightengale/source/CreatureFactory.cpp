#include "CreatureFactory.h"

#include "Player.h"
#include "Pawn.h"
#include "Passant.h"
#include "Bumbles.h"
#include "King.h"
#include "Rook.h"
#include "Prancer.h"
#include "Queen.h"
#include "Castle.h"
#include "Bishop.h"

#include "Knight.h"
#include "PackComposite.h"
#include "StaticObject.h"

CreatureFactory::CreatureFactory() {

	
	
}

CreatureFactory::~CreatureFactory() {

}

Entity* CreatureFactory::CreatePlayer(int _character) {

	Player* tmpPlayer = new Player();
	

	switch (_character) {

	case 0:
		tmpPlayer->SetName("Ash");			
		break; 

	case 1:
		tmpPlayer->SetName("Brock");	
		break;

	case 2:
		tmpPlayer->SetName("Jenny");
		break;

	case 3:
		tmpPlayer->SetName("Todd");
		break;

	default:
		tmpPlayer->SetName("Player");
		break;
	}	

	if (!tmpPlayer->GetTimestamp()) {
		AnimationSystem* AnimSys = AnimationSystem::GetInstance();
		if (tmpPlayer->GetName() != "Todd" && tmpPlayer->GetName() != "Ash"){
			tmpPlayer->LoadData({ 0, 0 }, { 0, 0 }, { 0, 0 }, Camera::GetInstance()->GetMapLayer()->FindSpawnPoint("Player"));
			AnimSys->Load(tmpPlayer->GetName(), tmpPlayer->GetName());
		}
		else{
			tmpPlayer->LoadData({ 0, 0 }, { 0, 0 }, { 0, 0 }, Camera::GetInstance()->GetMapLayer()->FindSpawnPoint("Player"));
			AnimSys->Load("Player", tmpPlayer->GetName());
		}
	}
	tmpPlayer->LoadDialog();
	Camera::GetInstance()->SetCamFocus(tmpPlayer);
	return tmpPlayer;
}

Entity* CreatureFactory::CreatePawn() {

	Entity* tmpPawn = new Pawn();
	return tmpPawn;
}

Entity* CreatureFactory::CreatePrancer() {
	Entity* tmpPrancer = new Prancer();
	return tmpPrancer;
}

Entity* CreatureFactory::CreateRook() {
	Entity* tmpRook = new Rook();
	return tmpRook;
}

Entity* CreatureFactory::CreateKing() {
	Entity* BOSS = new King();
	return BOSS;
}

Entity* CreatureFactory::CreatePassant() {
	Entity* tmpPassant = new Passant();
	return tmpPassant;
}

Entity* CreatureFactory::CreateBumbles() {
	Entity* tmpBumbles = new Bumbles();
	return tmpBumbles;
}

Entity* CreatureFactory::CreateQueen() {
	Entity* RoyalBitch = new Queen();
	return RoyalBitch;
}

Entity* CreatureFactory::CreateCastle() {
	Entity* Plant = new Castle(); 
	return Plant; 
}

Entity* CreatureFactory::CreateBishop() {

	Entity* Snake = new Bishop(); 
	return Snake; 
}
Entity*CreatureFactory::CreateKnight()
{

		Entity* wolf = new Knight();
		return wolf;
}

Entity*CreatureFactory::CreatePack()
{
	Entity* Pack = new PackComposite();
	return Pack;
}



StaticObject* CreatureFactory::CreateBrush(SGD::Point _SpawnPoint) {
	StaticObject* Bush = new StaticObject(_SpawnPoint);
	Bush->SetRect(SGD::Rectangle(_SpawnPoint, SGD::Size{ 32, 32 }));
	return Bush;
}

	StaticObject* CreatureFactory::CreateBTrans(SGD::Point _SpawnPoint) {
		StaticObject* btrans = new StaticObject(_SpawnPoint);
		btrans->SetRect(SGD::Rectangle(_SpawnPoint, SGD::Size{ 32, 32 }));
		return btrans;
}