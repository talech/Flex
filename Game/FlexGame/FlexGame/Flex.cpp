#include <NiMain.h>
#include <NiAnimation.h>
#include <NiLicense.h>

#include "GameStateManager.h"
#include "GameState.h"
#include "RunningState.h"
#include "InitializeState.h"
#include "LoadState.h"
#include "NewGameMenu.h"


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
    m_pkPhysManager->m_pPhysXSDK->setParameter(NX_VISUALIZE_BODY_AXES, 2.0f);
    m_pkPhysManager->m_pPhysXSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1.0f);
    m_pkPhysManager->m_pPhysXSDK->setParameter(NX_VISUALIZE_ACTOR_AXES, 5.0);
    m_pkPhysManager->m_pPhysXSDK->setParameter(NX_VISUALIZE_BODY_LIN_VELOCITY, 5.0);

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
    m_pkPhysManager->Shutdown();
	m_pCameraController = 0;
	NiApplication::Terminate();    

}
//---------------------------------------------------------------------------
bool 
Flex::CreateScene(){
	GameStateManager::getInstance()->pushState(LoadState::getInstance());

	if(!NiApplication::CreateScene())
        return false;

	NiAlphaAccumulator* pkAccum = NiNew NiAlphaAccumulator;
    m_spRenderer->SetSorter(pkAccum);

	// Set the background color
    NiColor kColor(0.5f, 0.6f, 1.0f);
    m_spRenderer->SetBackgroundColor(kColor);

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

	m_pPlayer = new Player();
	m_pPlayer->LoadSkeleton("C:/Users/Tammy/Documents/School/cis499/Flex_code/EMG/Bin/Actor.asf");
	m_pPlayer->LoadMotion("C:/Users/Tammy/Documents/School/cis499/Flex_code/EMG/Bin/Database/Motion/Dance/Swing2.amc");
    m_playerDisplay = new PlayerDisplay(m_pPlayer,m_spScene);
	totalFrame = m_pPlayer->GetTotalFrameCount();

	GameStateManager::getInstance()->popState();

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
    if (!kStream.Load(ConvertMediaFilename("Environment.nif")))
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
    if (!kStream.Load(ConvertMediaFilename("Player.nif")))
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

	NiAVObject* player = m_spScene->GetObjectByName("character1");
    for (int i = 0; i < physPlayer->GetDestinationsCount(); i++)
    {
        NiPhysXDest* src = physPlayer->GetDestinationAt(i);
        if (NiIsKindOf(NiPhysXTransformDest, src))
        {
            NiPhysXTransformDest* s = (NiPhysXTransformDest*) src;
            if (s->GetTarget() == player)
            {
                m_player = s->GetActor();
            }
        }
    }

	InitCollisionCallbacks();
    SetWallPhysicsEnabled(false, true);
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
	
    if (!kStream.Load(ConvertMediaFilename("Wall1.nif")))
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
//---------------------------------------------------------------------------
bool
Flex::InitCamera(){
	if (m_spScene)
    {
        m_spCamera = FindFirstCamera(m_spScene);
		m_spCamera->LookAtWorldPoint(NiPoint3(0,0,0), NiPoint3(0,1,0));
        m_spCamera->SetTranslate(NiPoint3::ZERO); // Needed to initialize orbit navigation   
    }

	NiAVObject* cameraRoot = m_spScene->GetObjectByName("camera1");
	m_pCameraController = NiNew CameraController(this, cameraRoot);

    return (m_spCamera != NULL);

}

//---------------------------------------------------------------------------

void
Flex::UpdateFrame(){
	m_spPhysScene->UpdateSources(m_fAccumTime);
    m_spPhysScene->Simulate(m_fAccumTime);
    m_spPhysScene->FetchResults(m_fAccumTime, true);
    m_spPhysScene->UpdateDestinations(m_fAccumTime);
    m_pCameraController->Update(m_fAccumTime);

	NiApplication::UpdateFrame(); // Calls process input
    m_spScene->Update(m_fAccumTime); // update the scene graph.

	fLastSimTime = m_fAccumTime;

	// For debugging: toggle physics viz
	if (GetInputSystem()->GetKeyboard()->KeyWasPressed(NiInputKeyboard::KEY_P))
	{
		bool d = m_spPhysScene->GetDebugRender();
	    m_spPhysScene->SetDebugRender(!d, m_spScene);
	}
	if (this->GetInputSystem()->GetKeyboard()->KeyWasPressed(NiInputKeyboard::KEY_UP)){
		pkKeyboard = this->GetInputSystem()->GetKeyboard();
		//update skeleton position
		PlayMotion();

		// update the playerDisplay
		m_playerDisplay->Update();
	}	

	GameStateManager::getInstance()->update(m_fAccumTime);    

}

//---------------------------------------------------------------------------
void 
Flex::RenderFrame(){
    NiApplication::RenderFrame();
}
//---------------------------------------------------------------------------
void 
Flex::processContacts(NxContactPair& pair, NxU32 events){
    NxActor* a1 = pair.actors[0];
    NxActor* a2 = pair.actors[1];
	NxVec3 normalForce = pair.sumNormalForce;

    NILOG(NIMESSAGE_GENERAL_0, "CONTACT: a1: %s a2: %s (%f)\n", 
            a1->getName(), a2->getName(), normalForce.magnitude());

    SetWallPhysicsEnabled(true);
}
//---------------------------------------------------------------------------
void 
Flex::SetWallPhysicsEnabled(bool b, bool force){
    if (force || b != m_wallPhysicsEnabled)
    {
        NiPhysXProp* phys = m_spPhysScene->GetPropAt(2);
        for (int i = 0; i < phys->GetDestinationsCount(); i++)
        {
            NiPhysXDest* src = phys->GetDestinationAt(i);
            if (NiIsKindOf(NiPhysXTransformDest, src))
            {
                NiPhysXTransformDest* s = (NiPhysXTransformDest*) src;
                if (s->GetActor() == m_player) continue;

                if (b) s->GetActor()->clearBodyFlag(NX_BF_FROZEN);
                else s->GetActor()->raiseBodyFlag(NX_BF_FROZEN);
            }
        }
        b = m_wallPhysicsEnabled;
    }
}
//---------------------------------------------------------------------------
void 
Flex::InitCollisionCallbacks(){
    NiPhysXProp* phys = m_spPhysScene->GetPropAt(2);
    for (int i = 0; i < phys->GetDestinationsCount(); i++)
    {
        NiPhysXDest* src = phys->GetDestinationAt(i);
        if (NiIsKindOf(NiPhysXTransformDest, src))
        {
            NiPhysXTransformDest* s = (NiPhysXTransformDest*) src;
            if (s->GetActor() == m_player) continue;

            m_spPhysScene->GetPhysXScene()->setActorPairFlags(
			    *m_player, *s->GetActor(), NX_NOTIFY_ON_TOUCH | NX_NOTIFY_FORCES );

        }
    }
}
//---------------------------------------------------------------------------
void
Flex::PlayMotion(){
	if (pkKeyboard != NULL){
		if (pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_UP)){
			m_pPlayer->m_frameIndex = m_pPlayer->GetNextFrameIndex(m_pPlayer->m_frameIndex);
			m_pPlayer->UpdateFrame(m_pPlayer->m_frameIndex);
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
	GameStateManager::getInstance()->processKeyboard(pkKeyboard);
}
//---------------------------------------------------------------------------
