#ifndef HighScores_H
#define HighScores_H

#include "GameState.h"
#include <NiUIManager.h>

class HighScores : public GameState
{
public:

	~HighScores();

	void enter();
	void exit();

	void pause();
	void resume();

	void processMouse( Mouse* mouse );
	void processKeyboard( Keyboard* keyboard );
	void processGamePad( GamePad* gamepad );

	void update( float delTime );

	void setUIGroup( NiUIGroup* group );

	static HighScores* getInstance() { return &mHighScores; }

protected:

	NiUIGroup* mMenuGroup;

	static HighScores mHighScores;

	HighScores() {}
};
#endif