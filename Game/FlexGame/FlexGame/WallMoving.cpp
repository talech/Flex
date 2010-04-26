#include "WallMoving.h"
#include "Collided.h"
#include "Paused.h"


WallMoving WallMoving::mWallMoving;

WallMoving::~WallMoving()
{
}

void WallMoving::enter()
{
	NiFixedString name = "Start";
	GameStateManager::getInstance()->physScene->AddSnapshotState(name);

	GameStateManager::getInstance()->state = aWallMoving;
	spWallProp = GameStateManager::getInstance()->physScene->GetPropAt(GameStateManager::getInstance()->currentWall);
	//GameStateManager::getInstance()->pkSnapshot = spWallProp->GetSnapshot();

	do{ currentWall_2 = randNum(); }
	while(currentWall_2 == GameStateManager::getInstance()->currentWall);

	spWallProp_2 = GameStateManager::getInstance()->physScene->GetPropAt(currentWall_2);
	
	GameStateManager::getInstance()->toggleEnableWall(true);	

	incSpeed = false;
	oldTime = 0;
	
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
	//toggle between paused game
	if (keyboard->KeyIsDown(NiInputKeyboard::KEY_SPACE)){
		if(GameStateManager::getInstance()->mode == Cont)
			GameStateManager::getInstance()->state = aPausedCont;
		else
			GameStateManager::getInstance()->state = aPausedSurviv;
		GameStateManager::getInstance()->changeState(Paused::getInstance());
		
	}

	if (keyboard->KeyIsDown(NiInputKeyboard::KEY_UP)){
		vel = vel + 0.1;	
	}

	if (keyboard->KeyIsDown(NiInputKeyboard::KEY_DOWN)){
		if(vel>4)
			vel = vel - 0.1;	
	}
	
}

void WallMoving::processGamePad(GamePad *gamepad)
{
}

void WallMoving::update(float delTime)
{
	//if I just changed state to WallMoving, don't move the wall on the first update
	if(oldTime == 0) oldTime = delTime;

	float dT = delTime - oldTime; 
	oldTime = delTime;

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
			float z = position[2];
			position[2] = position[2] + (dT*vel);
			wallActor->setGlobalPosition(position);
			
			NILOG(NIMESSAGE_GENERAL_0, "WALL: z-old: %f z-new: %f delTime: %f ", 
            z, position[2], dT);
			

		}
		if(position[2]>(0)){
			NxActor* wallActor_2;
			NxVec3 position_2;
			for(int i = 0; i < spWallProp_2->GetDestinationsCount(); i++)
			{
				wallActor_2 = ((NiPhysXRigidBodyDest*)spWallProp_2->GetDestinationAt(i))->GetActor();
				position_2 = wallActor_2->getGlobalPosition();
				float z = position_2[2];
				position_2[2] = position_2[2] + (dT*vel);
				wallActor_2->setGlobalPosition(position_2);

				NILOG(NIMESSAGE_GENERAL_0, "WALL: z-old: %f z-new: %f delTime: %f ", 
				z, position_2[2], dT);
			}
		}
		if(position[2]>(20)){
			ScoreKeeper::getInstance()->scoreWall();
			GameStateManager::getInstance()->toggleEnableWall(false);
			GameStateManager::getInstance()->currentWall = currentWall_2;
			ResetWall();
			GameStateManager::getInstance()->toggleEnableWall(true);

			do{ currentWall_2 = randNum(); }
			while(currentWall_2 == GameStateManager::getInstance()->currentWall);

			spWallProp = GameStateManager::getInstance()->physScene->GetPropAt(GameStateManager::getInstance()->currentWall);
			spWallProp_2 = GameStateManager::getInstance()->physScene->GetPropAt(currentWall_2);

			if(vel < 0.05 && (ScoreKeeper::getInstance()->getScore()>0) ){
				if((ScoreKeeper::getInstance()->getScore()%10) == 0 && !incSpeed){
					vel += 0.1;	
					incSpeed = true;
				}
				else if((ScoreKeeper::getInstance()->getScore()%10) != 0){	
					incSpeed = false;
				}
			}
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
		position[2] = -25;
		wallActor->setGlobalPosition(position);
	}
}