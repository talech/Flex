#ifndef Paused_H
#define Paused_H

#include "GameState.h"


// The State that should sit on the bottom of the stack.
// When the running state is popped, the application terminates,
// ensuring the stack is never empty.
class Paused : public GameState
{
public:

	~Paused();

	void enter();
	void exit();

	void pause();
	void resume();

	void processMouse( Mouse* mouse );
	void processKeyboard( Keyboard* keyboard );
	void processGamePad( GamePad* gamepad );

	void update( float delTime );

	static Paused* getInstance() { return &mPaused; }

protected:
	float updateTime;
	
	static Paused mPaused;

	Paused() {;}


	
	
};
#endif