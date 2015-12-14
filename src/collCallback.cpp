#include "collCallback.hpp"

void ballCollCallback (cCollObj* obj1, cCollObj* obj2, cVector2 collVector) {
	int obj2ID = static_cast<sObjInfo*>(obj2->getUsrPtr())->objID_;
	if (obj2ID == eObjID::PADDLE) {
		// Use the direction of the ball to determine whether the centroid
		// offset needs to be added or subtracted in the calculations
		// An offset of 7.5 (when paddle hw=0.5 and ball r=2) will
		// give a total range of 90 degrees
		sBallInfo* ballInfo = static_cast<sBallInfo*>(obj1->getUsrPtr());
		double opMod;
		ballInfo->dir_.getX() < 0 ? opMod = -1.0 : opMod = 1.0;
		double offset = 6.0;	//Change this value when changing the offset
		cVector2 centOffset = cVector2(offset*opMod,0);
		//Offset the centroid of the paddle when calculating the vector for
		//the new direction
		ballInfo->dir_ = vUnitVector(obj1->getObjPos()-(obj2->getObjPos()+centOffset));
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
