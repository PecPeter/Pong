#ifndef GAMESTATEHANDLER_HPP
#define GAMESTATEHANDLER_HPP

#include "gameState.hpp"
#include "stateHandler.hpp"

#include "stateAction.hpp"
#include "mainGameState.hpp"

class cGameStateHandler : public cStateHandler {
	public:
		cGameStateHandler (void);
		~cGameStateHandler (void);
		cGameState* getState (void);
		void changeState (int stateAction);
	private:
		cGameState* state_;
};

#endif
