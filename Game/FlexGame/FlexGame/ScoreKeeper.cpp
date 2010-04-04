#include "ScoreKeeper.h"

ScoreKeeper ScoreKeeper::mKeeper;

ScoreKeeper::~ScoreKeeper(){
	score = 0;
	lives = 0;
}

void
ScoreKeeper::scoreWall(){
	score += 1;
}

void
ScoreKeeper::loseLife(){
	lives -= 1;
}

int
ScoreKeeper::getScore(){
	return score;
}

int 
ScoreKeeper::getLives(){
	return lives;
}