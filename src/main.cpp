#include "engine.hpp"
#include "gameStateHandler.hpp"

int main (void) {
	cEngine gameEngine;
	cGameStateHandler stateHandler;
	gameEngine.init(640,480,"SPACE GAME V3",&stateHandler);
	gameEngine.mainLoop();
	gameEngine.quit();
	return 0;
}
