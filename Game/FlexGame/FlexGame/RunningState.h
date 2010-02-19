#ifndef RunningState_H
#define RunningState_H

#include "GameState.h"

// The State that should sit on the bottom of the stack.
// When the running state is popped, the application terminates,
// ensuring the stack is never empty.
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