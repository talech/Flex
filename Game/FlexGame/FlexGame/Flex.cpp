#include <NiMain.h>
#include <NiAnimation.h>
#include <NiParticle.h>
#include <NiLicense.h>


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
}
//---------------------------------------------------------------------------

Flex::~Flex(){
}
//---------------------------------------------------------------------------
bool 
Flex::CreateScene(){
    if(!NiApplication::CreateScene())
        return false;

    // Set the background color
    NiColor kColor(0.5f, 0.6f, 1.0f);
    m_spRenderer->SetBackgroundColor(kColor);

    NiAlphaAccumulator* pkAccum = NiNew NiAlphaAccumulator;
    m_spRenderer->SetSorter(pkAccum);

    NiStream kStream;

    // Load in the scenegraph for our world...
    bool bSuccess = kStream.Load(
        NiApplication::ConvertMediaFilename("Sphere.NIF"));

    if (!bSuccess)
    {
        NiMessageBox("Sphere.NIF file could not be loaded!", "NIF Error");
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

	m_pPlayer = new Player();
	m_pPlayer->LoadSkeleton("C:/Users/Tammy/Documents/School/cis499/Flex_code/EMG/Tools/AMC2Maya/asf_amc_convert_to_maya/Style4.asf");
//	m_pPlayer->LoadMotion("C:\Users\Tammy\Documents\School\cis499\Flex_code\EMG\Tools\AMC2Maya\asf_amc_convert_to_maya\walk_fwd_circle.amc");
    m_playerDisplay = new PlayerDisplay(m_pPlayer,m_spScene);

	
	// Set up camera control
    SetTurretControls();

    // Update the scene graph before rendering begins.
    m_spScene->UpdateProperties();
    m_spScene->UpdateEffects();
    m_spScene->Update(0.0f);
	

    return bSuccess;
}
//---------------------------------------------------------------------------

void
Flex::UpdateFrame(){
	NiApplication::UpdateFrame(); // Calls process input

	// Update the camera. This uses global time.
    if (m_kTurret.Read())
        m_spTrnNode->Update(m_fAccumTime);

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