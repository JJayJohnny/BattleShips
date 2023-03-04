#include "ship.h"
#pragma once

class player {
public:
	/*ship* CAR;
	ship* BAT;
	ship* CRU;
	ship* DES;*/
	ship* ships = nullptr;
	int fleet[4] = { 1, 2, 3, 4 };
	int shootsLeft;
	int yMin;
	int yMax;
	int xMin;
	int xMax;
	int shipsLeftToPlace = 10;
	coordinates* spyPlanes = nullptr;
	int spyPlanesCount = 0;
	void placeShip(char** board, int boardX, int boardY, int ifFromState, char player);
	void shoot(player otherPlayer, int maxX, int maxY);
	int findShipByIdAndSize(int size, int id);
	int countAliveParts();
	int countShipsByType(char type[4]);
	void initShips();
	void newRound();
	void spy(int boardX, int boardY);
	void print(char** board, int boardX, int boardY, int printMode);
};

