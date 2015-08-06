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
	ballShape_ = new cCollAabb(1,1);

	paddle1_ = world_->createObject (cVector2(225,240),*paddleShape_,
			eObjType::DYNAMIC);
	paddle2_ = world_->createObject(cVector2(415,240),*paddleShape_,
			eObjType::DYNAMIC);
	ball_ = world_->createObject(cVector2(320,240),*ballShape_,
			eObjType::DYNAMIC);
	p1Goal_ = world_->createObject(cVector2(220,240),*vWallShape_);
	p2Goal_ = world_->createObject(cVector2(420,240),*vWallShape_);
	wall1_ = world_->createObject(cVector2(320,190),*hWallShape_);
	wall2_ = world_->createObject(cVector2(320,290),*hWallShape_);

	kbHandler_.addCommand(eKeyAction::ESCAPE,SDLK_ESCAPE);
	kbHandler_.addCommand(eKeyAction::M_DOWN,SDLK_DOWN);
	kbHandler_.addCommand(eKeyAction::M_UP,SDLK_UP);
}

cMainState::~cMainState (void) {
	delete paddleShape_;
	delete world_;
	delete broadphase_;
}

void cMainState::handleState (SDL_Event& event) {
	kbHandler_.checkCommand(event.key,&kbActionList_);
}

int cMainState::updateState (void) {
	for (auto& itr : kbActionList_) {
		if (itr == eKeyAction::M_DOWN)
			paddle1_->translate(0,1.5);
		if (itr == eKeyAction::M_UP)
			paddle1_->translate(0,-1.5);
		if (itr == eKeyAction::ESCAPE)
			return eStateAction::REM_STATE;
	}
	std::deque<cCollPair>* collPairList = world_->checkColls();
	for (auto itr : collPairList) {
		
	}
	return eStateAction::NO_CHANGE;
}

void cMainState::renderState (SDL_Renderer* renderer, double timelag) {
	world_->drawDebugWorld(renderer);
}
