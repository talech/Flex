#include "WallMoving.h"

WallMoving WallMoving::mWallMoving;

WallMoving::~WallMoving()
{
}

void WallMoving::enter()
{
	spWallProp = GameStateManager::getInstance()->physScene->GetPropAt(2);
	


	
	
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
	NxActor* playerActor = ((NiPhysXRigidBodyDest*)spPlayerProp->GetDestinationAt(0))->GetActor();
	if( keyboard )
	{
		if (keyboard->KeyWasPressed(NiInputKeyboard::KEY_LEFT) || keyboard->KeyIsDown(NiInputKeyboard::KEY_LEFT))
		{
			NxVec3 currPos = playerActor->getGlobalPosition();
			if( currPos.x > -2 )
			{
				playerActor->setGlobalPosition( currPos + NxVec3(-0.25, 0, 0) );
			}
		}
		else if (keyboard->KeyWasPressed(NiInputKeyboard::KEY_RIGHT) || keyboard->KeyIsDown(NiInputKeyboard::KEY_RIGHT))
		{
			NxVec3 currPos = playerActor->getGlobalPosition();
			if( currPos.x < 2 )
			{
				playerActor->setGlobalPosition( currPos + NxVec3(0.25, 0, 0) );
			}
		}
	}

}

void WallMoving::processGamePad(GamePad *gamepad)
{
}

void WallMoving::update(float delTime)
{
	NxActor* wallActor;
	NxVec3 position;

	
	for(int i = 0; i < spWallProp->GetDestinationsCount(); i++)
	{
		wallActor = ((NiPhysXRigidBodyDest*)spWallProp->GetDestinationAt(i))->GetActor();
		position = wallActor->getGlobalPosition();
		position[2] = position[2] + .1;
		wallActor->setGlobalPosition(position);


	}
	
	



	
}