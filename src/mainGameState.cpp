#include "mainGameState.hpp"

cMainState::cMainState (const sMainStateSettings& stateSettings):
	cGameState(eStateAction::NO_CHANGE,eStateAction::REM_STATE),
	broadphase_(nullptr), world_(nullptr), aiFunc_(nullptr) {
	broadphase_ = new cGenBroadphase();
	world_ = new cCollWorld(broadphase_);
	world_->setDebugDraw(&debugDraw_);

	paddleShape_ = new cCollAabb(2.0,10);
	hWallShape_ = new cCollAabb(100,2);
	vWallShape_ = new cCollAabb(2,50);
	ballShape_ = new cCollCircle(2);

	paddle1_ = world_->createObject(cVector2(230,240),*paddleShape_,
			eObjType::DYNAMIC);
	paddle2_ = world_->createObject(cVector2(410,240),*paddleShape_,
			eObjType::DYNAMIC);
	paddle2_->rotate(3.141592653589793238462643383279502884197169399375105820974944592);
	ball_ = world_->createObject(cVector2(0,0),*ballShape_,
			eObjType::DYNAMIC);
	ball_->translate(cVector2(320,240));
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
	ballInfo->dir_ = cVector2(-1,1);
	ballInfo->speed_ = 100;

	paddle1_->setUsrPtr(static_cast<void*>(paddleInfo));
	paddle2_->setUsrPtr(static_cast<void*>(paddleInfo));
	ball_->setUsrPtr(static_cast<void*>(ballInfo));
	p1Goal_->setUsrPtr(static_cast<void*>(goalInfo1));
	p2Goal_->setUsrPtr(static_cast<void*>(goalInfo2));
	wall1_->setUsrPtr(static_cast<void*>(wallInfo));
	wall2_->setUsrPtr(static_cast<void*>(wallInfo));

	ball_->setCollCallback(ballCollCallback);

	if (stateSettings.numPlayers_ == 1) {
		switch (stateSettings.cpuDiff_) {
			case 0:
				aiFunc_ = updateAiEasy;
				break;
			case 1:
				aiFunc_ = updateAiMed;
				break;
			case 2:
				aiFunc_ = updateAiHard;
				break;
			default:
				break;
		}
	}

	kbHandler_.addCommand(eKeyAction::ESCAPE,SDLK_ESCAPE);
	kbHandler_.addCommand(eKeyAction::P1_DOWN,SDLK_s);
	kbHandler_.addCommand(eKeyAction::P1_UP,SDLK_w);

	if (stateSettings.numPlayers_ == 2) {
		kbHandler_.addCommand(eKeyAction::P2_DOWN,SDLK_DOWN);
		kbHandler_.addCommand(eKeyAction::P2_UP,SDLK_UP);
	}
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

int cMainState::updateState (double tickRate, void* interStateInfo) {
	//Update paddle pos
	double tickRateMultiple = 1.0/tickRate;
	double paddleSpeed = 200;
	for (auto& itr : kbActionList_) {
		if (itr == eKeyAction::P1_DOWN) {
			paddle1_->translate(0,tickRateMultiple*paddleSpeed);
		}
		else if (itr == eKeyAction::P1_UP) {
			paddle1_->translate(0,tickRateMultiple*-paddleSpeed);
		}
		else if (itr == eKeyAction::P2_DOWN) {
			paddle2_->translate(0,tickRateMultiple*paddleSpeed);
		}
		else if (itr == eKeyAction::P2_UP) {
			paddle2_->translate(0,tickRateMultiple*-paddleSpeed);
		}
		else if (itr == eKeyAction::ESCAPE)
			return eStateAction::REM_STATE;
	}
	if (aiFunc_ != nullptr) {
		switch (aiFunc_(*ball_,*paddle2_)) {
			case ePaddleMovement::P_UP:
				paddle2_->translate(0,tickRateMultiple*-paddleSpeed);
				break;
			case ePaddleMovement::P_DOWN:
				paddle2_->translate(0,tickRateMultiple*paddleSpeed);
				break;
			case ePaddleMovement::P_NONE:
				break;
			default:
				break;
		}
	}
	//Update ball pos
	sBallInfo* ballInfo = static_cast<sBallInfo*>(ball_->getUsrPtr());
	double vx = ballInfo->dir_.getX()*ballInfo->speed_,
		   vy = ballInfo->dir_.getY()*ballInfo->speed_;
	ball_->translate((1.0/tickRate)*vx,(1.0/tickRate)*vy);

	std::forward_list<cCollPair>* collPairList = world_->checkColls();
	bool roundReset = false;
	while (collPairList->empty() == false) {
		const cCollPair* collPair = &collPairList->front();
		int obj1ID = static_cast<sObjInfo*>(collPair->obj1()->getUsrPtr())->objID_,
			obj2ID = static_cast<sObjInfo*>(collPair->obj2()->getUsrPtr())->objID_;
		// Check if there is a collision between a goal and the ball
		if (obj1ID == eObjID::GOAL) {
			static_cast<sGoalInfo*>(collPair->obj1()->getUsrPtr())->points_ += 1;
			roundReset = true;
		}
		else if (obj2ID == eObjID::GOAL) {
			static_cast<sGoalInfo*>(collPair->obj2()->getUsrPtr())->points_ += 1;
			roundReset = true;
		}
		// Check if there is a collision with the ball and any other object
		if (obj1ID == eObjID::BALL)
			collPair->obj1()->collCallback(collPair->obj1(),collPair->obj2(),collPair->getObjOverlap());
		else if (obj2ID == eObjID::BALL)
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
//	std::cout << "\nP1: " << 
//		static_cast<sGoalInfo*>(p2Goal_->getUsrPtr())->points_ <<
//		"\tP2: " << static_cast<sGoalInfo*>(p1Goal_->getUsrPtr())->points_;
	if (roundReset == true) {
		ball_->translate(cVector2(320,ball_->getObjPos().getY())-ball_->getObjPos());
		ballInfo->speed_ = 80;
	}
	return eStateAction::NO_CHANGE;
}

void cMainState::renderState (SDL_Renderer* renderer, double timelag) {
	world_->drawDebugWorld(renderer);
}
