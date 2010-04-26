#include "Paused.h"
#include "WallMoving.h"
#include "GameOver.h"
#include "NewGameMenu.h"
#include "ScoreKeeper.h"


Paused Paused::mPaused;

Paused::~Paused()
{
}

void Paused::enter()
{
		
}

void Paused::exit()
{
	GameStateManager::getInstance()->ResetWallPhysics();	
}

void Paused::pause()
{
}

void Paused::resume()
{
}

void Paused::processMouse(Mouse *mouse)
{
}

void Paused::processKeyboard(Keyboard *keyboard)
{
	//toggle between paused game
	if (keyboard->KeyWasPressed(NiInputKeyboard::KEY_SPACE)){
		GameStateManager::getInstance()->state = aWallMoving;
		GameStateManager::getInstance()->changeState(WallMoving::getInstance());
	}
	if (keyboard->KeyWasPressed(NiInputKeyboard::KEY_Q)){
		if(GameStateManager::getInstance()->mode == Cont){
			GameStateManager::getInstance()->state = aGameOver;
			GameStateManager::getInstance()->ResetWallPhysics();
			GameStateManager::getInstance()->changeState(GameOver::getInstance());
		}
		else{
			GameStateManager::getInstance()->state = aNewGame;
			ScoreKeeper::getInstance()->resetGame();
			GameStateManager::getInstance()->ResetWallPhysics();
			GameStateManager::getInstance()->changeState(NewGameMenu::getInstance());
		}
	}

}

void Paused::processGamePad(GamePad *gamepad)
{
}

void Paused::update(float delTime)
{
	

		
}

