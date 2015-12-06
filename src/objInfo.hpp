#ifndef OBJINFO_HPP
#define OBJINFO_HPP

#include "mathVector.hpp"

enum eObjID {
	BALL,
	PADDLE,
	WALL,
	GOAL
};

struct sObjInfo {
	int objID_;
};

struct sBallInfo : public sObjInfo {
	cVector2 dir_;
	cVector2 speed_;
	cVector2 vel_;
};

struct sGoalInfo : sObjInfo {
	int points_;
};

#endif
