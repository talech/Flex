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

    // NiStreams are used to load a NIF file from disk. Once a stream is 
    // loaded, it will contain one or more "top-level" objects. These objects
    // could be NiNodes, NiTextures, or any other Gamebryo class. The Max and 
    // Maya exporters both place the scene graph as the first element in the
    // NIF file.

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

	
	
	

    return bSuccess;
}
//---------------------------------------------------------------------------

void
Flex::UpdateFrame(){
	NiApplication::UpdateFrame(); // Calls process input
	m_playerDisplay->Update();
}