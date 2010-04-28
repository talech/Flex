#include "ScoreKeeper.h"
#include "GameStateManager.h"
#include "defines.h"
#include "GotHigh.h"

ScoreKeeper ScoreKeeper::mKeeper;

ScoreKeeper::~ScoreKeeper(){
	walls = 0;
	score = 0;
	lives = 0;
	smash = 0;
	time = 0;
	initTime = 0;
}

void 
ScoreKeeper::resetGame(){
	walls = 0;
	score = 0;
	lives = 3;
	smash = 0;
	time = 0;
}

void
ScoreKeeper::smashToggle(bool on){
	if(!on)
		smash = 0;
}

void
ScoreKeeper::scoreFirstHit(){
	score += 10;
	smash += 10;
}

void
ScoreKeeper::scoreHit(){
	score += 1;
	smash += 1;
}

void
ScoreKeeper::scoreWall(){
	walls += 1;
}

void
ScoreKeeper::loseLife(){
	lives -= 1;
}

int
ScoreKeeper::getWalls(){
	return walls;
}

int
ScoreKeeper::getScore(){
	return score+walls;
}

int 
ScoreKeeper::getLives(){
	return lives;
}

int
ScoreKeeper::getSmashScore(){
	return smash;
}

float 
ScoreKeeper::getTime(){
	return time;
}

void
ScoreKeeper::startGame(float dtime){
	initTime = dtime;
}

void
ScoreKeeper::endGame(float dtime){
	time = dtime - initTime;
}

bool 
ScoreKeeper::isHigh(){
	int myScore = score+walls;
	if(GameStateManager::getInstance()->mode == Cont){
		myScore = myScore*time;
		if(myScore > highCont[4].second)
			return true;
	}
	else{
		if(myScore > highSurviv[4].second)
			return true;
	}
	return false;
}

void
ScoreKeeper::insertName(){
	string name = (string)GotHigh::getInstance()->getName();
	int myScore = score+walls;
	if(GameStateManager::getInstance()->mode == Cont){
		myScore = myScore*time;
		for(int i = 4; i>-1; i--){
			if(myScore > highCont[i].second){
				if(i<4){
					highCont[i+1] = highCont[i];
				}
				highCont[i] = make_pair(name,myScore);
			}
		}
	}
	else{
		for(int i = 4; i>-1; i--){
			if(myScore > highSurviv[i].second){
				if(i<4){
					highSurviv[i+1] = highSurviv[i];
				}
				highSurviv[i] = make_pair(name,myScore);
			}
		}
	}

	writeScores();
}

void 
ScoreKeeper::readScores(){
	string line;
	int mode = 0;
	int index = 0;

	ifstream myfile ("HighScores.txt");

	if (myfile.is_open())
	{
		while (! myfile.eof() )
		{
			getline (myfile,line);
			if(strcmp(line.c_str(),"*Cont")==0) mode = 1;
			else if(strcmp(line.c_str(),"*Surviv")==0){
				mode = 2;
				index = 0;
			}
			else{
				string::size_type position = line.find("-.");
				string name = line.substr(0,position);
				string number = line.substr(position+2,line.size());
				name.append("\0");
				int value = atoi(number.c_str());

				if(mode == 1){
					highCont[index] = make_pair(name,value);
					index++;
				}
				else if(mode == 2){
					highSurviv[index] = make_pair(name,value);
					index++;
				}
			}
		}
		myfile.close();
	}
	else cout << "Unable to open file"; 
}

void
ScoreKeeper::writeScores(){
	ofstream myfile;
	myfile.open ("HighScores2.txt");
	char* text;
	sprintf(text,"*Cont\n%s-.%d\n%s-.%d\n%s-.%d\n%s-.%d\n%s-.%d\n*Surviv\n%s-.%d\n%s-.%d\n%s-.%d\n%s-.%d\n%s-.%d",
		highCont[0].first.c_str(),highCont[0].second,
		highCont[1].first.c_str(),highCont[1].second,
		highCont[2].first.c_str(),highCont[2].second,
		highCont[3].first.c_str(),highCont[3].second,
		highCont[4].first.c_str(),highCont[4].second,
		highSurviv[0].first.c_str(),highSurviv[0].second,
		highSurviv[1].first.c_str(),highSurviv[1].second,
		highSurviv[2].first.c_str(),highSurviv[2].second,
		highSurviv[3].first.c_str(),highSurviv[3].second,
		highSurviv[4].first.c_str(),highSurviv[4].second);
	myfile << text;
	myfile.close();
}


