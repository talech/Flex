#include "Paused.h"
#include "WallMoving.h"


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

}

void Paused::processGamePad(GamePad *gamepad)
{
}

void Paused::update(float delTime)
{
	

		
}

