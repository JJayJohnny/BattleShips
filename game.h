
#include "player.h"
#pragma once
#define REEF '#'

const int defaultBoardX = 10;
const int defaultBoardY = 21;

class gameState {
public:
	player players[2];
	int expectedPlayer = 0;
	coordinates field;
	int possibleReefs;
	coordinates* reefs;
	int reefCount = 0;
	int extendedShips = 0;
	void startGame();
	void setFleet();
	void statePrint(int printMode);
	void nextPlayer();
	void checkVictory();
	void setBoardSize();
	void initPosition();
	void placeReef();
	char** prepareBoard();
	void ship();
	char** prepareBoardAdvanced();
	void save();
};

int boardEmpty(char** board, int y, int x);


