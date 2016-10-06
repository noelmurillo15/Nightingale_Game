#pragma once


class CreatureProfile {

public:

	CreatureProfile();
	~CreatureProfile();

	int FirstEnounterState;
	int PreviousEncounterState;
	int DangerLevel;
	bool isPrey = false;
	bool isPredator = false;
};