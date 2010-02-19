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
    DEFAULT_WIDTH, DEFAULT_HEIGHT, true)
{
#if defined (WIN32)
    SetMediaPath("../Data/");  
#endif


	m_spTrnNode = 0;
    m_spRotNode = 0;

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
    m_pkPhysManager->m_pPhysXSDK->setParameter(NX_BOUNCE_THRESHOLD, -0.75f);
    m_pkPhysManager->m_pPhysXSDK->setParameter(NX_VISUALIZATION_SCALE, 2.0f);

    if (!NiApplication::Initialize())
        return false;
            
    // Set up camera control
    SetTurretControls();
	pkKeyboard = NULL;

    // Update the scene graph before rendering begins.
    m_spScene->UpdateProperties();
    m_spScene->UpdateEffects();
    m_spScene->Update(0.0f);

	GameStateManager::getInstance()->physManager = m_pkPhysManager;
	GameStateManager::getInstance()->physScene = m_spPhysScene;
	GameStateManager::getInstance()->scene = m_spScene;
	GameStateManager::getInstance()->changeState(NewGameMenu::getInstance());
    
    return true;
}
//---------------------------------------------------------------------------
void Flex::Terminate()
{
    m_spTrnNode = 0;
    m_spRotNode = 0;

    // The PhysX scenes should be deleted before the SDK is shut down.
    m_spPhysScene = 0;

    // The PhysX manager must be shut down after all PhysX content has been
    // deleted and before the application terminates.
    m_pkPhysManager->Shutdown();

	NiApplication::Terminate();    

}
//---------------------------------------------------------------------------
bool 
Flex::CreateScene(){
	GameStateManager::getInstance()->pushState(LoadState::getInstance());
    if(!NiApplication::CreateScene())
        return false;

    // Set the background color
    NiColor kColor(0.5f, 0.6f, 1.0f);
    m_spRenderer->SetBackgroundColor(kColor);

    NiAlphaAccumulator* pkAccum = NiNew NiAlphaAccumulator;
    m_spRenderer->SetSorter(pkAccum);

    // We first create a PhysX scene and the Gamebryo wrapper for it. First
    // the wrapper.
    m_spPhysScene = NiNew NiPhysXScene();
    
    // Then the PhysX scene
    NxSceneDesc kSceneDesc;
    kSceneDesc.gravity.set(0.0f, -9.8f, 0.0f);
    NxScene* pkScene = m_pkPhysManager->m_pPhysXSDK->createScene(kSceneDesc);
    NIASSERT(pkScene != 0);
	
    
    // Attach the physics scene to the wrapper
    m_spPhysScene->SetPhysXScene(pkScene);
    
    // Load some PhysX-enabled content.
    NiStream kStream;
    if (!kStream.Load(ConvertMediaFilename("Sphere.nif")))
	{
        NIASSERT(0 && "Couldn't load nif file\n");
        NiMessageBox("Could not load Wall1.nif. Aborting\n",
            "Missing nif file.");
 
        return false;
    }

    m_spScene = (NiNode*) kStream.GetObjectAt(0);
    NIASSERT(NiIsKindOf(NiNode, m_spScene));

	m_spCamera = FindFirstCamera(m_spScene);
    if (!m_spCamera)
    {
        NiMessageBox("The NIF file has no camera!", "Camera Error");
        return false;
    }

	// Look for a camera and the PhysX content. In this case, the PhysX
    // content is the Flex scene.
    NiPhysXPropPtr spWallProp = 0;
    for (unsigned int ui = 1; ui < kStream.GetObjectCount(); ui++)
    {
        if (NiIsKindOf(NiPhysXProp, kStream.GetObjectAt(ui)))
        {
            // We have found the PhysX content in the NIF.
            spWallProp = (NiPhysXProp*)kStream.GetObjectAt(ui);
        }
    }
    NIASSERT(spWallProp != 0);

	m_spPhysScene->AddProp(spWallProp);

	m_spPhysScene->SetUpdateDest(true);

	// Show Debug data
	m_spPhysScene->SetDebugRender(true, m_spScene);

	m_pPlayer = new Player();
	m_pPlayer->LoadSkeleton("C:/Users/Tammy/Documents/School/cis499/Flex_code/EMG/Bin/Actor.asf");
	m_pPlayer->LoadMotion("C:/Users/Tammy/Documents/School/cis499/Flex_code/EMG/Bin/Database/Motion/Dance/Swing2.amc");
    m_playerDisplay = new PlayerDisplay(m_pPlayer,m_spScene);
	totalFrame = m_pPlayer->GetTotalFrameCount();

    return true;
}
//---------------------------------------------------------------------------

