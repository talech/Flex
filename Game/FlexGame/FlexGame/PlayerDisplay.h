#ifndef PLAYERDISPLAY_H
#define PLAYERDISPLAY_H


#include <Transformation.h>
#include <Player.h>

#include <vector>
#include <iostream>
#include <map>
#include <string>
#include <NiApplication.h>
#include <efdPhysX/PhysXSDKManager.h>
#include <NiPhysX.h>

#include "Defines.h"


typedef NiInputMouse Mouse;
typedef NiInputKeyboard Keyboard;


using namespace std;

class ViconDataClient;




class PlayerDisplay{
public:
	PlayerDisplay(Player* pPlayer, NiNodePtr sScene, NiPhysXScenePtr physScene);
	~PlayerDisplay();

	virtual void Update();
	void processKeyboard(Keyboard *keyboard);
	virtual const Player* GetPlayer(){return m_Player;}
	Player* m_Player;

	map<string, int> actorSkeleton;

protected:
	void init();

	virtual void DrawPlayer(Player* player);
	virtual void DrawActorRec(Player* player, Joint* pJoint);

	virtual void DataStream();
	virtual void DrawMarkers();



private:
	// Vicon networked client
	//ViconDataClient client; //("158.130.2.19", false);

	NiPhysXProp* spPlayerProp;
	NiNodePtr scene;
	Motion* motions[4];

	vector<vec3> unlabeledMarkers;
	map<string,vec3> markers;
	map<string,vec3> markersHead;

	bool isChild;
	bool playing;
	float Deg2Rad(float deg){return (deg*(3.14159265/180.0));}
};

#endif