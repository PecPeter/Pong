#ifndef MAINGAMESTATE_HPP
#define MAINGAMESTATE_HPP

#include <deque>
#include <vector>
#include <exception>

#include <SDL2/SDL.h>

#include "collBroadphase.hpp"
#include "collPair.hpp"
#include "collShape.hpp"
#include "collObject.hpp"
#include "collWorld.hpp"
#include "gameState.hpp"
#include "mathVector.hpp"
#include "cntrlKb.hpp"
#include "collDebugDrawer.hpp"

#include "stateAction.hpp"

class cMainState : public cGameState {
	public:
		cMainState (void);
		~cMainState (void);
	private:
		void handleState (SDL_Event& event);
		int updateState (void);
		void renderState (SDL_Renderer* renderer, double timelag);

		cCollBroadphase* broadphase_;
		cCollWorld* world_;
		cCollDebugDrawer debugDraw_;

		cCollAabb* paddleShape_,
			*hWallShape_,
			*vWallShape_,
			*ballShape_;

		cCollObj* paddle1_,
			*paddle2_,
			*ball_,
			*p1Goal_,
			*p2Goal_,
			*wall1_,
			*wall2_;

		enum class eKeyAction {
			ESCAPE,
			M_DOWN,
			M_UP
		};
		std::vector<eKeyAction> kbActionList_;
		cCntrlKb<eKeyAction> kbHandler_;
};

#endif
