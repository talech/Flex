#include "CameraController.h"
#include "NiApplication.h"
#include <NiAVObject.h>
#include <algorithm>

NiImplementRTTI(CameraController, NiTimeController);

NiPoint3 CameraController::dfltEye(20.0f, 10.0f, 20.0f);
NiPoint3 CameraController::dfltUp(0.0f, 1.0f, 0.0f);
NiPoint3 CameraController::dfltLook(0.0f, 0.0f, 0.0f);

CameraController::CameraController(NiApplication *pkApp, NiAVObject* cameraRoot) : 
	m_pkGameApp(pkApp)
{
    mSpeed = 0.1;
    mTurnRate = 0.1*(NI_PI/180.0);
	SetTarget(cameraRoot);
	Reset();
}

bool CameraController::TargetIsRequiredType() const
{
    return NiIsKindOf(NiAVObject, m_pkTarget);
}

void CameraController::Update(float fTime)
{   
    if (!m_pkTarget) return;
    
    NiAVObject* pkObject = (NiAVObject*)m_pkTarget;
    NIASSERT(NiIsKindOf(NiAVObject, pkObject));

	NiInputMouse* mouse = m_pkGameApp->GetInputSystem()->GetMouse();
	if (!mouse)
	{
		NILOG(NIMESSAGE_GENERAL_0, "ERROR: MOUSE NOT FOUND\n");
		return;
	}

	int deltaX, deltaY, deltaZ;
	mouse->GetPositionDelta(NiInputMouse::NIM_AXIS_X, deltaX);
	mouse->GetPositionDelta(NiInputMouse::NIM_AXIS_Y, deltaY);
	mouse->GetPositionDelta(NiInputMouse::NIM_AXIS_Z, deltaZ);  // mouse wheel

	unsigned int modifiers = mouse->GetModifiers();
	if ((modifiers & NiInputMouse::MMOD_LMENU) != 0 ||
		(modifiers & NiInputMouse::MMOD_LCONTROL) != 0) return;

	bool moveLeftRight = abs(deltaX) > abs(deltaY);
	bool moveUpDown = !moveLeftRight;

	if (mouse->ButtonIsDown(NiInputMouse::NIM_LEFT))  // orbit
	{
		if (moveLeftRight && deltaX > 0) orbitLeft(deltaX);
		else if (moveLeftRight && deltaX < 0) orbitRight(-deltaX);
		else if (moveUpDown && deltaY > 0) orbitUp(deltaY);
		else if (moveUpDown && deltaY < 0) orbitDown(-deltaY); 
	}
    else if (mouse->ButtonIsDown(NiInputMouse::NIM_MIDDLE))  // pan
    {    
		if (moveLeftRight && deltaX > 0) moveLeft(deltaX);
		else if (moveLeftRight && deltaX < 0) moveRight(-deltaX);
		else if (moveUpDown && deltaY > 0) moveUp(deltaY);
		else if (moveUpDown && deltaY < 0) moveDown(-deltaY);    
    }
    else if (mouse->ButtonIsDown(NiInputMouse::NIM_RIGHT))  // zoom
    {    
		if (moveUpDown && deltaY > 0) moveForward(deltaY);
		else if (moveUpDown && deltaY < 0) moveBack(-deltaY);
	}

	if (deltaZ > 0) moveForward(deltaZ);
	else if (deltaZ < 0) moveBack(-deltaZ);

	NiMatrix3 rot(-n,u,v);
	pkObject->SetTranslate(eye);
	pkObject->SetRotate(rot);

    //NILOG(NIMESSAGE_GENERAL_0, "CAMERA: eye: (%f,%f,%f)\n", 
     //       eye[0], eye[1], eye[2]);
}

void CameraController::Reset()
{
   // Calculate the initial heading & pitch
   // Note that  eye[0] = radius*cos(h)*cos(p); and  eye[1] = radius*sin(p);
   mPitch = -asin(dfltEye[1]/dfltEye.Length());
   mHeading = acos(dfltEye[0]/(dfltEye.Length()*cos(mPitch))) + NI_PI;

   Set(dfltEye, dfltLook, dfltUp);
}

void CameraController::ShowTopView()
{
	mRadius = 6120;
    look = NiPoint3(0,0,0);
	Orbit(4.710644, -1.557306);
}

void CameraController::Set(const NiPoint3& eyepos, const NiPoint3& lookpos, const NiPoint3& up)
{
   look = lookpos;
   eye = eyepos;
   n = eyepos - look;
   v = up.Cross(n);
   u = n.Cross(v);
   mRadius = n.Length(); // cache this distance

   u.Unitize();
   v.Unitize();
   n.Unitize();
}

void CameraController::Move(float dV, float dU, float dN)
{
   eye += dU*u + dV*v + dN*n;
   look += dU*u + dV*v + dN*n;
   //look[1] = 0; // Clamp on ground
}

void CameraController::Orbit(float h, float p)
{
   NiPoint3 rotatePt; // Calculate new location around sphere having mRadius
   rotatePt[0] = mRadius*cos(h)*cos(p);
   rotatePt[1] = mRadius*sin(p);
   rotatePt[2] = mRadius*sin(h)*cos(p);

   Set(look-rotatePt, look /* look */, NiPoint3(0,1,0) /* up Approx */);

   //NILOG(NIMESSAGE_GENERAL_0, "H: %f, P: %f, RADIUS: %f\n", h, p, mRadius);
}

void CameraController::orbitLeft(int scale) 
{
   mHeading += mTurnRate*scale;
   Orbit(mHeading, mPitch);
}

void CameraController::moveLeft(int scale) // => move along v
{    
   Move(-mSpeed*scale, 0.0, 0.0);
}

void CameraController::orbitRight(int scale)
{
   mHeading -= mTurnRate*scale;
   Orbit(mHeading, mPitch);
}

void CameraController::moveRight(int scale) // => move along v
{
   Move(mSpeed*scale, 0.0, 0.0);   
}

void CameraController::orbitUp(int scale)
{
	//mPitch = std::min(NI_PI/2.0f-0.01f, mPitch + mTurnRate*scale);
	mPitch = std::min(-0.01f, mPitch + mTurnRate*scale);
    Orbit(mHeading, mPitch);
}

void CameraController::moveUp(int scale) // => move along +u
{
   Move(0.0, mSpeed*scale, 0.0);   
}

void CameraController::orbitDown(int scale)
{
	mPitch = std::max(-NI_PI/2.0f+0.01f, mPitch - mTurnRate*scale);
   Orbit(mHeading, mPitch);
}

void CameraController::moveDown(int scale) // => move along -u
{
   Move(0.0, -mSpeed*scale, 0.0);   
}

void CameraController::moveForward(int scale) // => move along -n
{
   //Move(0.0, 0.0, -mSpeed*scale);      
   mRadius -= mSpeed*scale;  // Also "zoom" into radius
   Orbit(mHeading, mPitch);
}

void CameraController::moveBack(int scale) // => move along n
{
   //Move(0.0, 0.0, mSpeed*scale);   
   mRadius += mSpeed*scale;  // Also "zoom" out radius
   Orbit(mHeading, mPitch);
}