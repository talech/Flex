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
    ScreenMessage() : m_x(0), m_y(0), m_name(""), m_img(0), m_imgH(0), m_isOk(false) {}
    ScreenMessage(const char* basename, int x, int y, overType type)
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

		if(type == msg){
			m_x = (x/2) - (m_width/2);
			m_y = (y/2) - (m_height/2);
			m_img = NiMeshScreenElements::Create(pkTexture);
			m_img->AddNewScreenRect(m_y, m_x, m_width, m_height, 0, 0);
		}
		else if(type == lives){
			
			m_img = NiMeshScreenElements::Create(pkTexture);
			m_img->AddNewScreenRect(y, x, m_width, m_height, 0, 0);

			char filenameH[256];
			sprintf_s(filenameH, 256,"Messages/%s_gone.tga", basename);
	        
			pkTexture = NiSourceTexture::Create(
				NiApplication::ConvertMediaFilename(filenameH));
			if (!pkTexture)
			{
				NiOutputDebugString("Error: ScreenOverlay::Create() Failed\n");
				m_isOk = false;
			}
			m_imgH = NiMeshScreenElements::Create(pkTexture);
			m_imgH->AddNewScreenRect(y, x, m_width, m_height, 0, 0);

		}
        
    }
    
    ~ScreenMessage()
    {
        m_img = 0;
		m_imgH = 0;
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
		if (highlight){
            m_img->RenderImmediate(pkRenderer);
            return true;
        }
		else{
			m_imgH->RenderImmediate(pkRenderer);
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
	NiMeshScreenElementsPtr m_imgH;
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
    ms_pkTheScreenOverlay->m_spHUDFont = NiFont::Create(pkRenderer,
        NiApplication::ConvertMediaFilename("Calibri_BA_18.nff"));
    if (!ms_pkTheScreenOverlay->m_spHUDFont)
    {
        NiOutputDebugString("Error: ScreenOverlay::Create() Failed\n");
        return false;
    }

    // Initialize 2D Strings        
    NiColorA kColor(1.0f, 1.0f, 1.0f, 1.0f);
    ms_pkTheScreenOverlay->m_Score = NiNew Ni2DString(
        ms_pkTheScreenOverlay->m_spHUDFont, NiFontString::COLORED, 32, 
        "Let's make this happen!", kColor, 0, 0);

	ms_pkTheScreenOverlay->m_ScoreMsg = NiNew Ni2DString(
        ms_pkTheScreenOverlay->m_spHUDFont, NiFontString::COLORED, 38, 
        "Let's make this happen!", kColor, 0, 0);

    // Initialize messages
	int margin = 5;
    int size = 35;

	int width = app->DEFAULT_WIDTH;
	int height = app->DEFAULT_HEIGHT;
    
	ms_pkTheScreenOverlay->m_messages.resize(8);
    ms_pkTheScreenOverlay->m_messages[0] = new ScreenMessage("newgame", width, height, msg);
	ms_pkTheScreenOverlay->m_messages[1] = new ScreenMessage("collided2", width, height, msg);	//Infinite mode - Cont
	ms_pkTheScreenOverlay->m_messages[2] = new ScreenMessage("collided", width, height, msg);	//3 lives mode - Surviv
	ms_pkTheScreenOverlay->m_messages[3] = new ScreenMessage("gameOver", width, height, msg);
	ms_pkTheScreenOverlay->m_messages[4] = new ScreenMessage("highScores", width, height, msg);
	ms_pkTheScreenOverlay->m_messages[5] = new ScreenMessage("pausedCont", width, height, msg);
	ms_pkTheScreenOverlay->m_messages[6] = new ScreenMessage("pausedSurviv", width, height, msg);
	ms_pkTheScreenOverlay->m_messages[7] = new ScreenMessage("smash", width, height, msg);

	ms_pkTheScreenOverlay->m_lives.resize(3);
	ms_pkTheScreenOverlay->m_lives[0] = new ScreenMessage("dude", width-(margin*1 + size*1), margin, lives);
	ms_pkTheScreenOverlay->m_lives[1] = new ScreenMessage("dude", width-(margin*2 + size*2), margin, lives);	
	ms_pkTheScreenOverlay->m_lives[2] = new ScreenMessage("dude", width-(margin*3 + size*3), margin, lives);	
	
    
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
    m_Score = 0;
    m_spHUDFont = 0;
    for (unsigned int i = 0; i < m_messages.size(); i++) delete m_messages[i];
    m_app = 0;
}



void ScreenOverlay::Draw()
{
    NiRenderer* pkRenderer = NiRenderer::GetRenderer();
    pkRenderer->SetScreenSpaceCameraData();
   
	displayMessages(pkRenderer, GameStateManager::getInstance()->state);
	displayScore(pkRenderer,ScoreKeeper::getInstance()->getScore(), GameStateManager::getInstance()->state);
}



void ScreenOverlay::displayMessages(NiRenderer* pkRenderer, ActiveState state)
{
	int x,y;
	//m_app->GetMousePos(x,y);

	//if state is other than wall moving draw corresponding message
	if(state != aWallMoving){
		m_messages[state]->draw(pkRenderer, 0, 0, true);
	}

	if(state != aNewGame){
		//if survivor game mode display lives
		if(GameStateManager::getInstance()->mode == Surviv){
			for(int i = (m_lives.size()-1); i > -1; i--){
				int score = ScoreKeeper::getInstance()->getLives();
				if(i+1 < score || i+1 == score){
					m_lives[i]->draw(pkRenderer, 0, 0, true);
				}
				else{
					m_lives[i]->draw(pkRenderer, 0, 0, false);
				}
			}
		}
	}
	
}

void ScreenOverlay::displayScore(NiRenderer* pkRenderer, int score, ActiveState state)
{
	if(state != aNewGame && state != aGameOver && state != aHighScores){
		m_Score->sprintf("Score: %d", score);
	            
		float width, height;
		m_spHUDFont->GetTextExtent(m_Score->GetText(), width, height);

		unsigned int uiX = 5;
		unsigned int uiY = m_app->DEFAULT_HEIGHT - (int) height;
	               
		m_Score->SetPosition(5, 5);
		m_Score->Draw(pkRenderer);
	}
	else if(state == aGameOver){
		
		m_ScoreMsg->sprintf("Final Score: %d", score);
	            
		float width, height;
		m_spHUDFont->GetTextExtent(m_ScoreMsg->GetText(), width, height);

		unsigned int uiX = (m_app->DEFAULT_WIDTH/2)-(int)(width/2);
		unsigned int uiY =(m_app->DEFAULT_HEIGHT/2)-(int)(height/2);
	               
		m_ScoreMsg->SetPosition(uiX, uiY);
		m_ScoreMsg->Draw(pkRenderer);
	}

	else if(state == aHighScores){
		displayHighScore(pkRenderer);
	}

	//smash extra points
	if(state == aSmash){
		int smash = ScoreKeeper::getInstance()->getSmashScore();
		m_ScoreMsg->sprintf("%d points", smash);
	            
		float width, height;
		m_spHUDFont->GetTextExtent(m_ScoreMsg->GetText(), width, height);

		unsigned int uiX = (m_app->DEFAULT_WIDTH/2)-(int)(width/2);
		unsigned int uiY =(m_app->DEFAULT_HEIGHT/2)-(int)(height/2);
	               
		m_ScoreMsg->SetPosition(uiX, uiY);
		m_ScoreMsg->Draw(pkRenderer);
	}
}

void
ScreenOverlay::displayHighScore(NiRenderer *pkRenderer){
	
	m_ScoreMsg->sprintf("High Scores\n\n Continuous Mode: \n %s %d \n %s %d \n %s %d \n %s %d \n %s %d \n\n Survivor Mode: \n %s %d \n %s %d \n %s %d \n %s %d \n %s %d",
		ScoreKeeper::getInstance()->highCont[0].first.c_str(),ScoreKeeper::getInstance()->highCont[0].second,
		ScoreKeeper::getInstance()->highCont[1].first.c_str(),ScoreKeeper::getInstance()->highCont[1].second,
		ScoreKeeper::getInstance()->highCont[2].first.c_str(),ScoreKeeper::getInstance()->highCont[2].second,
		ScoreKeeper::getInstance()->highCont[3].first.c_str(),ScoreKeeper::getInstance()->highCont[3].second,
		ScoreKeeper::getInstance()->highCont[4].first.c_str(),ScoreKeeper::getInstance()->highCont[4].second,
		ScoreKeeper::getInstance()->highSurviv[0].first.c_str(),ScoreKeeper::getInstance()->highSurviv[0].second,
		ScoreKeeper::getInstance()->highSurviv[1].first.c_str(),ScoreKeeper::getInstance()->highSurviv[1].second,
		ScoreKeeper::getInstance()->highSurviv[2].first.c_str(),ScoreKeeper::getInstance()->highSurviv[2].second,
		ScoreKeeper::getInstance()->highSurviv[3].first.c_str(),ScoreKeeper::getInstance()->highSurviv[3].second,
		ScoreKeeper::getInstance()->highSurviv[4].first.c_str(),ScoreKeeper::getInstance()->highSurviv[4].second
		);
	            
	float width, height;
	m_spHUDFont->GetTextExtent(m_ScoreMsg->GetText(), width, height);

	unsigned int uiX = (m_app->DEFAULT_WIDTH/2)-(int)(width/2);
	unsigned int uiY = (m_app->DEFAULT_HEIGHT/2)-(int)(height/2);
	               
	m_ScoreMsg->SetPosition(uiX, uiY);
	m_ScoreMsg->Draw(pkRenderer);
}


