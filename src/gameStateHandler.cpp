#include "gameStateHandler.hpp"

cGameStateHandler::cGameStateHandler (void): state_(nullptr) {
	state_ = new cIntroState();
	numStates_ = 1;
}

cGameStateHandler::~cGameStateHandler (void) {
	if (state_ != nullptr)
		delete state_;
	state_ = nullptr;
}

cGameState* cGameStateHandler::getState (void) {
	return state_;
}

void cGameStateHandler::changeState (int stateAction) {
	switch (stateAction) {
		case eStateAction::INTRO_STATE:
			break;
		case eStateAction::MAIN_STATE: {
			delete state_;
			sMainStateSettings* stateSettings = static_cast<sMainStateSettings*>(interStateInfo_);
			state_ = new cMainState(*stateSettings);
			delete stateSettings;
			break;
		}
		case eStateAction::REM_STATE:
			delete state_;
			state_ = nullptr;
			numStates_ = 0;
			break;
		default: break;
	}
}
