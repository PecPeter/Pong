#ifndef INTROSTATE_HPP
#define INTROSTATE_HPP

#include <iostream>
#include <string>
#include <vector>

#include "gameState.hpp"
#include "cntrlKb.hpp"

#include "stateAction.hpp"

class cIntroState : public cGameState {
	public:
		cIntroState (void);
		~cIntroState (void);
	private:
		void handleState (SDL_Event& event);
		int updateState (double tickRate, void* interStateInfo);
		void renderState (SDL_Renderer* renderer, double timeLag);

		std::string menuList[3];
		std::string cpuDiffList[3];
		int menuSelector,
			cpuDiffSelector;

		enum class eKeyAction {
			M_UP,
			M_DOWN,
			M_LEFT,
			M_RIGHT,
			ENTER
		};
		std::vector<eKeyAction> kbActionList_;
		cCntrlKb<eKeyAction> kbHandler_;
};

#endif
