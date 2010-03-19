#include "NewGameMenu.h"
#include "WallMoving.h"
#include <NiUIManager.h>


NewGameMenu NewGameMenu::mNewGameMenu;

NewGameMenu::~NewGameMenu()
{
}

void NewGameMenu::enter()
{
	GameStateManager::getInstance()->state = aNewGame;
	GameStateManager::getInstance()->waitingForNewGame = true;
}

void NewGameMenu::exit()
{
	GameStateManager::getInstance()->waitingForNewGame = false;
	
}

void NewGameMenu::pause()
{
}

void NewGameMenu::resume()
{
}

void NewGameMenu::processMouse(Mouse *mouse)
{
}

void NewGameMenu::processKeyboard(Keyboard *keyboard)
{
	if( keyboard )
	{
		if( keyboard->KeyWasPressed(NiInputKeyboard::KEY_SPACE ) ){
			NiFixedString name = "Start";
			GameStateManager::getInstance()->physScene->AddSnapshotState(name);
			GameStateManager::getInstance()->currentWall = randNum();
			GameStateManager::getInstance()->changeState(WallMoving::getInstance());
		}
	}
	
}

void NewGameMenu::processGamePad(GamePad *gamepad)
{
}

void NewGameMenu::setUIGroup( NiUIGroup* group )
{

}

void NewGameMenu::update(float delTime)
{
	/*
	 // A call to Simulate starts the simulation.
    // We pass it the target time. 
    GameStateManager::getInstance()->physScene->Simulate(delTime);
    
    // We want the results immediately, so we call FetchResults with
    // the same time we just asked for, and the second argument as true
    // to block on the results.
    GameStateManager::getInstance()->physScene->FetchResults(delTime, true);

    // Now the actors have been moved, but we need to update the
    // Gamebryo objects. UpdateDestinations does that. Note it is called
    // on the NiPhysXScene that owns the objects being updated.
    GameStateManager::getInstance()->physScene->UpdateDestinations(delTime);
    
    // FInally we update the scene graph.
    GameStateManager::getInstance()->scene->Update(delTime);
	*/
}