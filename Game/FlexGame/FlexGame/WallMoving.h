/****************************************************
Flex - Senior Project
Copyright (C) 2010 Tamara Levy
*****************************************************/

#ifndef WallMoving_H
#define WallMoving_H

#include "GameState.h"
#include "ScoreKeeper.h"


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

	WallMoving() {vel = 3.5;}

	
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