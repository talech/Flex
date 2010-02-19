#ifndef InitializeState_H
#define InitializeState_H

#include "GameState.h"

class InitializeState : public GameState
{
public:

	~InitializeState();

	void enter();
	void exit();

	void pause();
	void resume();

	void processMouse( Mouse* mouse );
	void processKeyboard( Keyboard* keyboard );
	void processGamePad( GamePad* gamepad );

	void update( float delTime );

	static InitializeState* getInstance() { return &mInitializeState; }

protected:

	static InitializeState mInitializeState;

	InitializeState() {}
};
#endif