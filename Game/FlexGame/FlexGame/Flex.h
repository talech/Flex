#ifndef FLEX_H
#define FLEX_H

#define GAMEBRYO

#include <efdPhysX/PhysXSDKManager.h>
#include <stdio.h>
#include <NiApplication.h>
#include <NiPhysX.h>
#include <NiActorManager.h>
#include <NxUserContactReport.h>


#include <list>
#include <vector>
#include <iterator>
#include <algorithm>


#include "Defines.h"
#include "PlayerDisplay.h"
#include "ScreenOverlay.h"
#include "CameraController.h"


using namespace std;



class Flex : public NiApplication
{
public:
	Flex();
	~Flex();
	
	virtual bool Initialize();
    virtual void Terminate();
    virtual void UpdateFrame();
	virtual void ProcessInput();
	virtual void RenderFrame();
	virtual bool CreateScene();

	virtual bool CreateRenderer();


	void ResetWallPhysics();

	


protected:
	bool InitPhysics();
	bool InitCamera();
	bool InitPlayer();
	bool InitEnvironment();
    bool AddPhysicsProps(NiStream& kStream, NiPhysXPropPtr& propPtr);
	bool InitWalls(NiStream &kStream, NiPhysXPropPtr& propPtr);
    void SetWallPhysicsEnabled(bool b, bool force = false);
    void InitCollisionCallbacks();
	
	bool findPlayerActor(NxActor* actor);
	
	//Player related members
	Player* m_pPlayer;
	PlayerDisplay* m_playerDisplay;
	int totalFrame;
	map<string, int> actorSkeleton;
	void setSkeletonMap();

	// Keyboard
	NiInputKeyboard* pkKeyboard;

	

protected:

	// Physics contact callbacks
    class ContactReporter : public NxUserContactReport    
    {
    public:
        ContactReporter() : m_app(NULL) {}    
        void onContactNotify(NxContactPair& pair, NxU32 events)
        {
            if (m_app) m_app->processContacts(pair, events);
        }
        Flex* m_app;    
    };
    void processContacts(NxContactPair& pair, NxU32 events);

protected:

	NiAlphaAccumulator* pkAccum;
    CameraController* m_pCameraController;
	vector<NxActor*> m_player;
    bool m_wallPhysicsEnabled;

    // Physics related members
    efdPhysX::PhysXSDKManager* m_pkPhysManager;
    NiPhysXScenePtr m_spPhysScene;
    ContactReporter m_PhysContactReporter;
    float fLastSimTime;
	
    
};

#endif