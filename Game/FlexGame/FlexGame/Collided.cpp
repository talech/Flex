#include "Collided.h"
#include "WallMoving.h"
#include "ScoreKeeper.h"
#include "GameOver.h"
#include <math.h>

Collided Collided::mCollided;

Collided::~Collided()
{
}

void Collided::enter()
{
	waitTime = 0;
	if(GameStateManager::getInstance()->mode == Surviv){
		ScoreKeeper::getInstance()->loseLife();
		if(ScoreKeeper::getInstance()->getLives() < 0)
			GameStateManager::getInstance()->changeState(GameOver::getInstance());
	}
	
	
	
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
	if(waitTime == 20) {
		if(GameStateManager::getInstance()->mode == Surviv)
			GameStateManager::getInstance()->state = aCollidedSurviv;
		else
			GameStateManager::getInstance()->state = aCollidedCont;

	}
	if(waitTime == 35){
		GameStateManager::getInstance()->toggleEnableWall(false);
		GameStateManager::getInstance()->ResetWallPhysics();
		GameStateManager::getInstance()->changeState(WallMoving::getInstance());
		//NiPhysXProp* spWallProp = GameStateManager::getInstance()->physScene->GetPropAt(GameStateManager::getInstance()->currentWall);
		//spWallProp->SetSnapshot(GameStateManager::getInstance()->pkSnapshot);

	}
}