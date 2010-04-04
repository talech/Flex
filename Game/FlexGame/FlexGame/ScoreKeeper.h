/****************************************************
Tamara Levy - 2010
ScoreKeeper - 
singleton that manages the score and lives
****************************************************/

#ifndef ScoreKeeper_H
#define ScoreKeeper_H

#include "Defines.h"

class ScoreKeeper
{
public:

	~ScoreKeeper();

	void scoreWall();
	void loseLife();

	int getScore();
	int getLives();

	void update( float delTime );

	static ScoreKeeper* getInstance() { return &mKeeper; }


private:

	int score;
	int lives;

	static ScoreKeeper mKeeper;

	ScoreKeeper() {score = 0; lives = 3;}

	
};
#endif