/****************************************************
Flex - Senior Project
Copyright (C) 2010 Tamara Levy
*****************************************************/

#ifndef Defines_H
#define Defines_H
#include <stdlib.h>
#include "ViconDataClient.h"

/*
 * Use this space to define any constants (i.e. numbers, variables, enums, etc.)
 * only define the ViconClient here
 * Do NOT use this space to define or store Singletons
 */


static ViconDataClient client("158.130.2.253", false, true, true, false, false, ViconDataStreamSDK::CPP::StreamMode::ServerPush);

static int NUM_WALLS = 10;
static int NUM_SMASH = 3;
static int randNum(){ return (rand() % (NUM_WALLS+NUM_SMASH) + 2); }


enum ActiveState{aNewGame,aCollidedCont,aCollidedSurviv,aGameOver,aHighScores,aPausedCont,aPausedSurviv,aSmash,aGotHigh,aWallMoving};
enum GameMode{Cont,Surviv};


#endif