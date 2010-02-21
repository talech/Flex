#ifndef FLEX_H
#define FLEX_H

#define GAMEBRYO

#include <efdPhysX/PhysXSDKManager.h>
#include <stdio.h>
#include <NiApplication.h>
#include <NiPhysX.h>

#include "PlayerDisplay.h"
#include "GameStateManager.h"


class Flex : public NiApplication
{
public:
	Flex();
	~Flex();

	bool Initialize();
    void Terminate();
    void UpdateFrame();
	void ProcessInput();

	void SetCamera( NiCameraPtr cam );

protected:
	bool CreateScene();
	void PlayMotion();

	// Remap the keys used to control the view.
    void SetTurretControls();

	// Physics related members
    efdPhysX::PhysXSDKManager* m_pkPhysManager;
    NiPhysXScenePtr m_spPhysScene;
	
	//Player related members
	Player* m_pPlayer;
	PlayerDisplay* m_playerDisplay;
	int totalFrame;

	// Keyboard and gamepad handling for the camera.
    NiTurret m_kTurret;
    NiNodePtr m_spTrnNode;
    NiNodePtr m_spRotNode;


	NiInputKeyboard* pkKeyboard;
	
    
};

#endif