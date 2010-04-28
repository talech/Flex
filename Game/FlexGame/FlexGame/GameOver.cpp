#include "GameOver.h"
#include "NewGameMenu.h"
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
	if (keyboard->KeyIsDown(NiInputKeyboard::KEY_SPACE)){
		GameStateManager::getInstance()->state = aNewGame;
		
		if(ScoreKeeper::getInstance()->isHigh()){
			int ohyeah = 0;
		}
		else{
			ScoreKeeper::getInstance()->resetGame();
			GameStateManager::getInstance()->changeState(NewGameMenu::getInstance());
		}
		
	}
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
	}
}