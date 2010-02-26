
#ifndef FractureTestApp_H
#define FractureTestApp_H

#include <NiApplication.h>
#include <NiActorManager.h>
#include <NxUserContactReport.h>
#include <NiPhysX.h>
#include <list>
#include <vector>
#include "CameraController.h"

class FractureTestApp : public NiApplication
{
public:
    FractureTestApp();
    
    virtual bool Initialize();
    virtual bool CreateScene();
    virtual void UpdateFrame();
    virtual void Terminate();
    virtual void RenderFrame();
    virtual void Reset();
             
protected:
    NiCamera* FindCamera(NiAVObject* pkObject);
    bool InitPhysics();
	bool InitCamera();
    bool AddPhysicsProps(NiStream& kStream, NiPhysXPropPtr& propPtr);
    bool InitEnvironment();
    void SetWallPhysicsEnabled(bool b, bool force = false);
    void InitCollisionCallbacks();

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
        FractureTestApp* m_app;    
    };
    void processContacts(NxContactPair& pair, NxU32 events);

protected:

    CameraController* m_pCameraController;
    NxActor* m_ball;
    bool m_wallPhysicsEnabled;

    // Physics 
    NiPhysXManager* m_pkPhysManager;
    NiPhysXScenePtr m_spPhysScene;
    ContactReporter m_PhysContactReporter;
    float fLastSimTime;
};

#endif
