#include "GotHigh.h"
#include "NewGameMenu.h"
#include "ScoreKeeper.h"
#include <math.h>

GotHigh GotHigh::mGotHigh;

GotHigh::~GotHigh()
{
}

void GotHigh::enter()
{
	GameStateManager::getInstance()->state = aGotHigh;
	index = 0;
	name.resize(5);
	name = "_____";
		
}

void GotHigh::exit()
{
	
}

void GotHigh::pause()
{
}

void GotHigh::resume()
{
}

void GotHigh::processMouse(Mouse *mouse)
{
	
	
}

void GotHigh::processKeyboard(Keyboard *keyboard)
{
	if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_RETURN)){
		for(int i = index; i<5; i++){
			name.replace(i,1,"_");
		}
		name.append("\0");
		GameStateManager::getInstance()->state = aNewGame;
		ScoreKeeper::getInstance()->insertName();
		ScoreKeeper::getInstance()->resetGame();
		GameStateManager::getInstance()->changeState(NewGameMenu::getInstance());
	}
	else{
		ProcessWriting(keyboard);
	}
}

void GotHigh::processGamePad(GamePad *gamepad)
{
}

void GotHigh::update(float delTime)
{
	/*NILOG(NIMESSAGE_GENERAL_0, "name: %s, size: %d", 
			name.c_str(), name.size());*/
	
}

void GotHigh::AddToString(char* letter){
	name.replace(index,1,letter);
	index++;
}

void GotHigh::ProcessWriting(Keyboard *keyboard){
	if(index<5){
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_1))
			AddToString((char*)("1"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_2))
			AddToString((char*)("2"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_3))
			AddToString((char*)("3"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_4))
			AddToString((char*)("4"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_5))
			AddToString((char*)("5"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_6))
			AddToString((char*)("6"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_7))
			AddToString((char*)("7"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_8))
			AddToString((char*)("8"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_9))
			AddToString((char*)("9"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_0))
			AddToString((char*)("0"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_EQUALS))
			AddToString((char*)("="));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_Q))
			AddToString((char*)("Q"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_W))
			AddToString((char*)("W"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_E))
			AddToString((char*)("E"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_R))
			AddToString((char*)("R"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_T))
			AddToString((char*)("T"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_Y))
			AddToString((char*)("Y"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_U))
			AddToString((char*)("U"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_I))
			AddToString((char*)("I"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_O))
			AddToString((char*)("O"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_P))
			AddToString((char*)("P"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_A))
			AddToString((char*)("A"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_S))
			AddToString((char*)("S"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_D))
			AddToString((char*)("D"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_F))
			AddToString((char*)("F"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_G))
			AddToString((char*)("G"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_H))
			AddToString((char*)("H"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_J))
			AddToString((char*)("J"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_K))
			AddToString((char*)("K"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_L))
			AddToString((char*)("L"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_APOSTROPHE))
			AddToString((char*)("'"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_Z))
			AddToString((char*)("Z"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_X))
			AddToString((char*)("X"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_C))
			AddToString((char*)("C"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_V))
			AddToString((char*)("V"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_B))
			AddToString((char*)("B"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_N))
			AddToString((char*)("N"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_M))
			AddToString((char*)("M"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_COMMA))
			AddToString((char*)(","));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_SLASH))
			AddToString((char*)("/"));
		if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_MULTIPLY))
			AddToString((char*)("*"));

	}
	//DELETE
	if (keyboard->KeyWasReleased(NiInputKeyboard::KEY_BACK)){			
		if(index>0){
			index--;
			name.replace(index,1,"_");
		}
	}
}