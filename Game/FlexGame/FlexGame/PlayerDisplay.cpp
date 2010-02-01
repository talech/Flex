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
		float ty = 4 + pJoint->m_translation[VY]*10;
		float tz = pJoint->m_translation[VZ]*10;

		NiAVObject* m_Sphere = (NiAVObject*)scene->GetObjectByName("Sphere")->Clone();
		m_Sphere->SetName("Root");

		//Transformations
		m_Sphere->SetTranslate( NiPoint3(tx,ty,tz) );
		if (AMCRotationType == ROT_YZX){

			NiMatrix3 rotMat;
			NiPoint3 vx, vy, vz;

			/*if(ISLOCKED_MASK(pJoint->m_DOF, DOF_Y)) 
				glRotatef(pJoint->m_rotation[VY], 0.0f, 1.0f,  0.0f);
			if(ISLOCKED_MASK(pJoint->m_DOF, DOF_Z)) 
				glRotatef(pJoint->m_rotation[VZ], 0.0f, 0.0f,  1.0f);
			if(ISLOCKED_MASK(pJoint->m_DOF, DOF_X))
				glRotatef(pJoint->m_rotation[VX], 1.0f, 0.0f,  0.0f);*/

		}
		scene->AttachChild(m_Sphere);

		// Draw children joints
		for (vector<Joint*>::const_iterator iter = pJoint->m_pChildren.begin(); iter != pJoint->m_pChildren.end(); ++iter)
		{
			DrawActorRec(player, *iter);
		}



	}
}

void 
PlayerDisplay::DrawActorRec(Player *player, Joint *pJoint){

	float tx = pJoint->m_translation[VX]*10;
	float ty = 4 + pJoint->m_translation[VY]*10;
	float tz = pJoint->m_translation[VZ]*10;

	NiAVObject* m_Sphere = (NiAVObject*)scene->GetObjectByName("Sphere")->Clone();

	// allocate space for a zero-terminated copy of the string
	char *charPtrString = new char[pJoint->m_name.size()+1];

	// copy the string
	std::strcpy(charPtrString, pJoint->m_name.c_str());
	
	NiFixedString jointName = charPtrString;
	m_Sphere->SetName( jointName );

	//Transformations
	m_Sphere->SetTranslate( NiPoint3(tx,ty,tz) );
	if (AMCRotationType == ROT_YZX){
		NiMatrix3 rotMat;
		NiPoint3 vx, vy, vz;

		/*if(ISLOCKED_MASK(pJoint->m_DOF, DOF_Y)) 
			glRotatef(pJoint->m_rotation[VY], 0.0f, 1.0f,  0.0f);
		if(ISLOCKED_MASK(pJoint->m_DOF, DOF_Z)) 
			glRotatef(pJoint->m_rotation[VZ], 0.0f, 0.0f,  1.0f);
		if(ISLOCKED_MASK(pJoint->m_DOF, DOF_X))
			glRotatef(pJoint->m_rotation[VX], 1.0f, 0.0f,  0.0f);*/
	}
	scene->AttachChild(m_Sphere);

	// Recursion
	// Draw children joints
	for (vector<Joint*>::const_iterator iter = pJoint->m_pChildren.begin(); iter != pJoint->m_pChildren.end(); ++iter)
	{
		DrawActorRec(player, *iter);
	}
		

}


