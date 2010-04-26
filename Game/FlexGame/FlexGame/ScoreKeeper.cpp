#include "ScoreKeeper.h"

ScoreKeeper ScoreKeeper::mKeeper;

ScoreKeeper::~ScoreKeeper(){
	score = 0;
	lives = 0;
}

void 
ScoreKeeper::resetGame(){
	score = 0;
	lives = 3;
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

