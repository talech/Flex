#ifndef NewGameMenu_H
#define NewGameMenu_H

#include "GameState.h"
#include <NiUIManager.h>

class NewGameMenu : public GameState
{
public:

	~NewGameMenu();

	void enter();
	void exit();

	void pause();
	void resume();

	void processMouse( Mouse* mouse );
	void processKeyboard( Keyboard* keyboard );
	void processGamePad( GamePad* gamepad );

	void update( float delTime );

	void setUIGroup( NiUIGroup* group );

	static NewGameMenu* getInstance() { return &mNewGameMenu; }

protected:

	NiUIGroup* mMenuGroup;

	static NewGameMenu mNewGameMenu;

	NewGameMenu() {}
};
#endif