#include <NiApplication.h>
#include <NiFont.h>
#include <Ni2DString.h>
#include <string>
#include "Flex.h"
#include "GameStateManager.h"
#include "ScreenOverlay.h"
#include "NiSystemCursor.h"

#define WINDOW_HEADER_WIDTH 30 

//---------------------------------------------------------------------------
//---------------------- Screen Message Class -------------------------------
//---------------------------------------------------------------------------
class ScreenMessage
{
public:
    ScreenMessage() : m_x(0), m_y(0), m_name(""), m_img(0), m_isOk(false) {}
    ScreenMessage(const char* basename, int x, int y)
    {
        m_name = basename;
        char filename[256];
        sprintf_s(filename, 256, "Messages/%s.tga", basename);
        
        NiSourceTexturePtr pkTexture = NiSourceTexture::Create(
            NiApplication::ConvertMediaFilename(filename));
        if (!pkTexture)
        {
            NiOutputDebugString("Error: ScreenOverlay::Create() Failed\n");
            m_isOk = false;
        }
        m_width = pkTexture->GetWidth();
        m_height = pkTexture->GetHeight();
		m_x = (x/2) - (m_width/2);
		m_y = (y/2) - (m_height/2);
        m_img = NiMeshScreenElements::Create(pkTexture);
        m_img->AddNewScreenRect(m_y, m_x, m_width, m_height, 0, 0);
        
    }
    
    ~ScreenMessage()
    {
        m_img = 0;
    }
    
    bool ok() { return m_isOk; }
    const char* name() { return m_name.c_str(); }
    bool isOn(int x, int y)
    {
        return (x > m_x && x < m_x + m_width &&
            y > m_y && y < m_y + m_height);
    }
    
    bool draw(NiRenderer* pkRenderer, int x, int y, bool highlight) // Return true if highlighted
    {
        //if (isOn(x, y) || highlight)
		if (highlight)
        {
            m_img->RenderImmediate(pkRenderer);
            return true;
        }
		return false;
    }
    
protected:
    int m_x, m_y;
    int m_width;
    int m_height;
    bool m_isOk;
    NiMeshScreenElementsPtr m_img;
    std::string m_name;
};

//---------------------------------------------------------------------------
//---------------------- End Screen Message Class ---------------------------
//---------------------------------------------------------------------------

ScreenOverlay* ScreenOverlay::ms_pkTheScreenOverlay = 0;

ScreenOverlay::ScreenOverlay(Flex* app) : m_app(app), m_messages()
{
}

bool ScreenOverlay::Create(Flex* app)
{
    NiRenderer* pkRenderer = NiRenderer::GetRenderer();

    ms_pkTheScreenOverlay = NiNew ScreenOverlay(app);

    // Load HUD Font
    /*ms_pkTheScreenOverlay->m_spHUDFont = NiFont::Create(pkRenderer,
        NiApplication::ConvertMediaFilename("Calibri_BA_18.nff"));
    if (!ms_pkTheScreenOverlay->m_spHUDFont)
    {
        NiOutputDebugString("Error: ScreenOverlay::Create() Failed\n");
        return false;
    }*/

    // Initialize 2D Strings        
    /*NiColorA kColor(1.0f, 1.0f, 1.0f, 1.0f);
    ms_pkTheScreenOverlay->m_currentBehavior = NiNew Ni2DString(
        ms_pkTheScreenOverlay->m_spHUDFont, NiFontString::COLORED, 32, 
        "Let's make this happen!", kColor, 0, 0);*/

    // Initialize buttons
	int width = app->DEFAULT_WIDTH;
	int height = app->DEFAULT_HEIGHT;
    ms_pkTheScreenOverlay->m_messages.resize(2);
    ms_pkTheScreenOverlay->m_messages[0] = new ScreenMessage("newgame", width, height);
	ms_pkTheScreenOverlay->m_messages[1] = new ScreenMessage("collided", width, height);
    
    return true;
}
//---------------------------------------------------------------------------
void ScreenOverlay::Destroy()
{
    NiDelete ms_pkTheScreenOverlay;
}
//---------------------------------------------------------------------------
ScreenOverlay::~ScreenOverlay()
{
    //m_currentState = 0;
    //m_spHUDFont = 0;
    for (unsigned int i = 0; i < m_messages.size(); i++) delete m_messages[i];
    m_app = 0;
}



void ScreenOverlay::Draw()
{
    NiRenderer* pkRenderer = NiRenderer::GetRenderer();
    pkRenderer->SetScreenSpaceCameraData();
   
	displayMessages(pkRenderer, GameStateManager::getInstance()->state);
}



void ScreenOverlay::displayMessages(NiRenderer* pkRenderer, ActiveState state)
{
	int x,y;
	//m_app->GetMousePos(x,y);

	if(state != aWallMoving){
		m_messages[state]->draw(pkRenderer, 0, 0, true);
	}
	
}

