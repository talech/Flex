/****************************************************
Flex - Senior Project
Copyright (C) 2010 Tamara Levy
*****************************************************/

#ifndef GameOver_H
#define GameOver_H

#include "GameState.h"


class GameOver : public GameState
{
public:

	~GameOver();

	void enter();
	void exit();

	void pause();
	void resume();

	void processMouse( Mouse* mouse );
	void processKeyboard( Keyboard* keyboard );
	void processGamePad( GamePad* gamepad );

	void update( float delTime );

	static GameOver* getInstance() { return &mGameOver; }

protected:

	static GameOver mGameOver;
	int waitTime;

	GameOver() {}
};
#endif