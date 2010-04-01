#include "WallMoving.h"
#include "Collided.h"

WallMoving WallMoving::mWallMoving;

WallMoving::~WallMoving()
{
}

void WallMoving::enter()
{
	GameStateManager::getInstance()->state = aWallMoving;
	spWallProp = GameStateManager::getInstance()->physScene->GetPropAt(GameStateManager::getInstance()->currentWall);
	//GameStateManager::getInstance()->pkSnapshot = spWallProp->GetSnapshot();

	do{ currentWall_2 = randNum(); }
	while(currentWall_2 == GameStateManager::getInstance()->currentWall);

	spWallProp_2 = GameStateManager::getInstance()->physScene->GetPropAt(currentWall_2);
	


	
	
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
			position[2] = position[2] - 1.5;
			wallActor->setGlobalPosition(position);
		}
		if(position[2]<(0)){
			NxActor* wallActor_2;
			NxVec3 position_2;
			for(int i = 0; i < spWallProp_2->GetDestinationsCount(); i++)
			{
				wallActor_2 = ((NiPhysXRigidBodyDest*)spWallProp_2->GetDestinationAt(i))->GetActor();
				position_2 = wallActor_2->getGlobalPosition();
				position_2[2] = position_2[2] - 1.5;
				wallActor_2->setGlobalPosition(position_2);
			}
		}
		if(position[2]<(-20)){
			ResetWall();
			GameStateManager::getInstance()->currentWall = currentWall_2;
			do{ currentWall_2 = randNum(); }
			while(currentWall_2 == GameStateManager::getInstance()->currentWall);

			spWallProp = GameStateManager::getInstance()->physScene->GetPropAt(GameStateManager::getInstance()->currentWall);
			spWallProp_2 = GameStateManager::getInstance()->physScene->GetPropAt(currentWall_2);
		}
	}

		
}

void 
WallMoving::ResetWall(){
	NxActor* wallActor;
	NxVec3 position;
		
	for(int i = 0; i < spWallProp->GetDestinationsCount(); i++)
	{
		wallActor = ((NiPhysXRigidBodyDest*)spWallProp->GetDestinationAt(i))->GetActor();
		position = wallActor->getGlobalPosition();
		position[2] = 17;
		wallActor->setGlobalPosition(position);
	}
}