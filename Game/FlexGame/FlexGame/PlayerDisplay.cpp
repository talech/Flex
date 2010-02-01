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

		NiAVObject* m_Sphere2 = (NiAVObject*)scene->GetObjectByName("Sphere");
		m_Sphere2->SetTranslate( NiPoint3(-1000,-1000,-1000) );

		NiAVObject* m_Sphere = (NiAVObject*)scene->GetObjectByName("Sphere")->Clone();
		m_Sphere->SetName("Root");

		//Transformations
		m_Sphere->SetTranslate( NiPoint3(pJoint->m_translation[VX],pJoint->m_translation[VY],pJoint->m_translation[VZ]) );
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



	}
}


