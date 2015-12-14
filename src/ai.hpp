#ifndef AI_HPP
#define AI_HPP

#include <ctime>
#include <cstdlib>

#include "collObject.hpp"

#include "objInfo.hpp"

enum class ePaddleMovement {
	P_UP,
	P_DOWN,
	P_NONE
};

ePaddleMovement updateAiEasy (const cCollObj& ball, const cCollObj& controlledPaddle);
ePaddleMovement updateAiMed (const cCollObj& ball, const cCollObj& controlledPaddle);
ePaddleMovement updateAiHard (const cCollObj& ball, const cCollObj& controlledPaddle);

#endif
