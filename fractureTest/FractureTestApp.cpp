
#include <NiMain.h>
#include <NiAnimation.h>
#include <NiParticle.h>
#include <NiLicense.h>
#include <NiPick.h>
#include <NiNode.h>
#include <iostream>
//#include <fstream>

NiEmbedGamebryoLicenseCode;

#include "FractureTestApp.h"
#include "CameraController.h"

NiApplication* NiApplication::Create()
{
    return NiNew FractureTestApp;
}

FractureTestApp::FractureTestApp() : NiApplication("Fracture Test",
    DEFAULT_WIDTH /* 640 */,  DEFAULT_HEIGHT /* 480 */, true),
    m_PhysContactReporter()
{
    SetMediaPath("Data\\");   
    m_PhysContactReporter.m_app = this;
    
    // Initialize smart pointers to zero. In the case of early termination
    // this avoid errors.
    m_spPhysScene = 0;
	m_pCameraController = 0;
}

bool FractureTestApp::Initialize()
{
    // Save a pointer to the NiPhysXManager object. This is Gamembryo's
    // manager for all PhysX global functionality. The file "NiPhysX.h"
    // must be included for this object to exist.
    m_pkPhysManager = NiPhysXManager::GetPhysXManager();
    
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
        NiMessageBox(acMsg, "PhysX Initialization Failed");

        return false;
    }
        
    // The manager contains a public pointer to the PhysX SDK object,
    // m_pkPhysXSDK. Here we use it to set some global SDK parameters.
    // See the PhysX documentation for an explanation of these settings.
    m_pkPhysManager->m_pkPhysXSDK->setParameter(NX_SKIN_WIDTH, 0.01f);
    m_pkPhysManager->m_pkPhysXSDK->setParameter(NX_BOUNCE_THRESHOLD, -2.0f);
    m_pkPhysManager->m_pkPhysXSDK->setParameter(NX_VISUALIZATION_SCALE, 1.0f);
    m_pkPhysManager->m_pkPhysXSDK->setParameter(NX_VISUALIZE_BODY_AXES, 2.0f);
    m_pkPhysManager->m_pkPhysXSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1.0f);
    m_pkPhysManager->m_pkPhysXSDK->setParameter(NX_VISUALIZE_ACTOR_AXES, 5.0);
    m_pkPhysManager->m_pkPhysXSDK->setParameter(NX_VISUALIZE_BODY_LIN_VELOCITY, 5.0);

    if (!NiApplication::Initialize())
        return false;           

    // Update the scene graph before rendering begins.
    m_spScene->UpdateProperties();
    m_spScene->UpdateEffects();
    m_spScene->Update(0.0f);
    
    // Get simulation started. We give a small timestep to avoid zero length
    // steps, which cause problems with PhysX hardware in v2.3.2
    m_spPhysScene->UpdateSources(0.001f);
    m_spPhysScene->Simulate(0.001f);
    fLastSimTime = 0.001f;

    return true;
}

void FractureTestApp::Terminate()
{   
    m_spPhysScene = 0;
    m_pkPhysManager->Shutdown();
	m_pCameraController = 0;
    NiApplication::Terminate();
}

bool FractureTestApp::CreateScene()
{
    // Because our scene will have some billboards with alpha, we use 
    // a NiAlphaAccumulator in order that our alpha gets sorted and drawn
    // correctly.
    NiAlphaAccumulator* pkAccum = NiNew NiAlphaAccumulator;
    m_spRenderer->SetSorter(pkAccum);

    if (!InitPhysics())
    {
        return false;
    }

    if (!InitEnvironment())
    {
        NiMessageBox("Cannot initialize environment!", "Bear Picnic Error");
        return false;
    }

    if (!InitCamera())
    {
        NiMessageBox("The NIF file has no camera!", "Camera Error");
        return false;
    }

    return true;
}

void FractureTestApp::Reset()
{
    m_spPhysScene->RestoreSnapshotState(0);
    SetWallPhysicsEnabled(false);
}

//---------------------------------------------------------------------------   
void FractureTestApp::UpdateFrame()
{
    m_spPhysScene->UpdateSources(m_fAccumTime);
    m_spPhysScene->Simulate(m_fAccumTime);
    m_spPhysScene->FetchResults(m_fAccumTime, true);
    m_spPhysScene->UpdateDestinations(m_fAccumTime);
    m_pCameraController->Update(m_fAccumTime);

	NiApplication::UpdateFrame();
	m_spScene->Update(m_fAccumTime);

    fLastSimTime = m_fAccumTime;	

	// For debugging: toggle physics viz
	if (GetInputSystem()->GetKeyboard()->KeyWasPressed(NiInputKeyboard::KEY_P))
	{
		bool d = m_spPhysScene->GetDebugRender();
	    m_spPhysScene->SetDebugRender(!d, m_spScene);
	}
	if (GetInputSystem()->GetKeyboard()->KeyWasPressed(NiInputKeyboard::KEY_R))
	{
        Reset();
	}
	if (GetInputSystem()->GetKeyboard()->KeyWasPressed(NiInputKeyboard::KEY_SPACE))
	{
        m_ball->setLinearVelocity(NxVec3(-50.0, 0.0, 0.0));
    }
}

void FractureTestApp::RenderFrame()
{
    NiApplication::RenderFrame();
}

