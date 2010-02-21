#ifndef WallMoving_H
#define WallMoving_H

#include "GameState.h"

// The State that should sit on the bottom of the stack.
// When the running state is popped, the application terminates,
// ensuring the stack is never empty.
class WallMoving : public GameState
{
public:

	~WallMoving();

	void enter();
	void exit();

	void pause();
	void resume();

	void processMouse( Mouse* mouse );
	void processKeyboard( Keyboard* keyboard );
	void processGamePad( GamePad* gamepad );

	void update( float delTime );

	static WallMoving* getInstance() { return &mWallMoving; }

protected:

	static WallMoving mWallMoving;

	WallMoving() {}

	
	NiPhysXProp* spWallProp;
};
#endif