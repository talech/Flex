#ifndef SCREENOVERLAY_H
#define SCREENOVERLAY_H

#include <NiSystem.h>
#include <NiFont.h>
#include <Ni2DString.h>
#include <vector>

#include "ScoreKeeper.h"
#include "GameStateManager.h"

class Flex;
class ScreenMessage;

enum overType{msg,lives};

class ScreenOverlay : public NiMemObject
{
public:

    ScreenOverlay(Flex* app);
    ~ScreenOverlay();

    static bool Create(Flex* app);
    static void Destroy();
    static ScreenOverlay* Get();

    void Draw();
    //const char* GetActiveButton() const;

protected:

	void displayMessages(NiRenderer* pkRenderer, ActiveState state); 
    void displayScore(NiRenderer* pkRenderer, int score); 

    NiFontPtr m_spHUDFont;
	Ni2DStringPtr m_Score;

    ActiveState m_currentState;
    vector<ScreenMessage*> m_messages;    
	vector<ScreenMessage*> m_lives;
    Flex* m_app;

private:
    // This is a singleton class
    static ScreenOverlay* ms_pkTheScreenOverlay;
};

inline ScreenOverlay* ScreenOverlay::Get()
{
    return ms_pkTheScreenOverlay;
}

#endif
