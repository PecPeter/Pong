#include "gameStateHandler.hpp"

cGameStateHandler::cGameStateHandler (void): state_(nullptr) {
	state_ = new cMainState();
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
		case eStateAction::REM_STATE:
			delete state_;
			state_ = nullptr;
			numStates_ = 0;
			break;
		default: break;
	}
}
