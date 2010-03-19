#ifndef SCREENOVERLAY_H
#define SCREENOVERLAY_H

#include <NiSystem.h>
#include <NiFont.h>
#include <Ni2DString.h>
#include <vector>

class Flex;
class ScreenMessage;



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
    //void displayButtons(NiRenderer* pkRenderer, const char* name); 

    //NiFontPtr m_spHUDFont;
    ActiveState m_currentState;
    std::vector<ScreenMessage*> m_messages;    
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
