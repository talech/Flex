#ifndef FLEX_H
#define FLEX_H

#define GAMEBRYO

#include <stdio.h>

#include "PlayerDisplay.h"


class Flex : public NiApplication
{
public:
	Flex();
	~Flex();

	virtual bool CreateScene();

	virtual void UpdateFrame();

protected:
	
	Player* m_pPlayer;
	PlayerDisplay* m_playerDisplay;
	
    
};

#endif