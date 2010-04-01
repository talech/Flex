#include "PlayerDisplay.h"
#include <CommonTypes.h>

PlayerDisplay::PlayerDisplay(Player* pPlayer, NiNodePtr sScene, NiPhysXScenePtr physScene){
	scene = sScene;
	m_Player = pPlayer;
	spPlayerProp = physScene->GetPropAt(1);
	playing = false; 

	m_Player->NormalizeMotionToFloorHeight("C:/Users/Tammy/Documents/School/cis499/Flex_code/EMG/Bin/Database/Motion/Boxing/female.Punch.amc",0.5);
	m_Player->NormalizeMotionToFloorHeight("C:/Users/Tammy/Documents/School/cis499/Flex_code/EMG/Bin/Database/Motion/Duck/duck_0.amc",0.5);
	m_Player->NormalizeMotionToFloorHeight("C:/Users/Tammy/Documents/School/cis499/Flex_code/EMG/Bin/Database/Motion/CMU_Jump/MixJump.amc",0.5);
	m_Player->NormalizeMotionToFloorHeight("C:/Users/Tammy/Documents/School/cis499/Flex_code/EMG/Bin/Database/Motion/MartialArts/male_frontkick.amc",0.5);

	Motion* m0 = new Motion("C:/Users/Tammy/Documents/School/cis499/Flex_code/EMG/Bin/Database/Motion/Boxing/female.Punch.amc");
	Motion* m1 = new Motion("C:/Users/Tammy/Documents/School/cis499/Flex_code/EMG/Bin/Database/Motion/Duck/duck_0.amc");
	Motion* m2 = new Motion("C:/Users/Tammy/Documents/School/cis499/Flex_code/EMG/Bin/Database/Motion/CMU_Jump/MixJump.amc");
	Motion* m3 = new Motion("C:/Users/Tammy/Documents/School/cis499/Flex_code/EMG/Bin/Database/Motion/MartialArts/male_frontkick.amc");

	motions[0] = m0;
	motions[1] = m1;
	motions[2] = m2;
	motions[3] = m3;

	m_Player->SetMotion(motions[3]);
	m_Player->m_frameIndex = m_Player->m_startFrame;

	
	
	
}

PlayerDisplay::~PlayerDisplay(){
}

void
PlayerDisplay::Update(){
	//Live Data Stream, use this if getting real-time data
	/*if(client.Tick()&& playing)
		DataStream();

	if(m_Player && playing)
		DrawMarkers();*/
	

	//Update frame to next frame if playing
	//Motion from file, use this for testing any changes if not hooked up to the network
	if(m_Player && playing){
		int index = m_Player->GetNextFrameIndex(m_Player->m_frameIndex);
		if(index == m_Player->m_endFrame) index = 0;
		
		m_Player->m_frameIndex = index;
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
		m_Player->SetMotion(motions[0]);
		m_Player->m_frameIndex = m_Player->m_startFrame;
	}
	else if (keyboard->KeyIsDown(NiInputKeyboard::KEY_W)){
		m_Player->SetMotion(motions[1]);
		m_Player->m_frameIndex = m_Player->m_startFrame;
	}
	else if (keyboard->KeyIsDown(NiInputKeyboard::KEY_E)){
		m_Player->SetMotion(motions[2]);
		m_Player->m_frameIndex = m_Player->m_startFrame;
	}
	else if (keyboard->KeyIsDown(NiInputKeyboard::KEY_R)){
		m_Player->SetMotion(motions[3]);
		m_Player->m_frameIndex = m_Player->m_startFrame;
	}
}



void 
PlayerDisplay::DataStream(){
	/*int numSubjects = client.GetSubjectCount();
	for (int subject = 0; subject < numSubjects; subject++)
	{
		std::string subjectName = client.GetSubjectName(subject);
		int numMarkers = client.GetMarkerCount(subjectName);
		for (int marker = 0; marker < numMarkers; marker++)
		{
			vec3 position;
			std::string markerName;
			std::string markerParentName;
			client.GetMarkerData(subjectName, marker, markerName, markerParentName, position);
			if(markerName == "LFHD"||markerName == "RFHD"||markerName == "LBHD"||markerName == "RBHD")
				markersHead[markerName] = position;
			else
				markers[markerName] = position;
		}
	}*/
}

void
PlayerDisplay::DrawMarkers(){
	//std::map<std::string,vec3>::const_iterator it;
	//int index = 0;
	//for (it = markers.begin(); it != markers.end(); ++it)
	//{
	//	std::string name = it->first;
	//	vec3 pos = it->second;

	//	if(index < spPlayerProp->GetDestinationsCount()-1){
	//		NxActor* jointActor = ((NiPhysXRigidBodyDest*)spPlayerProp->GetDestinationAt(index))->GetActor();
	//		//scale to my world coordinates
	//		NxVec3 translation((pos[0]/1000.0),(pos[1]/1000.0)+0.5,(pos[2]/1000.0)-14);
	//		jointActor->setGlobalPosition(translation);
	//	}
	//	index++;

	//}

	////make sure no unused markers are in the way 
	//while(index < spPlayerProp->GetDestinationsCount()-1){
	//	NxActor* jointActor = ((NiPhysXRigidBodyDest*)spPlayerProp->GetDestinationAt(index))->GetActor();
	//	NxVec3 translation(0,-10,0);
	//	jointActor->setGlobalPosition(translation);
	//	index++;
	//}

	////draw head
	//float x = 0; float y = 0; float z = 0;
	//map<string,vec3>::const_iterator itH;
	//for (itH = markersHead.begin(); itH != markersHead.end(); ++itH)
	//{
	//	x += itH->second[0];
	//	y += itH->second[1];
	//	z += itH->second[2];
	//}
	//x = x/4.0; y = y/4.0; z = z/4.0;
	//index = spPlayerProp->GetDestinationsCount()-1;
	//NxActor* jointActor = ((NiPhysXRigidBodyDest*)spPlayerProp->GetDestinationAt(index))->GetActor();
	////scale to my world coordinates
	//NxVec3 translation((x/1000.0),(y/1000.0)+0.5,(z/1000.0)+14);
	//jointActor->setGlobalPosition(translation);


}


	
		
		
		
			
