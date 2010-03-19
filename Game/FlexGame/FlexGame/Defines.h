#ifndef Defines_H
#define Defines_H
#include <stdlib.h>

/*
 * Use this space to define any constants (i.e. numbers, variables, etc.)
 * Do NOT use this space to define or store Singletons
 */


static int randNum(){ return (rand() % 5 + 2); }
enum ActiveState{aNewGame,aCollided,aGameOver,aWallMoving};


#endif