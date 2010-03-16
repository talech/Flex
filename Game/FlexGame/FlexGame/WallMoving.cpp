#include "WallMoving.h"
#include "Collided.h"

WallMoving WallMoving::mWallMoving;

WallMoving::~WallMoving()
{
}

void WallMoving::enter()
{
	spWallProp = GameStateManager::getInstance()->physScene->GetPropAt(GameStateManager::getInstance()->currentWall);
	//GameStateManager::getInstance()->pkSnapshot = spWallProp->GetSnapshot();
	


	
	
}

void WallMoving::exit()
{
	
}

void WallMoving::pause()
{
}

void WallMoving::resume()
{
}

void WallMoving::processMouse(Mouse *mouse)
{
}

void WallMoving::processKeyboard(Keyboard *keyboard)
{
}

void WallMoving::processGamePad(GamePad *gamepad)
{
}

void WallMoving::update(float delTime)
{
	if(GameStateManager::getInstance()->collision){
		GameStateManager::getInstance()->changeState(Collided::getInstance());
	}
	else{
		NxActor* wallActor;
		NxVec3 position;

		
		for(int i = 0; i < spWallProp->GetDestinationsCount(); i++)
		{
			wallActor = ((NiPhysXRigidBodyDest*)spWallProp->GetDestinationAt(i))->GetActor();
			position = wallActor->getGlobalPosition();
			position[2] = position[2] - 1;
			wallActor->setGlobalPosition(position);

			

		}
		if(position[2]<(-30)){
			//spWallProp->SetSnapshot(GameStateManager::getInstance()->pkSnapshot);
			GameStateManager::getInstance()->physScene->RestoreSnapshotState(1);
			GameStateManager::getInstance()->currentWall = randNum();
			spWallProp = GameStateManager::getInstance()->physScene->GetPropAt(GameStateManager::getInstance()->currentWall);
			//GameStateManager::getInstance()->pkSnapshot = spWallProp->GetSnapshot();
			
		}
	}

	
	



	
}