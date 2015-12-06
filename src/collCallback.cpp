#include "collCallback.hpp"

void ballCollCallback (cCollObj* obj1, cCollObj* obj2, cVector2 collVector) {
	//Get the normal direction
	
	cVector2 norm = vUnitVector(collVector);
	sBallInfo* ballInfo = static_cast<sBallInfo*>(obj1->getUsrPtr());
	cVector2 invVel = vUnitVector(ballInfo->vel_)*cVector2(-1,-1);
	ballInfo->vel_ = vMirror(ballInfo->vel_,norm)*-1;
}
