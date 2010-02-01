#ifndef PLAYERDISPLAY_H
#define PLAYERDISPLAY_H


#include <Transformation.h>
#include <Player.h>

#include <vector>
#include <NiApplication.h>

using namespace std;

class PlayerDisplay{
public:
	PlayerDisplay(Player* pPlayer, NiNodePtr sScene);
	~PlayerDisplay();

	virtual void Update();
	virtual const Player* GetPlayer(){return m_Player;}

protected:
	void init();

	virtual void DrawPlayer(Player* player);
	virtual void DrawActorRec(Player* player, Joint* pJoint);


private:
	Player* m_Player;
	NiNodePtr scene;

};

#endif