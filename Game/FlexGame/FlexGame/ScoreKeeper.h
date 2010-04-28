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

	void resetGame();
	void scoreWall();
	void loseLife();
	void scoreFirstHit();
	void scoreHit();
	void smashToggle(bool on);
	void startGame(float dtime);
	void endGame(float dtime);

	int getWalls();
	int getScore();
	int getLives();
	int getSmashScore();
	float getTime();

	bool isHigh();

	void insertName();
	void writeScores();

	void update( float delTime );

	static ScoreKeeper* getInstance() { return &mKeeper; }

	//High Scores
	pair<string,int> highCont[5];
	pair<string,int> highSurviv[5];


private:

	int walls;
	int score;
	int lives;
	int smash;
	float time;
	float initTime;


	static ScoreKeeper mKeeper;

	void readScores();
	

	ScoreKeeper() {score = 0; lives = 3; walls = 0; smash = 0; time = 0; initTime = 0; readScores();}

	
};
#endif