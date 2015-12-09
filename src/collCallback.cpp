#include "collCallback.hpp"

void ballCollCallback (cCollObj* obj1, cCollObj* obj2, cVector2 collVector) {
	//Determine what the ball is colliding with
	int obj2ID = static_cast<sObjInfo*>(obj2->getUsrPtr())->objID_;
	//Ball colliding with paddle
	if (obj2ID == eObjID::PADDLE) {
		sBallInfo* ballInfo = static_cast<sBallInfo*>(obj1->getUsrPtr());
		ballInfo->dir_ = vUnitVector(obj1->getObjPos()-obj2->getObjPos());
		ballInfo->speed_ += 15;
	}
	else if (obj2ID == eObjID::WALL) {
		cVector2 norm = vUnitVector(collVector);
		sBallInfo* ballInfo = static_cast<sBallInfo*>(obj1->getUsrPtr());
		cVector2 invVel = vUnitVector(ballInfo->dir_)*cVector2(-1,-1);
		ballInfo->dir_ = vMirror(ballInfo->dir_,norm)*-1;
	}
	obj1->translate(collVector);
}
