#ifndef MAINGAMESTATE_HPP
#define MAINGAMESTATE_HPP

#include <iostream>
#include <forward_list>
#include <vector>

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

#include "ai.hpp"
#include "collCallback.hpp"
#include "objInfo.hpp"
#include "stateAction.hpp"

typedef ePaddleMovement(*aiFuncContr) (const cCollObj& ball, const cCollObj& controlledPaddle);

class cMainState : public cGameState {
	public:
		cMainState (const sMainStateSettings& stateSettings);
		~cMainState (void);
	private:
		void handleState (SDL_Event& event);
		int updateState (double tickRate, void* interStateInfo);
		void renderState (SDL_Renderer* renderer, double timelag);

		cCollBroadphase* broadphase_;
		cCollWorld* world_;
		cCollDebugDrawer debugDraw_;

		cCollAabb* paddleShape_,
			*hWallShape_,
			*vWallShape_;
		cCollCircle* ballShape_;

		cCollObj* paddle1_,
			*paddle2_,
			*ball_,
			*p1Goal_,
			*p2Goal_,
			*wall1_,
			*wall2_;

		aiFuncContr aiFunc_; 

		enum class eKeyAction {
			ESCAPE,
			P1_DOWN,
			P2_DOWN,
			P1_UP,
			P2_UP
		};
		std::vector<eKeyAction> kbActionList_;
		cCntrlKb<eKeyAction> kbHandler_;
};

#endif
