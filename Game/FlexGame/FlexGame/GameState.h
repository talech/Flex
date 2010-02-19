#ifndef GameState_H
#define GameState_H

#include "GameStateManager.h"

class GameState
{
public:

		virtual void enter() = 0;
		virtual void exit() = 0;

		virtual void pause() = 0;
		virtual void resume() = 0;

		virtual void processMouse( Mouse* mouse ) = 0;
		virtual void processKeyboard( Keyboard* keyboard ) = 0;
		virtual void processGamePad( GamePad* gamepad ) = 0;

		virtual void update( float delTime ) = 0;

		void changeState(GameState* state) { GameStateManager::getInstance()->changeState(state); }
		void pushState(GameState* state) { GameStateManager::getInstance()->pushState(state); }
		void popState() { GameStateManager::getInstance()->popState(); }
protected:

	GameState() {}
};

#endif