#include "HighScores.h"
#include "NewGameMenu.h"
#include <NiUIManager.h>


HighScores HighScores::mHighScores;

HighScores::~HighScores()
{
}

void HighScores::enter()
{
	GameStateManager::getInstance()->state = aHighScores;
}

void HighScores::exit()
{
	}

void HighScores::pause()
{
}

void HighScores::resume()
{
}

void HighScores::processMouse(Mouse *mouse)
{
}

void HighScores::processKeyboard(Keyboard *keyboard)
{
	if( keyboard )
	{
		//if DELETE -> go back to New Game Menu
		if( keyboard->KeyWasPressed(NiInputKeyboard::KEY_BACK ) ){
			GameStateManager::getInstance()->changeState(NewGameMenu::getInstance());
		}
	}
	
}

void HighScores::processGamePad(GamePad *gamepad)
{
}

void HighScores::setUIGroup( NiUIGroup* group )
{

}

void HighScores::update(float delTime)
{
}