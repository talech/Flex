#ifndef LoadState_H
#define LoadState_H

#include "GameState.h"

class LoadState : public GameState
{
public:

	~LoadState();

	void enter();
	void exit();

	void pause();
	void resume();

	void processMouse( Mouse* mouse );
	void processKeyboard( Keyboard* keyboard );
	void processGamePad( GamePad* gamepad );

	void update( float delTime );

	static LoadState* getInstance() { return &mLoadState; }

protected:

	static LoadState mLoadState;

	LoadState() {}
};
#endif