void
Flex::UpdateFrame(){
	NiApplication::UpdateFrame(); // Calls process input

	// Update the camera. This uses global time.
    if (m_kTurret.Read())
        m_spTrnNode->Update(m_fAccumTime);

	GameStateManager::getInstance()->update(m_fAccumTime);    

 
	if (this->GetInputSystem()){
		pkKeyboard = this->GetInputSystem()->GetKeyboard();
	}


	//update skeleton position
	PlayMotion();

	// update the playerDisplay
	m_playerDisplay->Update();

	// update the scene graph.
    m_spScene->Update(m_fAccumTime);


}

//---------------------------------------------------------------------------
void 
Flex::SetTurretControls(){   

    m_spCamera->Update(0.0f);
    m_spTrnNode = NiNew NiNode();
    m_spTrnNode->SetTranslate(m_spCamera->GetWorldTranslate());
    m_spCamera->SetTranslate(NiPoint3::ZERO);
    m_spRotNode = NiNew NiNode();
    m_spTrnNode->AttachChild(m_spRotNode);
    m_spRotNode->SetRotate(m_spCamera->GetWorldRotate());
    m_spCamera->SetRotate(NiMatrix3::IDENTITY);
    m_spRotNode->AttachChild(m_spCamera);
    m_spTrnNode->Update(0.0f);
    
    float fTrnSpeed = 0.05f;
    float fRotSpeed = 0.005f;

    m_kTurret.SetStandardTrn(fTrnSpeed, m_spTrnNode);
    m_kTurret.SetStandardRot(fRotSpeed, m_spTrnNode, m_spRotNode);
    NiMatrix3 kRot;
    kRot.SetCol(0, 1.0f, 0.0f, 0.0f);
    kRot.SetCol(1, 0.0f, 0.0f, 1.0f);
    kRot.SetCol(2, 0.0f, -1.0f, 0.0f);
    m_kTurret.SetAxes(kRot);
    
    if (m_kTurret.GetInputDevice() == NiTurret::TUR_KEYBOARD)
    {
        m_kTurret.SetTrnButtonsKB(2,
            NiInputKeyboard::KEY_W, NiInputKeyboard::KEY_S);
        m_kTurret.SetTrnButtonsKB(1,
            NiInputKeyboard::KEY_Q, NiInputKeyboard::KEY_E);
        m_kTurret.SetTrnButtonsKB(0,
            NiInputKeyboard::KEY_D, NiInputKeyboard::KEY_A);
            
        m_kTurret.SetRotButtonsKB(1,
            NiInputKeyboard::KEY_J, NiInputKeyboard::KEY_L);
        m_kTurret.SetRotButtonsKB(2,
            NiInputKeyboard::KEY_I, NiInputKeyboard::KEY_K);
		m_kTurret.SetRotButtonsKB(2,
            NiInputKeyboard::KEY_U, NiInputKeyboard::KEY_O);
    }
    else if (m_kTurret.GetInputDevice() == NiTurret::TUR_GAMEPAD)
    {
        m_kTurret.SetTrnButtonsStickDirGP(0, 
            NiInputGamePad::NIGP_STICK_LEFT, 
            NiInputGamePad::NIGP_STICK_AXIS_V);
        m_kTurret.SetTrnButtonsGP(1, 
            NiInputGamePad::NIGP_L1, 
            NiInputGamePad::NIGP_R1);
        m_kTurret.SetTrnButtonsStickDirGP(2, 
            NiInputGamePad::NIGP_STICK_LEFT, 
            NiInputGamePad::NIGP_STICK_AXIS_H);

        m_kTurret.SetRotButtonsGP(0, NiInputGamePad::NIGP_NONE,
            NiInputGamePad::NIGP_NONE);
        m_kTurret.SetRotButtonsStickDirGP(1, 
            NiInputGamePad::NIGP_STICK_RIGHT, 
            NiInputGamePad::NIGP_STICK_AXIS_H);
        m_kTurret.SetRotModifiers(1, NiInputGamePad::NIGP_MASK_NONE);
        m_kTurret.SetRotButtonsStickDirGP(2, 
            NiInputGamePad::NIGP_STICK_RIGHT, 
            NiInputGamePad::NIGP_STICK_AXIS_V);
        m_kTurret.SetRotModifiers(2, NiInputGamePad::NIGP_MASK_NONE);
    }
}
//---------------------------------------------------------------------------

void
Flex::PlayMotion(){
	if (pkKeyboard != NULL){
		if (pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_RIGHT)){
			m_pPlayer->m_frameIndex = m_pPlayer->GetNextFrameIndex(m_pPlayer->m_frameIndex);
			m_pPlayer->UpdateFrame(m_pPlayer->m_frameIndex);
		}
    }

}