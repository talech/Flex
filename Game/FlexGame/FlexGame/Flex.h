#ifndef FLEX_H
#define FLEX_H

#define GAMEBRYO

#include <stdio.h>
#include <NiApplication.h>

#include "PlayerDisplay.h"


class Flex : public NiApplication
{
public:
	Flex();
	~Flex();

	virtual bool CreateScene();

	virtual void UpdateFrame();

protected:
	// Remap the keys used to control the view.
    void SetTurretControls();


	Player* m_pPlayer;
	PlayerDisplay* m_playerDisplay;

	// Keyboard and gamepad handling for the camera.
    NiTurret m_kTurret;
    NiNodePtr m_spTrnNode;
    NiNodePtr m_spRotNode;
	
    
};

#endif