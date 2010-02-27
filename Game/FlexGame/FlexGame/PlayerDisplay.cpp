#include "PlayerDisplay.h"
#include <CommonTypes.h>

PlayerDisplay::PlayerDisplay(Player* pPlayer, NiNodePtr sScene, NiPhysXScenePtr physScene){
	scene = sScene;
	m_Player = pPlayer;
	spPlayerProp = physScene->GetPropAt(1);
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

		float tx = pJoint->m_translation[VX];
		float ty = pJoint->m_translation[VY];
		float tz = pJoint->m_translation[VZ];

		NiAVObject* m_Sphere = (NiAVObject*)scene->GetObjectByName("root");
		

		//Transformations
		m_Sphere->SetTranslate( NiPoint3(tx,ty,tz) );
		if (AMCRotationType == ROT_YZX){

			NiMatrix3 rot = NiMatrix3::IDENTITY;
			rot.FromEulerAnglesYZX(Deg2Rad(pJoint->m_rotation[VY]),Deg2Rad(pJoint->m_rotation[VZ]),Deg2Rad(pJoint->m_rotation[VX]));
			m_Sphere->SetRotate(rot);
		}	

		// Draw children joints
		for (vector<Joint*>::const_iterator iter = pJoint->m_pChildren.begin(); iter != pJoint->m_pChildren.end(); ++iter)
		{
			DrawActorRec(player, *iter);
		}

		m_Sphere = (NiAVObject*)scene->GetObjectByName("root");
		m_Sphere->SetTranslate( NiPoint3(m_Sphere->GetTranslate().x,m_Sphere->GetTranslate().y,m_Sphere->GetTranslate().z) );


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
		}
		
		

		
		// Recursion
		// Draw children joints
		for (vector<Joint*>::const_iterator iter = pJoint->m_pChildren.begin(); iter != pJoint->m_pChildren.end(); ++iter)
		{
			
			DrawActorRec(player, *iter);
			
		}
	}

	
		

}


