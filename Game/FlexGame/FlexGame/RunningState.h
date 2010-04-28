/****************************************************
Flex - Senior Project
Copyright (C) 2010 Tamara Levy
*****************************************************/

#ifndef RunningState_H
#define RunningState_H

#include "GameState.h"

//This state should sit at the bottom of the stack.
//So the stack is never empty, when the game terminates 
//Running_State is popped
class RunningState : public GameState
{
public:

	~RunningState();

	void enter();
	void exit();

	void pause();
	void resume();

	void processMouse( Mouse* mouse );
	void processKeyboard( Keyboard* keyboard );
	void processGamePad( GamePad* gamepad );

	void update( float delTime );

	static RunningState* getInstance() { return &mRunningState; }

protected:

	static RunningState mRunningState;

	RunningState() {}
};
#endif