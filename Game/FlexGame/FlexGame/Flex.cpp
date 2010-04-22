#include <NiMain.h>
#include <NiAnimation.h>
#include <NiLicense.h>
#include <NiUIManager.h>

#include "GameStateManager.h"
#include "GameState.h"
#include "RunningState.h"
#include "InitializeState.h"
#include "LoadState.h"
#include "NewGameMenu.h"
#include "WallMoving.h"


NiEmbedGamebryoLicenseCode;

#include "Flex.h"


//---------------------------------------------------------------------------
NiApplication* NiApplication::Create()
{
    return NiNew Flex;
}
//---------------------------------------------------------------------------

Flex::Flex() : NiApplication("Flex",
    DEFAULT_WIDTH, DEFAULT_HEIGHT, true),
	m_PhysContactReporter()
{
    SetMediaPath("../Data/");  
	m_PhysContactReporter.m_app = this;

	// Initialize smart pointers to zero. In the case of early termination
    // this avoid errors.
    m_spPhysScene = 0;

	GameStateManager::getInstance()->addApplication(this);
	GameStateManager::getInstance()->start(RunningState::getInstance());
}
//---------------------------------------------------------------------------

Flex::~Flex(){
}
//---------------------------------------------------------------------------
bool 
Flex::Initialize(){
	GameStateManager::getInstance()->pushState(InitializeState::getInstance());

    // Save a pointer to the PhysXSDKManager object. This is Emergent's
    // manager for all PhysX global functionality. The file
    // <efdPhysX/PhysXSDKManager.h> must be included for this object to exist.
    m_pkPhysManager = efdPhysX::PhysXSDKManager::GetManager();
    
    // Try to initialize the PhysX SDK. By default, we are setting up
    // Gamebryo's memory manager and debug output interfaces for use
    // with PhysX.
    if (!m_pkPhysManager->Initialize())
    {
        char acMsg[1024];
        NiSprintf(acMsg, 1024,
            "Unable to initialize PhysX SDK version %d.\n"
            "This may mean you don't have PhysX installed.\n"
            "Have you installed PhysX System Software and Core?\n",
            NX_SDK_VERSION_NUMBER);
        NiMessageBox(acMsg,
            "PhysX Initialization Failed");
        return false;
    }
        
    // The manager contains a public pointer to the PhysX SDK object,
    // m_pkPhysXSDK. Here we use it to set some global SDK parameters.
    // See the PhysX documentation for an explanation of these settings.
    m_pkPhysManager->m_pPhysXSDK->setParameter(NX_SKIN_WIDTH, 0.01f);
    m_pkPhysManager->m_pPhysXSDK->setParameter(NX_BOUNCE_THRESHOLD, -2.0f);
    m_pkPhysManager->m_pPhysXSDK->setParameter(NX_VISUALIZATION_SCALE, 1.0f);
    m_pkPhysManager->m_pPhysXSDK->setParameter(NX_VISUALIZE_BODY_AXES, 1.0f);
    m_pkPhysManager->m_pPhysXSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1.0f);
    m_pkPhysManager->m_pPhysXSDK->setParameter(NX_VISUALIZE_ACTOR_AXES, 0.5);
    m_pkPhysManager->m_pPhysXSDK->setParameter(NX_VISUALIZE_BODY_LIN_VELOCITY, 0.5);

    if (!NiApplication::Initialize())
        return false;
            
	pkKeyboard = NULL;

    // Update the scene graph before rendering begins.
    m_spScene->UpdateProperties();
    m_spScene->UpdateEffects();
    m_spScene->Update(0.0f);

	// Get simulation started. We give a small timestep to avoid zero length
    // steps
    m_spPhysScene->UpdateSources(0.001f);
    m_spPhysScene->Simulate(0.001f);
    fLastSimTime = 0.001f;

	// Setup the GameStateManger, add the PhysManager, the physScene, and the scene.
	GameStateManager::getInstance()->physManager = m_pkPhysManager;
	GameStateManager::getInstance()->physScene = m_spPhysScene;
	GameStateManager::getInstance()->scene = m_spScene;
	GameStateManager::getInstance()->changeState(NewGameMenu::getInstance());
    
    return true;
}
//---------------------------------------------------------------------------
void Flex::Terminate()
{
    m_spPhysScene = 0;
	m_pCameraController = 0;
	m_spScene = 0;
	pkAccum = 0;
	m_pkPhysManager->Shutdown();
	NiApplication::Terminate();    

}
//---------------------------------------------------------------------------
bool 
Flex::CreateScene(){
	GameStateManager::getInstance()->pushState(LoadState::getInstance());

	if(!NiApplication::CreateScene())
        return false;

	pkAccum = NiNew NiAlphaAccumulator;
   

	if (!InitPhysics())
    {
        return false;
    }

    if (!InitEnvironment())
    {
        NiMessageBox("Cannot initialize environment!", "Flex Error");
        return false;
    }

    if (!InitCamera())
    {
        NiMessageBox("The NIF file has no camera!", "Camera Error");
        return false;
    }

	if (!InitPlayer())
    {
        NiMessageBox("Cannot initialize Motions!", "Player Error");
        return false;
    }

	if (!ScreenOverlay::Create(this))
    {
		NiMessageBox("Cannot initialize overlay!", "Flex Error");
        return false;
    }

	

	GameStateManager::getInstance()->popState();

	return true;
}
//---------------------------------------------------------------------------
bool 
Flex::CreateRenderer(){
	NiColor kColor(0.5f, 0.6f, 1.0f);
    bool success = NiApplication::CreateRenderer();
    if (success) m_spRenderer->SetBackgroundColor(kColor);
    return success;
}
//---------------------------------------------------------------------------
bool
Flex::InitPlayer(){
	/*m_pPlayer = new Player();
	m_pPlayer->LoadSkeleton("C:/Users/Tammy/Documents/School/cis499/Flex_code/EMG/Bin/Actor.asf");
	m_pPlayer->NormalizeMotionToFloorHeight("C:/Users/Tammy/Documents/School/cis499/Flex_code/EMG/Bin/Database/Motion/MartialArts/male_frontkick.amc",0.5);
	m_pPlayer->LoadMotion("C:/Users/Tammy/Documents/School/cis499/Flex_code/EMG/Bin/Database/Motion/MartialArts/male_frontkick.amc");*/
    m_playerDisplay = new PlayerDisplay(NULL,m_spScene, m_spPhysScene);
	m_playerDisplay->actorSkeleton = this->actorSkeleton;
	//totalFrame = m_pPlayer->GetTotalFrameCount();
	return true;
}
//---------------------------------------------------------------------------
bool 
Flex::InitPhysics(){
	// We first create a PhysX scene and the Gamebryo wrapper for it. First
    // the wrapper.
    m_spPhysScene = NiNew NiPhysXScene();

	// We are doing asynchronous simulation, and the scene wrapper must be
    // informed.
    m_spPhysScene->SetAsynchronousSimulation(false);
    
    // Then the PhysX scene
    NxSceneDesc kSceneDesc;
    kSceneDesc.gravity.set(0.0f, -50.0f, 0.0f);
    NxScene* pkScene = m_pkPhysManager->m_pPhysXSDK->createScene(kSceneDesc);
    NIASSERT(pkScene != 0);
	pkScene->setUserContactReport(&m_PhysContactReporter);
	
    
    // Attach the physics scene to the wrapper
    m_spPhysScene->SetPhysXScene(pkScene);
	m_spPhysScene->SetUpdateDest(true);
	m_spPhysScene->SetUpdateSrc(true);
    return true;
}
//---------------------------------------------------------------------------
bool
Flex::InitEnvironment(){
	NiStream kStream;
    if (!kStream.Load(ConvertMediaFilename("Environment2.nif")))
	{
        NIASSERT(0 && "Couldn't load nif file\n");
        NiMessageBox("Could not load Wall1.nif. Aborting\n",
            "Missing nif file.");
 
        return false;
    }
    m_spScene = (NiNode*) kStream.GetObjectAt(0);
    NIASSERT(NiIsKindOf(NiNode, m_spScene));

	NiPhysXPropPtr physEnvironment;
    if (!AddPhysicsProps(kStream, physEnvironment))
    {
        NiMessageBox("The NIF file has no physics objects!", "Warning");        
    }

	// Repeat the process with the player.
    kStream.RemoveAllObjects();
    if (!kStream.Load(ConvertMediaFilename("Skeleton2.nif")))
    {
        NIASSERT(0 && "Couldn't load nif file\n");
        NiMessageBox("Could not load Player.nif. Aborting\n",
            "Missing nif file.");
 
        return false;
    }

    // We know that this NIF file has the player at location 0. Attach the
    // player to the scene graph.
    NIASSERT(NiIsKindOf(NiAVObject, kStream.GetObjectAt(0)));
    m_spScene->AttachChild((NiAVObject*)kStream.GetObjectAt(0));

	NiPhysXPropPtr physPlayer;
    if (!AddPhysicsProps(kStream, physPlayer))
    {
        NiMessageBox("The NIF file has no physics objects!", "Warning");        
    }

	// Repeat the process with the walls
	 kStream.RemoveAllObjects();
	 NiPhysXPropPtr physWalls;
    if (!InitWalls(kStream, physWalls))
    {
        NiMessageBox("The Walls didn't load!", "Warning");        
    }

	NiAVObject* player = m_spScene->GetObjectByName("Bally1");
    for (int i = 0; i < physPlayer->GetDestinationsCount(); i++)
    {
        NiPhysXDest* src = physPlayer->GetDestinationAt(1);
        if (NiIsKindOf(NiPhysXTransformDest, src))
        {
            NiPhysXTransformDest* s = (NiPhysXTransformDest*) src;
            m_player.push_back(s->GetActor());
			
        }
    }

	InitCollisionCallbacks();
    SetWallPhysicsEnabled(false, true);
	setSkeletonMap();
    return true;
	

}
//---------------------------------------------------------------------------
bool
Flex::AddPhysicsProps(NiStream &kStream, NiPhysXPropPtr &propPtr){
	// Look for the PhysX content. In this case, the PhysX
    // content is the Flex scene.
    NiPhysXPropPtr prop = 0;
    for (unsigned int ui = 1; ui < kStream.GetObjectCount(); ui++)
    {
        if (NiIsKindOf(NiPhysXProp, kStream.GetObjectAt(ui)))
        {
            // We have found the PhysX content in the NIF.
            prop = (NiPhysXProp*)kStream.GetObjectAt(ui);
            m_spPhysScene->AddProp(prop);
            propPtr = prop;
        }
    }
    return (prop != 0); // We should have found at least one

}
//---------------------------------------------------------------------------
bool 
Flex::InitWalls(NiStream &kStream, NiPhysXPropPtr& propPtr){

	for(int i = 1; i<NUM_WALLS+1; i++){
	
		char name[128];
		sprintf(name, "Walls/Wall%d.nif", i);

		if (!kStream.Load(ConvertMediaFilename(name)))
		{
			NIASSERT(0 && "Couldn't load nif file\n");
			NiMessageBox("Could not load Wall1.nif. Aborting\n",
				"Missing nif file.");
	 
			return false;
		}

		// We know that this NIF file has the wall at location 0. Attach the
		// wall to the scene graph.
		NIASSERT(NiIsKindOf(NiAVObject, kStream.GetObjectAt(0)));
		m_spScene->AttachChild((NiAVObject*)kStream.GetObjectAt(0));

		if (!AddPhysicsProps(kStream, propPtr))
		{
			NiMessageBox("The NIF file has no physics objects!", "Warning");        
		}
	}


	//init red block = collision block
	if (!kStream.Load(ConvertMediaFilename("redBlock.nif")))
	{
		NIASSERT(0 && "Couldn't load nif file\n");
		NiMessageBox("Could not load Wall1.nif. Aborting\n",
			"Missing nif file.");
	
		return false;
	}

	// We know that this NIF file has the wall at location 0. Attach the
	// wall to the scene graph.
	NIASSERT(NiIsKindOf(NiAVObject, kStream.GetObjectAt(0)));
	m_spScene->AttachChild((NiAVObject*)kStream.GetObjectAt(0));

	if (!AddPhysicsProps(kStream, propPtr))
	{
		NiMessageBox("The NIF file has no physics objects!", "Warning");        
	}
	redBlock = ((NiPhysXRigidBodyDest*)propPtr->GetDestinationAt(0))->GetActor();
	//no physX for the red cube
	redBlock->raiseBodyFlag(NX_BF_FROZEN);
	redBlock->setGlobalPosition( NxVec3(0,-100,0) );

	return true;
}
//---------------------------------------------------------------------------
bool
Flex::InitCamera(){
	if (m_spScene)
    {
        m_spCamera = FindFirstCamera(m_spScene);
		//m_spCamera->LookAtWorldPoint(NiPoint3(0,0,0), NiPoint3(0,1,0));
  //      m_spCamera->SetTranslate(NiPoint3::ZERO); // Needed to initialize orbit navigation   

		
    }

	/*NiAVObject* cameraRoot = m_spScene->GetObjectByName("camera1");
	m_pCameraController = NiNew CameraController(this, cameraRoot);*/



    return (m_spCamera != NULL);

}

