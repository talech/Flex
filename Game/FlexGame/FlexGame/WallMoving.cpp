#include "WallMoving.h"
#include "Collided.h"

WallMoving WallMoving::mWallMoving;

WallMoving::~WallMoving()
{
}

void WallMoving::enter()
{
	spWallProp = GameStateManager::getInstance()->physScene->GetPropAt(GameStateManager::getInstance()->currentWall);
	


	
	
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
	NiPhysXProp* spPlayerProp = GameStateManager::getInstance()->physScene->GetPropAt(1);
	
	for(int i = 0; i < spPlayerProp->GetDestinationsCount(); i++)
	{
		NxActor* playerActor = ((NiPhysXRigidBodyDest*)spPlayerProp->GetDestinationAt(i))->GetActor();
		NxVec3 currPos = playerActor->getGlobalPosition();

		if( keyboard )
		{
			if (keyboard->KeyWasPressed(NiInputKeyboard::KEY_LEFT) || keyboard->KeyIsDown(NiInputKeyboard::KEY_LEFT))
			{
				
				if( currPos.x > -5 )
				{
					playerActor->setGlobalPosition( currPos + NxVec3(-0.25, 0, 0) );
				}
			}
			else if (keyboard->KeyWasPressed(NiInputKeyboard::KEY_RIGHT) || keyboard->KeyIsDown(NiInputKeyboard::KEY_RIGHT))
			{
				if( currPos.x < 5 )
				{
					playerActor->setGlobalPosition( currPos + NxVec3(0.25, 0, 0) );
				}
			}
			else if (keyboard->KeyWasPressed(NiInputKeyboard::KEY_RIGHT) || keyboard->KeyIsDown(NiInputKeyboard::KEY_UP))
			{
				if( currPos.z > 13 )
				{
					playerActor->setGlobalPosition( currPos + NxVec3(0, 0, -0.25) );
				}
				
			}
			else if (keyboard->KeyWasPressed(NiInputKeyboard::KEY_RIGHT) || keyboard->KeyIsDown(NiInputKeyboard::KEY_DOWN))
			{
				if( currPos.z < 15 )
				{
					playerActor->setGlobalPosition( currPos + NxVec3(0, 0, 0.25) );
				}
			}
		}
	}
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
			position[2] = position[2] + .2;
			wallActor->setGlobalPosition(position);

			

		}
		if(position[2]>30){
			GameStateManager::getInstance()->currentWall = randNum();
			spWallProp = GameStateManager::getInstance()->physScene->GetPropAt(GameStateManager::getInstance()->currentWall);
			GameStateManager::getInstance()->physScene->RestoreSnapshotState(1);
		}
	}

	
	



	
}