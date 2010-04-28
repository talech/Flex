#ifndef GotHigh_H
#define GotHigh_H

#include "GameState.h"
#include <string>


class GotHigh : public GameState
{
public:

	~GotHigh();

	void enter();
	void exit();

	void pause();
	void resume();

	void processMouse( Mouse* mouse );
	void processKeyboard( Keyboard* keyboard );
	void processGamePad( GamePad* gamepad );

	void update( float delTime );
	
	const char* getName(){ return name.c_str();}

	static GotHigh* getInstance() { return &mGotHigh; }

	string name;

protected:

	static GotHigh mGotHigh;
	int index;
	

	void ProcessWriting(Keyboard *keyboard);
	void AddToString(char* letter);

	GotHigh() {name = "_____\0";}
};
#endif