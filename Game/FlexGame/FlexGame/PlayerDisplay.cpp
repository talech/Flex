#include "PlayerDisplay.h"
#include <CommonTypes.h>

PlayerDisplay::PlayerDisplay(Player* pPlayer, NiNodePtr sScene){
	scene = sScene;
	m_Player = pPlayer;
}

PlayerDisplay::~PlayerDisplay(){
}

void
PlayerDisplay::Update(){
	if(m_Player)
		DrawPlayer(m_Player);
}

void 
PlayerDisplay::DrawPlayer(Player* player){
	Skeleton* pSkeleton = player->GetSkeleton();

	if( pSkeleton && pSkeleton->m_pJoints.size() > 0 ){
		//Draw Root Joint as a sphere
		Joint* pJoint = pSkeleton->GetJointByIndex(root);

		float tx = pJoint->m_translation[VX]*10;
		float ty = pJoint->m_translation[VY]*10;
		float tz = pJoint->m_translation[VZ]*10;

		NiAVObject* m_Sphere = (NiAVObject*)scene->GetObjectByName("root");
		

		//Transformations
		m_Sphere->SetTranslate( NiPoint3(tx,ty,tz) );
		if (AMCRotationType == ROT_YZX){

			NiMatrix3 rot = NiMatrix3::IDENTITY;
			rot.FromEulerAnglesYZX(Deg2Rad(pJoint->m_rotation[VY]),Deg2Rad(pJoint->m_rotation[VZ]),Deg2Rad(pJoint->m_rotation[VX]));
			m_Sphere->SetRotate(rot);

			/*if(ISLOCKED_MASK(pJoint->m_DOF, DOF_Y)) 
				glRotatef(pJoint->m_rotation[VY], 0.0f, 1.0f,  0.0f);
			if(ISLOCKED_MASK(pJoint->m_DOF, DOF_Z)) 
				glRotatef(pJoint->m_rotation[VZ], 0.0f, 0.0f,  1.0f);
			if(ISLOCKED_MASK(pJoint->m_DOF, DOF_X))
				glRotatef(pJoint->m_rotation[VX], 1.0f, 0.0f,  0.0f);*/

		}
		//scene->AttachChild(m_Sphere);
		
		

		// Draw children joints
		for (vector<Joint*>::const_iterator iter = pJoint->m_pChildren.begin(); iter != pJoint->m_pChildren.end(); ++iter)
		{
			DrawActorRec(player, *iter);
		}

		m_Sphere = (NiAVObject*)scene->GetObjectByName("root");
		m_Sphere->SetTranslate( NiPoint3(m_Sphere->GetTranslate().x,m_Sphere->GetTranslate().y-8,m_Sphere->GetTranslate().z) );


	}
}

void 
PlayerDisplay::DrawActorRec(Player *player, Joint *pJoint){

	isChild = false;
	float tx = pJoint->m_translation[VX]*10;
	float ty = pJoint->m_translation[VY]*10;
	float tz = pJoint->m_translation[VZ]*10;

	

	// allocate space for a zero-terminated copy of the string
	char *charPtrString = new char[pJoint->m_name.size()+1];

	// copy the string
	std::strcpy(charPtrString, pJoint->m_name.c_str());
	
	NiFixedString jointName = charPtrString;

	if(scene->GetObjectByName(charPtrString) != NULL){
		NiAVObject* m_Sphere = (NiAVObject*)scene->GetObjectByName(charPtrString);
		//m_Sphere->SetName( jointName );

		//Transformations
		m_Sphere->SetTranslate( NiPoint3(tx,ty,tz) );
		
		if (AMCRotationType == ROT_YZX){
			NiMatrix3 rot = NiMatrix3::IDENTITY;
			rot.FromEulerAnglesYZX(Deg2Rad(pJoint->m_rotation[VY]),Deg2Rad(pJoint->m_rotation[VZ]),Deg2Rad(pJoint->m_rotation[VX]));
			m_Sphere->SetRotate(rot);

			/*if(ISLOCKED_MASK(pJoint->m_DOF, DOF_Y)) 
				glRotatef(pJoint->m_rotation[VY], 0.0f, 1.0f,  0.0f);
			if(ISLOCKED_MASK(pJoint->m_DOF, DOF_Z)) 
				glRotatef(pJoint->m_rotation[VZ], 0.0f, 0.0f,  1.0f);
			if(ISLOCKED_MASK(pJoint->m_DOF, DOF_X))
				glRotatef(pJoint->m_rotation[VX], 1.0f, 0.0f,  0.0f);*/
		}
		
		
		//scene->AttachChild(m_Sphere);
		
		// Recursion
		// Draw children joints
		for (vector<Joint*>::const_iterator iter = pJoint->m_pChildren.begin(); iter != pJoint->m_pChildren.end(); ++iter)
		{
			
			DrawActorRec(player, *iter);
			
		}
	}

	
		

}


