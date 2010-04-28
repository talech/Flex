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
	ScoreKeeper::getInstance()->smashToggle(false);	
	spWallProp = GameStateManager::getInstance()->physScene->GetPropAt(GameStateManager::getInstance()->currentWall);
	//GameStateManager::getInstance()->pkSnapshot = spWallProp->GetSnapshot();

	do{ currentWall_2 = randNum(); }
	while(currentWall_2 == GameStateManager::getInstance()->currentWall);

	spWallProp_2 = GameStateManager::getInstance()->physScene->GetPropAt(currentWall_2);
	
	GameStateManager::getInstance()->toggleEnableWall(true);	

	incSpeed = false;
	oldTime = 0;

	if(GameStateManager::getInstance()->currentWall > NUM_WALLS+1)
		GameStateManager::getInstance()->smashing = true;
	else
		GameStateManager::getInstance()->smashing = false;
	
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
		MoveFrontWall(dT);
	}
		
}

void
WallMoving::MoveFrontWall(float dT){
	NxActor* wallActor;
	NxVec3 position;

	for(int i = 0; i < spWallProp->GetDestinationsCount(); i++)
	{
		wallActor = ((NiPhysXRigidBodyDest*)spWallProp->GetDestinationAt(i))->GetActor();
		position = wallActor->getGlobalPosition();
		float z = position[2];
		position[2] = position[2] + (dT*vel);
		wallActor->setGlobalPosition(position);
		
		/*NILOG(NIMESSAGE_GENERAL_0, "WALL: z-old: %f z-new: %f delTime: %f ", 
          z, position[2], dT);*/
	}
	if(position[2]>(0)){
		MoveBackWall(dT);
	}
	if(position[2]>(20)){
		ScoreWall();
	}


}

void
WallMoving::MoveBackWall(float dT){
	NxActor* wallActor_2;
	NxVec3 position_2;
	for(int i = 0; i < spWallProp_2->GetDestinationsCount(); i++)
	{
		wallActor_2 = ((NiPhysXRigidBodyDest*)spWallProp_2->GetDestinationAt(i))->GetActor();
		position_2 = wallActor_2->getGlobalPosition();
		float z = position_2[2];
		position_2[2] = position_2[2] + (dT*vel);
		wallActor_2->setGlobalPosition(position_2);

		/*NILOG(NIMESSAGE_GENERAL_0, "WALL: z-old: %f z-new: %f delTime: %f ", 
		z, position_2[2], dT);*/
	}
}

void
WallMoving::ScoreWall(){
	
	ScoreKeeper::getInstance()->scoreWall();
	GameStateManager::getInstance()->toggleEnableWall(false);
	GameStateManager::getInstance()->currentWall = currentWall_2;
	ResetWall();
	GameStateManager::getInstance()->toggleEnableWall(true);

	do{ currentWall_2 = randNum(); }
	while(currentWall_2 == GameStateManager::getInstance()->currentWall);

	spWallProp = GameStateManager::getInstance()->physScene->GetPropAt(GameStateManager::getInstance()->currentWall);
	spWallProp_2 = GameStateManager::getInstance()->physScene->GetPropAt(currentWall_2);


	//increase speed 
	if(vel < 4.0  && (ScoreKeeper::getInstance()->getWalls()>0) ){
		if((ScoreKeeper::getInstance()->getScore()%10) == 0 && !incSpeed){
			vel += 0.1;	
			incSpeed = true;
		}
		else if((ScoreKeeper::getInstance()->getWalls()%10) != 0){	
			incSpeed = false;
		}
	}

	//is it a smashing wall?
	if(GameStateManager::getInstance()->currentWall > NUM_WALLS+1)
		GameStateManager::getInstance()->smashing = true;
	else
		GameStateManager::getInstance()->smashing = false;
	
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