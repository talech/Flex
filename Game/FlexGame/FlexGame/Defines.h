#ifndef Defines_H
#define Defines_H
#include <stdlib.h>
#include "ViconDataClient.h"

/*
 * Use this space to define any constants (i.e. numbers, variables, etc.)
 * Do NOT use this space to define or store Singletons
 */


//static ViconDataClient client("158.130.2.253", false);

static int randNum(){ return (rand() % 5 + 2); }

enum ActiveState{aNewGame,aCollided,aGameOver,aWallMoving};


#endif