NiCamera* FractureTestApp::FindCamera(NiAVObject* pkObject)
{
    if (NiIsKindOf(NiCamera, pkObject))
    {
        return (NiCamera*) pkObject;
    }
    else if (NiIsKindOf(NiNode, pkObject))
    {
        NiNode* pkNode = (NiNode*) pkObject;
        for (unsigned int ui = 0; ui < pkNode->GetArrayCount(); ui++)
        {
            NiCamera* pkFoundCamera = FindCamera(pkNode->GetAt(ui));
            if (pkFoundCamera) return pkFoundCamera;
        }
    }
    return NULL;
}

bool FractureTestApp::InitCamera()
{
    if (m_spScene)
    {
        m_spCamera = FindCamera(m_spScene);
		m_spCamera->LookAtWorldPoint(NiPoint3(0,0,0), NiPoint3(0,1,0));
        m_spCamera->SetTranslate(NiPoint3::ZERO); // Needed to initialize orbit navigation   
    }

	NiAVObject* cameraRoot = m_spScene->GetObjectByName("Camera");
	m_pCameraController = NiNew CameraController(this, cameraRoot);

    return (m_spCamera != NULL);
}

bool FractureTestApp::InitEnvironment()
{
    NiStream kStream;
    bool bSuccess = kStream.Load(ConvertMediaFilename("Environment.nif"));
    if (!bSuccess)
    {
        NiMessageBox("Environment.nif file could not be loaded!", "NIF Error");
        return false;
    }

    m_spScene = (NiNode*) kStream.GetObjectAt(0);
    NIASSERT(NiIsKindOf(NiNode, m_spScene));

    NiPhysXPropPtr phys;
    if (!AddPhysicsProps(kStream, phys))
    {
        NiMessageBox("The NIF file has no physics objects!", "Warning");        
    }

    NiAVObject* ball = m_spScene->GetObjectByName("nxBall");
    for (int i = 0; i < phys->GetDestinationsCount(); i++)
    {
        NiPhysXDest* src = phys->GetDestinationAt(i);
        if (NiIsKindOf(NiPhysXTransformDest, src))
        {
            NiPhysXTransformDest* s = (NiPhysXTransformDest*) src;
            // NiObject is in s->GetTarget()
            // NxActor = s->GetActor()
            if (s->GetTarget() == ball)
            {
                m_ball = s->GetActor();
                s->GetActor()->setLinearVelocity(NxVec3(0.0, 0.0, 0.0));
            }
        }
    }

    InitCollisionCallbacks();
    SetWallPhysicsEnabled(false, true);
    return true;
}

bool FractureTestApp::InitPhysics()
{
    // We first create a PhysX scene and the Gamebryo wrapper for it. First
    // the wrapper.
    m_spPhysScene = NiNew NiPhysXScene();
    
    // We are doing asynchronous simulation, and the scene wrapper must be
    // informed.
    m_spPhysScene->SetAsynchronousSimulation(false);
    
    // Then the PhysX scene
    NxSceneDesc kSceneDesc;
    kSceneDesc.gravity.set(0.0f, -50.0f, 0.0);
    NxScene* pkScene = m_pkPhysManager->m_pkPhysXSDK->createScene(kSceneDesc);
    NIASSERT(pkScene != 0);    
    pkScene->setUserContactReport(&m_PhysContactReporter);
    
    // Attach the physics scene to the wrapper
    m_spPhysScene->SetPhysXScene(pkScene);
    m_spPhysScene->SetUpdateDest(true);
    m_spPhysScene->SetUpdateSrc(true);
    return true;
}

bool FractureTestApp::AddPhysicsProps(NiStream& kStream, NiPhysXPropPtr& propPtr)
{
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

void FractureTestApp::processContacts(NxContactPair& pair, NxU32 events)
{
    NxActor* a1 = pair.actors[0];
    NxActor* a2 = pair.actors[1];
	NxVec3 normalForce = pair.sumNormalForce;

    NILOG(NIMESSAGE_GENERAL_0, "CONTACT: a1: %s a2: %s (%f)\n", 
            a1->getName(), a2->getName(), normalForce.magnitude());

    SetWallPhysicsEnabled(true);
}

void FractureTestApp::SetWallPhysicsEnabled(bool b, bool force)
{
    if (force || b != m_wallPhysicsEnabled)
    {
        NiPhysXProp* phys = m_spPhysScene->GetPropAt(0);
        for (int i = 0; i < phys->GetDestinationsCount(); i++)
        {
            NiPhysXDest* src = phys->GetDestinationAt(i);
            if (NiIsKindOf(NiPhysXTransformDest, src))
            {
                NiPhysXTransformDest* s = (NiPhysXTransformDest*) src;
                if (s->GetActor() == m_ball) continue;

                if (b) s->GetActor()->clearBodyFlag(NX_BF_FROZEN);
                else s->GetActor()->raiseBodyFlag(NX_BF_FROZEN);
            }
        }
        b = m_wallPhysicsEnabled;
    }
}

void FractureTestApp::InitCollisionCallbacks()
{
    NiPhysXProp* phys = m_spPhysScene->GetPropAt(0);
    for (int i = 0; i < phys->GetDestinationsCount(); i++)
    {
        NiPhysXDest* src = phys->GetDestinationAt(i);
        if (NiIsKindOf(NiPhysXTransformDest, src))
        {
            NiPhysXTransformDest* s = (NiPhysXTransformDest*) src;
            if (s->GetActor() == m_ball) continue;

            m_spPhysScene->GetPhysXScene()->setActorPairFlags(
			    *m_ball, *s->GetActor(), NX_NOTIFY_ON_TOUCH | NX_NOTIFY_FORCES );

        }
    }
}