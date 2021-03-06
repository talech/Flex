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

Flex::Flex() : NiSample("Flex",
    DEFAULT_WIDTH, DEFAULT_HEIGHT, true)
{
#if defined(_XENON)
    SetMediaPath("D:/DATA/");
#elif defined (WIN32)
    SetMediaPath("../Data/");   
#elif defined (_PS3)
    SetMediaPath("../../../../Data/");  
#elif defined(_WII)


    m_uiMaxOutputLogLines = 4;
#endif

    m_bUseNavSystem = false;
}
//---------------------------------------------------------------------------

Flex::~Flex(){
}
//---------------------------------------------------------------------------
bool 
Flex::CreateScene(){
    // Because our scene will have some billboards with alpha, we use 
    // a NiAlphaAccumulator in order that our alpha gets sorted and drawn
    // correctly.
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
        NiApplication::ConvertMediaFilename("WORLD.NIF"));

    if (!bSuccess)
    {
        NiMessageBox("WORLD.NIF file could not be loaded!", "NIF Error");
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

    // We expect the world to have been exported with a camera, so we 
    // look for it here.
    // In order to render the scene graph, we need a camera. We're now going
    // to recurse the scene graph looking for a camera.

    return bSuccess;
}
//---------------------------------------------------------------------------
bool 
Flex::CreateUIElements(){

    NiSample::CreateUIElements();

    // The Log is a text output used by all NiSample derived classes
#if defined(_WII)
    AddLogEntry("Press (minus) + (plus) on a Wii Remote or "
        "Z + Start on a Gamecube gamepad to quit");
#else
    AddLogEntry("Press ESC or Start + Select to quit");
#endif

    return true;
}
