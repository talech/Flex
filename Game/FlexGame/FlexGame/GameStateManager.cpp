#include "GameStateManager.h"
#include "GameState.h"
#include "Flex.h"

GameStateManager GameStateManager::mManager;

void GameStateManager::start(GameState *state)
{
	state->enter();
	mStack.push(state);

	
}

void GameStateManager::changeState(GameState *state)
{
	assert(mRunning);

	mStack.top()->exit();
	mStack.pop();
	state->enter();
	mStack.push(state);
}

void GameStateManager::pushState(GameState *state)
{
	assert(mRunning);
	mStack.top()->pause();
	state->enter();
	mStack.push(state);
}

void GameStateManager::popState()
{
	assert(mRunning);
	mStack.top()->exit();
	mStack.pop();
	mStack.top()->resume();
}

void GameStateManager::processMouse(Mouse *mouse)
{
	mStack.top()->processMouse(mouse);
}

void GameStateManager::processKeyboard(Keyboard *keyboard)
{
	mStack.top()->processKeyboard(keyboard);
}

void GameStateManager::processGamePad(GamePad *gamePad)
{
	mStack.top()->processGamePad(gamePad);
}

void GameStateManager::update(float delTime)
{
	mStack.top()->update(delTime);
}

GameStateManager::~GameStateManager()
{
	// clean up all the states
		while (!mStack.empty()) {
			mStack.top()->exit();
			mStack.pop();
		}
}

void GameStateManager::addApplication(Flex *app)
{
	mApp = app;
}

/*void GameStateManager::setCamera(NiCameraPtr cam)
{
	mCamera = cam;
	mApp->SetCamera(mCamera);
}*/

void GameStateManager::terminateApp()
{
	mApp->Terminate();
}

void GameStateManager::startRunning()
{
	mRunning = true;
}
