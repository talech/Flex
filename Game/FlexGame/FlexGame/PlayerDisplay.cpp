#include "PlayerDisplay.h"
#include <CommonTypes.h>

PlayerDisplay::PlayerDisplay(Player* pPlayer, NiNodePtr sScene, NiPhysXScenePtr physScene){
	scene = sScene;
	m_Player = pPlayer;
	spPlayerProp = physScene->GetPropAt(1);
	playing = false; 
}

PlayerDisplay::~PlayerDisplay(){
}

void
PlayerDisplay::Update(){
	//Update frame to next frame if playing
	if(playing){
		m_Player->m_frameIndex = m_Player->GetNextFrameIndex(m_Player->m_frameIndex);
		m_Player->UpdateFrame(m_Player->m_frameIndex);
		m_Player->m_frameIndex = m_Player->GetNextFrameIndex(m_Player->m_frameIndex);
		m_Player->UpdateFrame(m_Player->m_frameIndex);
	}

	if(m_Player)
		DrawPlayer(m_Player);
}

void 
PlayerDisplay::DrawPlayer(Player* player){
	Skeleton* pSkeleton = player->GetSkeleton();

	if( pSkeleton && pSkeleton->m_pJoints.size() > 0 ){
		//Draw Root Joint as a sphere
		Joint* pJoint = pSkeleton->GetJointByIndex(root);
	
		float tx = pJoint->m_translation[VX];
		float ty = pJoint->m_translation[VY];
		float tz = pJoint->m_translation[VZ];

		NiAVObject* m_Sphere = (NiAVObject*)scene->GetObjectByName("root");
		NxActor* jointActor = ((NiPhysXRigidBodyDest*)spPlayerProp->GetDestinationAt(0))->GetActor();
	
		pJoint->UpdateTransformation(true);
		//vec3 m_transf = pSkeleton->ComputeJointGlobalPosition(root);
		
		Transform m_transf = pJoint->m_global;
		
		NxVec3 translation(m_transf.m_translation[0],m_transf.m_translation[1],m_transf.m_translation[2]-14);
		jointActor->setGlobalPosition(translation);

		
		//Transformations
		/*m_Sphere->SetTranslate( NiPoint3(tx,ty,tz) );
		if (AMCRotationType == ROT_YZX){

			NiMatrix3 rot = NiMatrix3::IDENTITY;
			rot.FromEulerAnglesYZX(Deg2Rad(pJoint->m_rotation[VY]),Deg2Rad(pJoint->m_rotation[VZ]),Deg2Rad(pJoint->m_rotation[VX]));
			m_Sphere->SetRotate(rot);
		}	*/

		// Draw children joints
		for (vector<Joint*>::const_iterator iter = pJoint->m_pChildren.begin(); iter != pJoint->m_pChildren.end(); ++iter)
		{
			DrawActorRec(player, *iter);
		}
	}
}

void 
PlayerDisplay::DrawActorRec(Player *player, Joint *pJoint){
	Skeleton* pSkeleton = player->GetSkeleton();

	isChild = false;
	float tx = pJoint->m_translation[VX];
	float ty = pJoint->m_translation[VY];
	float tz = pJoint->m_translation[VZ];

	

	// allocate space for a zero-terminated copy of the string
	char *charPtrString = new char[pJoint->m_name.size()+1];

	// copy the string
	std::strcpy(charPtrString, pJoint->m_name.c_str());
	
	NiFixedString jointName = charPtrString;

	if(scene->GetObjectByName(charPtrString) != NULL){
		NiAVObject* m_Sphere = (NiAVObject*)scene->GetObjectByName(charPtrString);
		
		int index = actorSkeleton[charPtrString];

		NxActor* jointActor = ((NiPhysXRigidBodyDest*)spPlayerProp->GetDestinationAt(index))->GetActor();
		pJoint->UpdateTransformation(true);
		//vec3 m_transf = pSkeleton->ComputeJointGlobalPosition(root);
		
		Transform m_transf = pJoint->m_global;
		
		
		NxVec3 translation(m_transf.m_translation[0],m_transf.m_translation[1],m_transf.m_translation[2]-14);
		jointActor->setGlobalPosition(translation);
		
		

		
		//Transformations
		/*
		
		if (AMCRotationType == ROT_YZX){
			NiMatrix3 rot = NiMatrix3::IDENTITY;
			rot.FromEulerAnglesYZX(Deg2Rad(pJoint->m_rotation[VY]),Deg2Rad(pJoint->m_rotation[VZ]),Deg2Rad(pJoint->m_rotation[VX]));
			m_Sphere->SetRotate(rot);
		}*/
		
		
		// Recursion
		// Draw children joints
		for (vector<Joint*>::const_iterator iter = pJoint->m_pChildren.begin(); iter != pJoint->m_pChildren.end(); ++iter)
		{
			
			DrawActorRec(player, *iter);
			
		}
	}	
}

void 
PlayerDisplay::processKeyboard(Keyboard* keyboard){
	if (keyboard->KeyIsDown(NiInputKeyboard::KEY_SPACE)){
		playing = true;
	}
	else if (keyboard->KeyIsDown(NiInputKeyboard::KEY_Q)){
		m_Player->LoadMotion("C:/Users/Tammy/Documents/School/cis499/Flex_code/EMG/Bin/Database/Motion/Boxing/female.Punch.amc");
		m_Player->NormalizeMotionToFloorHeight("C:/Users/Tammy/Documents/School/cis499/Flex_code/EMG/Bin/Database/Motion/Boxing/female.Punch.amc",0.0);
	}
	else if (keyboard->KeyIsDown(NiInputKeyboard::KEY_W)){
		m_Player->LoadMotion("C:/Users/Tammy/Documents/School/cis499/Flex_code/EMG/Bin/Database/Motion/Duck/duck_0.amc");
		m_Player->NormalizeMotionToFloorHeight("C:/Users/Tammy/Documents/School/cis499/Flex_code/EMG/Bin/Database/Motion/Duck/duck_0.amc",0.0);
	}
	else if (keyboard->KeyIsDown(NiInputKeyboard::KEY_E)){
		m_Player->LoadMotion("C:/Users/Tammy/Documents/School/cis499/Flex_code/EMG/Bin/Database/Motion/CMU_Jump/MixJump.amc");
		m_Player->NormalizeMotionToFloorHeight("C:/Users/Tammy/Documents/School/cis499/Flex_code/EMG/Bin/Database/Motion/CMU_Jump/MixJump.amc",0.0);
	}
	else if (keyboard->KeyIsDown(NiInputKeyboard::KEY_R)){
		m_Player->LoadMotion("C:/Users/Tammy/Documents/School/cis499/Flex_code/EMG/Bin/Database/Motion/MartialArts/male_frontkick.amc");
		m_Player->NormalizeMotionToFloorHeight("C:/Users/Tammy/Documents/School/cis499/Flex_code/EMG/Bin/Database/Motion/MartialArts/male_frontkick.amc",0.0);
	}

}


