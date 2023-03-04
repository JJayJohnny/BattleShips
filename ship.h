#pragma once

#define HEALTHY 1
#define DESTROYED 0
#define FOG '?'
#define CANNON '!'
#define ENGINE '%'
#define RADAR '@'

const char segmentStates[2] = {'x', '+'};

struct coordinates {
	int x;
	int y;
};

class ship {
public:
	int size;
	int id = -1;
	coordinates position;
	char direction;
	char type[4];
	int* segments;
	int status = 0;
	int countAliveSegments();
	void move(char** board, int boardX, int boardY, int extendedShips);
	void shoot(ship* opponent, int shipCount, int boardX, int boardY);
	int movesLeft;
	int shootsLeft;
};

int giveSizeFromType(char type[4]);



