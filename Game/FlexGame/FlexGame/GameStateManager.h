#ifndef GameStateManager_H
#define GameStateManager_H

#include <NiApplication.h>
#include <efdPhysX/PhysXSDKManager.h>
#include <NiPhysX.h>
typedef NiInputMouse Mouse;
typedef NiInputKeyboard Keyboard;
typedef NiInputGamePad GamePad;

#include <stack>
#include "Defines.h"



class GameState;
class Flex;


class GameStateManager
{
public:

	~GameStateManager();

	void start( GameState* state );
	void changeState( GameState* state );
	void pushState( GameState* state );
	void popState();
	
	void processMouse( Mouse* mouse );
	void processKeyboard( Keyboard* keyboard );
	void processGamePad( GamePad* gamePad );

	void update( float delTime );

	void addApplication( Flex* app );
	void terminateApp();

	void startRunning();
	void ResetWallPhysics();

	//void setCamera( NiCameraPtr cam );

	static GameStateManager* getInstance() { return &mManager; }

	// Giving access to these for the sake of prototyping.  Play nice.
	efdPhysX::PhysXSDKManager* physManager;
	NiPhysXScenePtr physScene;
	
	
	NiNodePtr scene;

	bool waitingForNewGame;
	bool collision;
	int currentWall;
	NiPhysXPropDesc* pkSnapshot;

	ActiveState state;

private:

	bool mRunning;

	NiCameraPtr mCamera;

	Flex* mApp;

	std::stack<GameState*> mStack;

	static GameStateManager mManager;

	GameStateManager() { mRunning = false;  waitingForNewGame = false; collision = false; currentWall = 2; }

	
};
#endif