//---------------------------------------------------------------------------

void
Flex::UpdateFrame(){
	
	m_spPhysScene->UpdateSources(m_fAccumTime);
    m_spPhysScene->Simulate(m_fAccumTime);
    m_spPhysScene->FetchResults(m_fAccumTime, true);
    m_spPhysScene->UpdateDestinations(m_fAccumTime);
    //m_pCameraController->Update(m_fAccumTime);

	NiApplication::UpdateFrame(); // Calls process input
    m_spScene->Update(m_fAccumTime); // update the scene graph.

	fLastSimTime = m_fAccumTime;

	// For debugging: toggle physics viz
	if (GetInputSystem()->GetKeyboard()->KeyWasPressed(NiInputKeyboard::KEY_P))
	{
		bool d = m_spPhysScene->GetDebugRender();
	    m_spPhysScene->SetDebugRender(!d, m_spScene);
	}
	if(GameStateManager::getInstance()->collision == true){
		followCube();
	}
	// update the playerDisplay
	m_playerDisplay->Update();
	GameStateManager::getInstance()->update(m_fAccumTime);    

}
//---------------------------------------------------------------------------
void
Flex::followCube(){
	redBlock->setGlobalPosition(collideCube->getGlobalPosition());
}
//---------------------------------------------------------------------------
void 
Flex::RenderFrame(){
    m_spRenderer->SetSorter(pkAccum);

    NiApplication::RenderFrame();
    ScreenOverlay::Get()->Draw();
}
//---------------------------------------------------------------------------
void 
Flex::processContacts(NxContactPair& pair, NxU32 events){
    NxActor* a1 = pair.actors[0];
    NxActor* a2 = pair.actors[1];
	
	NxVec3 normalForce = pair.sumNormalForce;

    NILOG(NIMESSAGE_GENERAL_0, "CONTACT: a1: %s a2: %s (%f)\n", 
            a1->getName(), a2->getName(), normalForce.magnitude());

	if(GameStateManager::getInstance()->collision == false){
		collideCube = a1;
		GameStateManager::getInstance()->collision = true;
	}
	
    SetWallPhysicsEnabled(true);
}
//---------------------------------------------------------------------------
void
Flex::toggleEnableWall(bool enable){
	NiPhysXProp* phys = m_spPhysScene->GetPropAt(GameStateManager::getInstance()->currentWall);
	for (int i = 0; i < phys->GetDestinationsCount(); i++)
	{
		NiPhysXDest* src = phys->GetDestinationAt(i);
		if (NiIsKindOf(NiPhysXTransformDest, src))
		{
			NiPhysXTransformDest* s = (NiPhysXTransformDest*) src;
				if(enable)s->GetActor()->clearActorFlag(NX_AF_DISABLE_COLLISION);
				else s->GetActor()->raiseActorFlag(NX_AF_DISABLE_COLLISION);
			}
	}
}
//---------------------------------------------------------------------------
void
Flex::ResetWallPhysics(){
	SetWallPhysicsEnabled(false);
	m_spPhysScene->RestoreSnapshotState(1);
	GameStateManager::getInstance()->collision = false;
	//hide red block
	redBlock->setGlobalPosition( NxVec3(0,-100,0) );
	
	
}
//---------------------------------------------------------------------------
void 
Flex::SetWallPhysicsEnabled(bool b, bool force){
    if (force || b != m_wallPhysicsEnabled)
    {
		//for collision
		if(GameStateManager::getInstance()->collision)
		{
			NiPhysXProp* phys = m_spPhysScene->GetPropAt(GameStateManager::getInstance()->currentWall);
				for (int i = 0; i < phys->GetDestinationsCount(); i++)
				{
					NiPhysXDest* src = phys->GetDestinationAt(i);
					if (NiIsKindOf(NiPhysXTransformDest, src))
					{
						NiPhysXTransformDest* s = (NiPhysXTransformDest*) src;
						if(b)s->GetActor()->clearBodyFlag(NX_BF_FROZEN);
						else s->GetActor()->raiseBodyFlag(NX_BF_FROZEN);
					}
				}
		}
		//for initialization 
		else{
			for(int j = 2; j<NUM_WALLS+2; j++)
			{
				NiPhysXProp* phys = m_spPhysScene->GetPropAt(j);
				for (int i = 0; i < phys->GetDestinationsCount(); i++)
				{
					NiPhysXDest* src = phys->GetDestinationAt(i);
					if (NiIsKindOf(NiPhysXTransformDest, src))
					{
						NiPhysXTransformDest* s = (NiPhysXTransformDest*) src;
						//if (findPlayerActor(s->GetActor())) s->GetActor()->raiseBodyFlag(NX_BF_FROZEN);//continue;

						if (b) s->GetActor()->clearBodyFlag(NX_BF_FROZEN);
						else{ 
							s->GetActor()->raiseBodyFlag(NX_BF_FROZEN);
							NxVec3 pos = s->GetActor()->getGlobalPosition();
							pos[2] = -25;
							s->GetActor()->setGlobalPosition(pos);
							s->GetActor()->raiseActorFlag(NX_AF_DISABLE_COLLISION);
						}
					}
				}
			}
		}
		b = m_wallPhysicsEnabled;
	}
	if(force && !GameStateManager::getInstance()->collision){
		NiPhysXProp* phys = m_spPhysScene->GetPropAt(1);
        for (int i = 0; i < phys->GetDestinationsCount(); i++)
        {
            NiPhysXDest* src = phys->GetDestinationAt(i);
            if (NiIsKindOf(NiPhysXTransformDest, src))
            {
                NiPhysXTransformDest* s = (NiPhysXTransformDest*) src;
                s->GetActor()->raiseBodyFlag(NX_BF_FROZEN);
            }
		}
	}
}
//---------------------------------------------------------------------------
void 
Flex::InitCollisionCallbacks(){
    
	NiPhysXProp* spPlayerProp = m_spPhysScene->GetPropAt(1);
	for(int k = 2; k<NUM_WALLS+2; k++)
	{
		NiPhysXProp* phys = m_spPhysScene->GetPropAt(k);
		for (int i = 0; i < phys->GetDestinationsCount(); i++)
		{
			NiPhysXDest* src = phys->GetDestinationAt(i);
			if (NiIsKindOf(NiPhysXTransformDest, src))
			{
				NiPhysXTransformDest* s = (NiPhysXTransformDest*) src;
				//if (findPlayerActor(s->GetActor())) continue;

				for(int j = 0; j<spPlayerProp->GetDestinationsCount(); j++)
				{    
					NxActor* playerActor = ((NiPhysXRigidBodyDest*)spPlayerProp->GetDestinationAt(j))->GetActor();
					m_spPhysScene->GetPhysXScene()->setActorPairFlags(
						*playerActor, *s->GetActor(), NX_NOTIFY_ON_START_TOUCH | NX_NOTIFY_FORCES );

				}

			}
		}
	}
	
}

