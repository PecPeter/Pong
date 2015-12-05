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
	ball_ = world_->createObject(cVector2(0,0),*ballShape_,
			eObjType::DYNAMIC);
	p1Goal_ = world_->createObject(cVector2(219,240),*vWallShape_);
	p2Goal_ = world_->createObject(cVector2(421,240),*vWallShape_);
	wall1_ = world_->createObject(cVector2(320,189),*hWallShape_);
	wall2_ = world_->createObject(cVector2(320,291),*hWallShape_);

	sObjInfo* paddleInfo = new sObjInfo,
		* wallInfo = new sObjInfo;
	sGoalInfo* goalInfo1 = new sGoalInfo,
		* goalInfo2 = new sGoalInfo;
	sBallInfo* ballInfo = new sBallInfo;
	paddleInfo->objID_ = eObjID::PADDLE;
	wallInfo->objID_ = eObjID::WALL;
	goalInfo1->objID_ = goalInfo2->objID_ = eObjID::GOAL;
	goalInfo1->points_ = goalInfo2->points_ = 0;
	ballInfo->objID_ = eObjID::BALL;
	ballInfo->vel_ = cVector2(-50.0,20.0);
	resetBall(true,ball_);

	paddle1_->setUsrPtr(static_cast<void*>(paddleInfo));
	paddle2_->setUsrPtr(static_cast<void*>(paddleInfo));
	ball_->setUsrPtr(static_cast<void*>(ballInfo));
	p1Goal_->setUsrPtr(static_cast<void*>(goalInfo1));
	p2Goal_->setUsrPtr(static_cast<void*>(goalInfo2));
	wall1_->setUsrPtr(static_cast<void*>(wallInfo));
	wall2_->setUsrPtr(static_cast<void*>(wallInfo));

	ball_->setCollCallback(ballCollCallback);

	kbHandler_.addCommand(eKeyAction::ESCAPE,SDLK_ESCAPE);
	kbHandler_.addCommand(eKeyAction::M_DOWN,SDLK_DOWN);
	kbHandler_.addCommand(eKeyAction::M_UP,SDLK_UP);
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
	sBallInfo* ballInfo = static_cast<sBallInfo*>(ball_->getUsrPtr());
	ball_->translate((1.0/tickRate)*ballInfo->vel_.getX(),(1.0/tickRate)*ballInfo->vel_.getY());

	std::forward_list<cCollPair>* collPairList = world_->checkColls();
	while (collPairList->empty() == false) {
		const cCollPair* collPair = &collPairList->front();
		if (static_cast<sObjInfo*>(collPair->obj1()->getUsrPtr())->objID_ == eObjID::BALL)
			collPair->obj1()->collCallback(collPair->obj1(),collPair->obj2(),collPair->getObjOverlap());
		if (static_cast<sObjInfo*>(collPair->obj2()->getUsrPtr())->objID_ == eObjID::BALL)
			collPair->obj2()->collCallback(collPair->obj2(),collPair->obj1(),collPair->getObjOverlap()*-1);
		else {
			if (collPair->obj2()->getObjType() == eObjType::DYNAMIC) {

			}
			else if (collPair->obj2()->getObjType() == eObjType::STATIC) {
				collPair->obj1()->translate(collPair->getObjOverlap());
			}
		}
		collPairList->pop_front();
	}
	return eStateAction::NO_CHANGE;
}

void cMainState::renderState (SDL_Renderer* renderer, double timelag) {
	world_->drawDebugWorld(renderer);
}

void resetBall (bool direction, cCollObj* ball) {
	// if direction == true, go left
	// else go right
	ball->translate(cVector2(320,240)-ball->getObjPos());
	sBallInfo* ballInfo = static_cast<sBallInfo*>(ball->getUsrPtr());
	if (direction == true)
		ballInfo->vel_ = cVector2(-50.0,20.0);
	else
		ballInfo->vel_ = cVector2(50.0,20.0);
}
