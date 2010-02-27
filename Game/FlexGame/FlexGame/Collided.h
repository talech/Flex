#ifndef Collided_H
#define Collided_H

#include "GameState.h"


class Collided : public GameState
{
public:

	~Collided();

	void enter();
	void exit();

	void pause();
	void resume();

	void processMouse( Mouse* mouse );
	void processKeyboard( Keyboard* keyboard );
	void processGamePad( GamePad* gamepad );

	void update( float delTime );

	static Collided* getInstance() { return &mCollided; }

protected:

	static Collided mCollided;
	

	Collided() {}
	int waitTime;
};
#endif