//---------------------------------------------------------------------------
void 
Flex::ProcessInput(){
    NiApplication::ProcessInput();

	NiInputMouse* pkMouse = GetInputSystem()->GetMouse();

	GameStateManager::getInstance()->processMouse( pkMouse );

    NiInputKeyboard* pkKeyboard = GetInputSystem()->GetKeyboard();

	m_playerDisplay->processKeyboard(pkKeyboard);
	GameStateManager::getInstance()->processKeyboard(pkKeyboard);
}
//---------------------------------------------------------------------------
bool
Flex::findPlayerActor(NxActor* actor){
	vector<NxActor*>::iterator it;
	it = std::find(m_player.begin(), m_player.end(), actor);
	if (it == m_player.end()){
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------
void
Flex::setSkeletonMap(){
	actorSkeleton["root"] = 0;
	actorSkeleton["lhipjoint"] = 1;
	actorSkeleton["lfemur"] = 2;
	actorSkeleton["ltibia"] = 3;
	actorSkeleton["lfoot"] = 4;
	actorSkeleton["ltoes"] = 5;
	actorSkeleton["rhipjoint"] = 6;
	actorSkeleton["rfemur"] = 7;
	actorSkeleton["rtibia"] = 8;
	actorSkeleton["rfoot"] = 9;
	actorSkeleton["rtoes"] = 10;
	actorSkeleton["lowerback"] = 11;
	actorSkeleton["upperback"] = 12;
	actorSkeleton["thorax"] = 13;
	actorSkeleton["lclavicle"] = 14;
	actorSkeleton["lhumerus"] = 15;
	actorSkeleton["lradius"] = 16;
	actorSkeleton["lwrist"] = 17;
	actorSkeleton["lhand"] = 18;
	actorSkeleton["rclavicle"] = 19;
	actorSkeleton["rhumerus"] = 20;
	actorSkeleton["rradius"] = 21;
	actorSkeleton["rwrist"] = 22;
	actorSkeleton["rhand"] = 23;
	actorSkeleton["lowerneck"] = 24;
	actorSkeleton["upperneck"] = 25;
	actorSkeleton["head"] = 26;

}