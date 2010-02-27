#include "Collided.h"
#include "WallMoving.h"
#include <math.h>

Collided Collided::mCollided;

Collided::~Collided()
{
}

void Collided::enter()
{
	waitTime = 0;
	
	
}

void Collided::exit()
{
	GameStateManager::getInstance()->currentWall = randNum();
	
}

void Collided::pause()
{
}

void Collided::resume()
{
}

void Collided::processMouse(Mouse *mouse)
{
	
	
}

void Collided::processKeyboard(Keyboard *keyboard)
{
}

void Collided::processGamePad(GamePad *gamepad)
{
}

void Collided::update(float delTime)
{
	waitTime++;
	if(waitTime == 20){
		GameStateManager::getInstance()->ResetWallPhysics();
	}
}