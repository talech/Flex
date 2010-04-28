#ifndef WallMoving_H
#define WallMoving_H

#include "GameState.h"
#include "ScoreKeeper.h"

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

	WallMoving() {vel = 2.5;}

	
	NiPhysXProp* spWallProp;
	NiPhysXProp* spWallProp_2;
	int currentWall_2;
	float vel;
	float oldTime;
	bool incSpeed;

	void MoveFrontWall(float dT);
	void MoveBackWall(float dT);
	void ScoreWall();
	void ResetWall();
};
#endif