#include "introGameState.hpp"

cIntroState::cIntroState (void): cGameState(eStateAction::NO_CHANGE,eStateAction::REM_STATE) {
	menuList[0] = std::string("ONE PLAYER");
	menuList[1] = std::string("TWO PLAYER");
	menuList[2] = std::string("EXIT");
	menuSelector = 0;

	cpuDiffList[0] = std::string("EASY");
	cpuDiffList[1] = std::string("MEDIUM");
	cpuDiffList[2] = std::string("HARD");
	cpuDiffSelector = 0;

	kbHandler_.addCommand(eKeyAction::M_UP,SDLK_UP,false);
	kbHandler_.addCommand(eKeyAction::M_DOWN,SDLK_DOWN,false);
	kbHandler_.addCommand(eKeyAction::M_LEFT,SDLK_LEFT,false);
	kbHandler_.addCommand(eKeyAction::M_RIGHT,SDLK_RIGHT,false);
	kbHandler_.addCommand(eKeyAction::ENTER,SDLK_RETURN);
}

cIntroState::~cIntroState (void) {

}

void cIntroState::handleState (SDL_Event& event) {
	switch (event.type) {
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			kbHandler_.checkCommand(event.key,&kbActionList_);
			break;
		case SDL_MOUSEMOTION:
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			break;
		default:
			break;
	}
}

int cIntroState::updateState (double tickRate, void* interStateInfo) {
	for (auto& itr : kbActionList_) {
		if (itr == eKeyAction::M_UP) {
			if (menuSelector != 0)
				menuSelector--;
		}
		else if (itr == eKeyAction::M_DOWN) {
			if (menuSelector != 2)
				menuSelector++;
		}
		else if (itr == eKeyAction::M_LEFT) {
			if (menuSelector == 0) {
				if (cpuDiffSelector != 0)
					cpuDiffSelector--;
			}
		}
		else if (itr == eKeyAction::M_RIGHT) {
			if (menuSelector == 0) {
				if (cpuDiffSelector != 2)
					cpuDiffSelector++;
			}
		}
		else if (itr == eKeyAction::ENTER) {
			if (menuSelector == 2)
				return eStateAction::REM_STATE;
			sMainStateSettings* settings = new sMainStateSettings;
			switch (menuSelector) {
				case 0:
					settings->numPlayers_ = 2;
					settings->cpuDiff_ = 0;
					break;
				case 1:
					settings->numPlayers_ = 1;
					switch (cpuDiffSelector) {
						case 0:
							settings->cpuDiff_ = 0;
							break;
						case 1:
							settings->cpuDiff_ = 1;
							break;
						case 2:
							settings->cpuDiff_ = 2;
							break;
						default: break;
					}
					break;
				default: break;
			}
			interStateInfo = static_cast<void*>(settings);
			return eStateAction::MAIN_STATE;
		}
	}
	return eStateAction::NO_CHANGE;
}

void cIntroState::renderState (SDL_Renderer* renderer, double timeLag) {
	std::cout << "Selected Option: " << menuList[menuSelector];
	if (menuSelector == 0) {
		std::cout << "\nSelected CPU Difficulty: " << cpuDiffList[cpuDiffSelector];
	}
	std::cout << "\n\n";
}
