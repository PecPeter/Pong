#include "mainGameState.hpp"

cMainState::cMainState (void):
	cGameState(eStateAction::NO_CHANGE,eStateAction::REM_STATE),
	broadphase_(nullptr), world_(nullptr) {
	broadphase_ = new cGenBroadphase();
	world_ = new cCollWorld(broadphase_);
	world_->setDebugDraw(&debugDraw_);

	paddleShape_ = new cCollAabb(0.5,5);
	hWallShape_ = new cCollAabb(100,1);
	vWallShape_ = new cCollAabb(1,50);
	ballShape_ = new cCollCircle(2);

	paddle1_ = world_->createObject(cVector2(225,240),*paddleShape_,
			eObjType::DYNAMIC);
	paddle2_ = world_->createObject(cVector2(415,240),*paddleShape_,
			eObjType::DYNAMIC);
	ball_ = world_->createObject(cVector2(320,240),*ballShape_,
			eObjType::DYNAMIC);
	p1Goal_ = world_->createObject(cVector2(219,240),*vWallShape_);
	p2Goal_ = world_->createObject(cVector2(421,240),*vWallShape_);
	wall1_ = world_->createObject(cVector2(320,189),*hWallShape_);
	wall2_ = world_->createObject(cVector2(320,291),*hWallShape_);

	kbHandler_.addCommand(eKeyAction::ESCAPE,SDLK_ESCAPE);
	kbHandler_.addCommand(eKeyAction::M_DOWN,SDLK_DOWN);
	kbHandler_.addCommand(eKeyAction::M_UP,SDLK_UP);

	ballVx_ = 50.0;
	ballVy_ = -20.0;
}

cMainState::~cMainState (void) {
	delete paddleShape_;
	delete hWallShape_;
	delete vWallShape_;
	delete ballShape_;
	delete world_;
	delete broadphase_;
}

void cMainState::handleState (SDL_Event& event) {
	kbHandler_.checkCommand(event.key,&kbActionList_);
}

int cMainState::updateState (double tickRate) {
	//Update paddle1 pos
	for (auto& itr : kbActionList_) {
		if (itr == eKeyAction::M_DOWN)
			paddle1_->translate(0,1);
		else if (itr == eKeyAction::M_UP)
			paddle1_->translate(0,-1);
		else if (itr == eKeyAction::ESCAPE)
			return eStateAction::REM_STATE;
	}
	//Update ball pos
	ball_->translate((1.0/tickRate)*ballVx_,(1.0/tickRate)*ballVy_);

	std::forward_list<cCollPair>* collPairList = world_->checkColls();
	while (collPairList->empty() == false) {
		cCollPair* collPair = &collPairList->front();
		if (collPair->obj2()->getObjType() == eObjType::DYNAMIC) {

		}
		else if (collPair->obj2()->getObjType() == eObjType::STATIC) {
			collPair->obj1()->translate(collPair->getObjOverlap());
		}
		collPairList->pop_front();
	}
	return eStateAction::NO_CHANGE;
}

void cMainState::renderState (SDL_Renderer* renderer, double timelag) {
	world_->drawDebugWorld(renderer);
}
