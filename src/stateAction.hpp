#ifndef STATEACTION_HPP
#define STATEACTION_HPP

enum eStateAction {
	NO_CHANGE,
	REM_STATE,
	INTRO_STATE,
	MAIN_STATE
};

struct sMainStateSettings {
	int numPlayers_,
		cpuDiff_;
};

#endif
