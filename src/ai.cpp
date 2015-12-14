#include "ai.hpp"

ePaddleMovement updateAiEasy (const cCollObj& ball, const cCollObj& controlledPaddle) {
	srand(time(NULL));
	int paddleHh = controlledPaddle.getCollShape()->getData().at(0).getY();
	double paddleY = controlledPaddle.getObjPos().getY(),
		   ballY = ball.getObjPos().getY();
	// Randomize the hitting point on the paddle
	paddleY += (rand()%(2*paddleHh)-paddleHh);
	if (ballY > paddleY)
		return ePaddleMovement::P_DOWN;
	else if (ballY < paddleY)
		return ePaddleMovement::P_UP;
	return ePaddleMovement::P_NONE;
}

ePaddleMovement updateAiMed (const cCollObj& ball, const cCollObj& controlledPaddle) {
	return ePaddleMovement::P_NONE;
}

ePaddleMovement updateAiHard (const cCollObj& ball, const cCollObj& controlledPaddle) {
	return ePaddleMovement::P_NONE;
}
