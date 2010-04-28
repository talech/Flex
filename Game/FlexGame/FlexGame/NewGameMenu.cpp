#include "NewGameMenu.h"
#include "WallMoving.h"
#include "HighScores.h"
#include "ScoreKeeper.h"
#include <NiUIManager.h>


NewGameMenu NewGameMenu::mNewGameMenu;

NewGameMenu::~NewGameMenu()
{
}

void NewGameMenu::enter()
{
	GameStateManager::getInstance()->state = aNewGame;
	GameStateManager::getInstance()->waitingForNewGame = true;
	GameStateManager::getInstance()->soundSystem->StopSong();
	GameStateManager::getInstance()->soundSystem->PlayBackground();
	
}

void NewGameMenu::exit()
{
	GameStateManager::getInstance()->waitingForNewGame = false;
	ScoreKeeper::getInstance()->resetGame();
	
}

void NewGameMenu::pause()
{
}

void NewGameMenu::resume()
{
}

void NewGameMenu::processMouse(Mouse *mouse)
{
}

void NewGameMenu::processKeyboard(Keyboard *keyboard)
{
	if( keyboard )
	{
		//if 1 -> Start continuous game mode
		if( keyboard->KeyWasPressed(NiInputKeyboard::KEY_1 ) ){
			GameStateManager::getInstance()->mode = Cont;
			GameStateManager::getInstance()->currentWall = randNum();
			GameStateManager::getInstance()->soundSystem->PlaySong();
			ScoreKeeper::getInstance()->startGame(updateTime);
			GameStateManager::getInstance()->changeState(WallMoving::getInstance());
		}
		//if 2 -> Start survivor game mode
		else if( keyboard->KeyWasPressed(NiInputKeyboard::KEY_2 ) ){
			GameStateManager::getInstance()->mode = Surviv;
			GameStateManager::getInstance()->currentWall = randNum();
			GameStateManager::getInstance()->soundSystem->PlaySong();
			ScoreKeeper::getInstance()->startGame(updateTime);
			GameStateManager::getInstance()->changeState(WallMoving::getInstance());
		}
		//if 3 -> view high scores
		else if( keyboard->KeyWasPressed(NiInputKeyboard::KEY_3) ){
			GameStateManager::getInstance()->changeState(HighScores::getInstance());
		}
	}
	
}

void NewGameMenu::processGamePad(GamePad *gamepad)
{
}

void NewGameMenu::setUIGroup( NiUIGroup* group )
{

}

void NewGameMenu::update(float delTime)
{
	updateTime = delTime;
	
}