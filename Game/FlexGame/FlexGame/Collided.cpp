/****************************************************
Flex - Senior Project
Copyright (C) 2010 Tamara Levy
*****************************************************/

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
	if(GameStateManager::getInstance()->mode == Surviv && !GameStateManager::getInstance()->smashing){
		ScoreKeeper::getInstance()->loseLife();	
	}

	
	
	
}

void Collided::exit()
{
	GameStateManager::getInstance()->collision = false;
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
	if(!GameStateManager::getInstance()->smashing){
		if(waitTime == 30) {
			if(GameStateManager::getInstance()->mode == Surviv)
				GameStateManager::getInstance()->state = aCollidedSurviv;
			else
				GameStateManager::getInstance()->state = aCollidedCont;

		}
		if(waitTime == 40){
			GameStateManager::getInstance()->toggleEnableWall(false);
			GameStateManager::getInstance()->ResetWallPhysics();
			//NiPhysXProp* spWallProp = GameStateManager::getInstance()->physScene->GetPropAt(GameStateManager::getInstance()->currentWall);
			//spWallProp->SetSnapshot(GameStateManager::getInstance()->pkSnapshot);
		}

		if(waitTime == 50){
			if(ScoreKeeper::getInstance()->getLives() < 0){
				ScoreKeeper::getInstance()->endGame(delTime);
				GameStateManager::getInstance()->changeState(GameOver::getInstance());
			}
			else
				GameStateManager::getInstance()->changeState(WallMoving::getInstance());
		}
	}
	//if smashing wait more
	else{
		if(waitTime == 50) {
			GameStateManager::getInstance()->state = aSmash;

		}
		if(waitTime == 60){
			GameStateManager::getInstance()->toggleEnableWall(false);
			GameStateManager::getInstance()->ResetWallPhysics();
			//NiPhysXProp* spWallProp = GameStateManager::getInstance()->physScene->GetPropAt(GameStateManager::getInstance()->currentWall);
			//spWallProp->SetSnapshot(GameStateManager::getInstance()->pkSnapshot);
		}

		if(waitTime == 70){
			GameStateManager::getInstance()->changeState(WallMoving::getInstance());
		}
	}
}