#ifndef Defines_H
#define Defines_H
#include <stdlib.h>
#include "ViconDataClient.h"

/*
 * Use this space to define any constants (i.e. numbers, variables, etc.)
 * Do NOT use this space to define or store Singletons
 */


static ViconDataClient client("158.130.2.253", false, true, true, false, false, ViconDataStreamSDK::CPP::StreamMode::ServerPush);

static int NUM_WALLS = 10;
static int randNum(){ return (rand() % NUM_WALLS + 2); }


enum ActiveState{aNewGame,aCollidedCont,aCollidedSurviv,aGameOver,aHighScores,aPaused,aWallMoving};
enum GameMode{Cont,Surviv};


#endif