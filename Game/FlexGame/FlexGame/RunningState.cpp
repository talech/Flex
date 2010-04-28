/****************************************************
Flex - Senior Project
Copyright (C) 2010 Tamara Levy
*****************************************************/

#include "RunningState.h"

RunningState RunningState::mRunningState;

RunningState::~RunningState()
{
}

void RunningState::enter()
{
	GameStateManager::getInstance()->startRunning();
}

void RunningState::exit()
{
	GameStateManager::getInstance()->terminateApp();
}

void RunningState::pause()
{
}

void RunningState::resume()
{
}

void RunningState::processMouse(Mouse *mouse)
{
}

void RunningState::processKeyboard(Keyboard *keyboard)
{
}

void RunningState::processGamePad(GamePad *gamepad)
{
}

void RunningState::update(float delTime)
{
}