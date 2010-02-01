#ifndef FLEX_H
#define FLEX_H

#define GAMEBRYO

#include <stdio.h>
#include <NiSample.h>
#include "PlayerDisplay.h"


class Flex : public NiSample
{
public:
	Flex();
	~Flex();

	virtual bool CreateScene();
    virtual bool CreateUIElements();

protected:
	
    
};

#endif