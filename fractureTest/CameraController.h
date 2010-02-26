#ifndef CameraController_H_
#define CameraController_H_

#include <NiTimeController.h>
#include <NiPoint3.h>

class NiApplication;
class NiInputKeyboard;

class CameraController : public NiTimeController 
{
    NiDeclareRTTI;

public:
    CameraController(NiApplication *pkApp, NiAVObject* cameraRoot);
    virtual void Update(float fTime);
    virtual void Set(const NiPoint3& eyepos, const NiPoint3& look, const NiPoint3& up);
	virtual void Reset();
	virtual void ShowTopView();

protected:
    virtual bool TargetIsRequiredType () const;
	virtual void Orbit(float h, float p);
	virtual void Move(float dV, float dU, float dN);

	virtual void orbitLeft(int scale = 1.0);
	virtual void orbitRight(int scale = 1.0);
	virtual void orbitUp(int scale = 1.0);
	virtual void orbitDown(int scale = 1.0);
	virtual void moveLeft(int scale = 1.0);
	virtual void moveRight(int scale = 1.0);
	virtual void moveUp(int scale = 1.0);
	virtual void moveDown(int scale = 1.0);
	virtual void moveForward(int scale = 1.0);
	virtual void moveBack(int scale = 1.0);

    NiApplication* m_pkGameApp;
    float m_fLastUpdateTime;
public:
    NiPoint3 eye; // camera position
    NiPoint3 look;
    float mHeading, mPitch, mRadius;
	float mSpeed, mTurnRate;
   
    // Basis of camera local coord system
    NiPoint3 u; // up
    NiPoint3 v; // v points right
    NiPoint3 n; // -n points forward

public:
	static NiPoint3 dfltEye, dfltUp, dfltLook;
};

NiSmartPointer(CameraController);

#endif