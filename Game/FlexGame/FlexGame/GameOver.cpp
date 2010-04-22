#include "GameOver.h"
#include "WallMoving.h"
#include "ScoreKeeper.h"
#include <math.h>

GameOver GameOver::mGameOver;

GameOver::~GameOver()
{
}

void GameOver::enter()
{
	waitTime = 0;
		
}

void GameOver::exit()
{
	
}

void GameOver::pause()
{
}

void GameOver::resume()
{
}

void GameOver::processMouse(Mouse *mouse)
{
	
	
}

void GameOver::processKeyboard(Keyboard *keyboard)
{
}

void GameOver::processGamePad(GamePad *gamepad)
{
}

void GameOver::update(float delTime)
{
	waitTime++;
	if(waitTime == 20) 
		GameStateManager::getInstance()->state = aGameOver;
	if(waitTime == 35){
		GameStateManager::getInstance()->ResetWallPhysics();
			
		//NiPhysXProp* spWallProp = GameStateManager::getInstance()->physScene->GetPropAt(GameStateManager::getInstance()->currentWall);
		//spWallProp->SetSnapshot(GameStateManager::getInstance()->pkSnapshot);

	}
}