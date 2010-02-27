#ifndef PLAYERDISPLAY_H
#define PLAYERDISPLAY_H


#include <Transformation.h>
#include <Player.h>

#include <vector>
#include <NiApplication.h>
#include <efdPhysX/PhysXSDKManager.h>
#include <NiPhysX.h>

typedef NiInputMouse Mouse;
typedef NiInputKeyboard Keyboard;

using namespace std;



class PlayerDisplay{
public:
	PlayerDisplay(Player* pPlayer, NiNodePtr sScene, NiPhysXScenePtr physScene);
	~PlayerDisplay();

	virtual void Update();
	virtual const Player* GetPlayer(){return m_Player;}
	Player* m_Player;

protected:
	void init();

	virtual void DrawPlayer(Player* player);
	virtual void DrawActorRec(Player* player, Joint* pJoint);



private:
	NiPhysXProp* spPlayerProp;
	NiNodePtr scene;

	bool isChild;
	float Deg2Rad(float deg){return (deg*(3.14159265/180.0));}
};

#endif