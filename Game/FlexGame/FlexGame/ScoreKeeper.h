/****************************************************
Tamara Levy - 2010
ScoreKeeper - 
singleton that manages the score and lives
****************************************************/

#ifndef ScoreKeeper_H
#define ScoreKeeper_H

#include <utility>
#include <string>
#include <iostream>
#include <fstream>

#include "Defines.h"

using namespace std;

class ScoreKeeper
{
public:

	~ScoreKeeper();

	void scoreWall();
	void loseLife();

	int getScore();
	int getLives();

	bool isHigh();

	void update( float delTime );

	static ScoreKeeper* getInstance() { return &mKeeper; }

	//High Scores
	pair<string,int> highCont[5];
	pair<string,int> highSurviv[5];


private:

	int score;
	int lives;

	static ScoreKeeper mKeeper;

	void readScores();
	void writeScores();

	ScoreKeeper() {score = 0; lives = 3; readScores();}

	
};
#endif