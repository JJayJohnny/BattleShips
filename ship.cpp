#include "ship.h"
#include "player.h"
#include "game.h"
#include <string.h>
#include <iostream>

using namespace std;

int giveSizeFromType(char type[4])
{
	if (strcmp(type, "CAR") == 0)
	{
		return 5;
	}
	else if (strcmp(type, "BAT") == 0)
	{

		return 4;
	}
	else if (strcmp(type, "CRU") == 0)
	{

		return 3;
	}
	else if (strcmp(type, "DES") == 0){

		return 2;
	}
	return 0;
}

int ship::countAliveSegments()
{
	int counter = 0;
	for (int i = 0; i < size; i++)
		if (segments[i] == 1)
			counter++;
	return counter;
}

void ship::move(char** board, int boardX, int boardY, int extendedShips)
{
	char moveDir;
	cin >> moveDir;
	int newX = 0, newY = 0;
	char newDir = 0;
	if (extendedShips && segments[size - 1])
	{
		cout << "INVALID OPERATION \"MOVE " << id << " " << type << " " << moveDir << "\": SHIP CANNOT MOVE" << endl;
		exit(0);
	}
	if (movesLeft == 0)
	{
		cout << "INVALID OPERATION \"MOVE " << id << " " << type << " " << moveDir << "\": SHIP MOVED ALREADY" << endl;
		exit(0);
	}
	if (moveDir == 'F')
	{
		newDir = direction;
		switch (direction)
		{
		case 'N':
			newY = position.y - 1;
			newX = position.x;
			break;
		case 'S':
			newY = position.y + 1;
			newX = position.x;
			break;
		case 'E':
			newY = position.y;
			newX = position.x + 1;
			break;
		case 'W':
			newY = position.y;
			newX = position.x - 1;
			break;
		}
	}
	else if (moveDir == 'L')
	{
		switch (direction)
		{
		case 'N':
			newDir = 'W';
			newY = position.y - 1;
			newX = position.x - (size-1);
			break;
		case 'S':
			newDir = 'E';
			newY = position.y + 1;
			newX = position.x + (size - 1);
			break;
		case 'E':
			newDir = 'N';
			newY = position.y - (size - 1);
			newX = position.x + 1;
			break;
		case 'W':
			newDir = 'S';
			newY = position.y + (size - 1);
			newX = position.x - 1;
			break;
		}
	}
	else if (moveDir == 'R')
	{
		switch (direction)
		{
		case 'N':
			newDir = 'E';
			newY = position.y - 1;
			newX = position.x + (size - 1);
			break;
		case 'S':
			newDir = 'W';
			newY = position.y + 1;
			newX = position.x - (size - 1);
			break;
		case 'E':
			newDir = 'S';
			newY = position.y + (size - 1);
			newX = position.x + 1;
			break;
		case 'W':
			newDir = 'N';
			newY = position.y - (size - 1);
			newX = position.x - 1;
			break;
		}
	}

	for (int i = 0; i < size; i++)//usuniecie ruszanego statku z planszy aby nie kolidowal sam ze soba
	{
		int deleteX=0, deleteY=0;
		switch (direction)
		{
		case 'N':
			deleteX = position.x;
			deleteY = position.y + i;
			break;
		case 'S':
			deleteX = position.x;
			deleteY = position.y - i;
			break;
		case'E':
			deleteX = position.x - i;
			deleteY = position.y;
			break;
		case'W':
			deleteX = position.x + i;
			deleteY = position.y;
			break;
		}
		board[deleteY][deleteX] = ' ';
	}

	if (newY < 0 || newY >= boardY || newX < 0 || newX >= boardX || (newDir == 'N' && newY + size -1 >= boardY) || (newDir == 'E' && newX - size + 1 < 0) || (newDir == 'S' && newY - size + 1 < 0) || (newDir == 'W' && newX + size - 1 >= boardX))
	{
		cout << "INVALID OPERATION \"MOVE " << id << " " << type << " " << moveDir << "\": SHIP WENT FROM BOARD" << endl;
		exit(0);
	}
	for (int i = 0; i < size; i++)
	{
		int checkX=0, checkY=0;
		switch (newDir)
		{
		case 'N':
			checkX = newX;
			checkY = newY + i;
			break;
		case 'S':
			checkX = newX;
			checkY = newY - i;
			break;
		case'E':
			checkX = newX - i;
			checkY = newY;
			break;
		case'W':
			checkX = newX + i;
			checkY = newY;
			break;
		}
		if (board[checkY][checkX] == REEF)//check if ship on reef
		{
			cout << "INVALID OPERATION \"MOVE " << id << " " << type << " " << moveDir << "\": PLACING SHIP ON REEF" << endl;
			exit(0);
		}
		if (board[checkY][checkX] == segmentStates[1] || board[checkY][checkX] == segmentStates[0])
		{
			cout << "INVALID OPERATION \"MOVE " << id << " " << type << " " << moveDir << "\": PLACING SHIP TOO CLOSE TO OTHER SHIP" << endl;
			exit(0);
		}
		else if ((checkX - 1 >= 0) && board[checkY][checkX - 1] == segmentStates[1])//check 4 sides for other ships nearby
		{
			cout << "INVALID OPERATION \"MOVE " << id << " " << type << " " << moveDir << "\": PLACING SHIP TOO CLOSE TO OTHER SHIP" << endl;
			exit(0);
		}
		else if ((checkX + 1 < boardX) && board[checkY][checkX + 1] == segmentStates[1])
		{
			cout << "INVALID OPERATION \"MOVE " << id << " " << type << " " << moveDir << "\": PLACING SHIP TOO CLOSE TO OTHER SHIP" << endl;
			exit(0);
		}
		else if ((checkY - 1 >= 0) && board[checkY - 1][checkX] == segmentStates[1])
		{
			cout << "INVALID OPERATION \"MOVE " << id << " " << type << " " << moveDir << "\": PLACING SHIP TOO CLOSE TO OTHER SHIP" << endl;
			exit(0);
		}
		else if ((checkY + 1 < boardY) && board[checkY + 1][checkX] == segmentStates[1])
		{
			cout << "INVALID OPERATION \"MOVE " << id << " " << type << " " << moveDir << "\": PLACING SHIP TOO CLOSE TO OTHER SHIP" << endl;
			exit(0);
		}
	}
	direction = newDir;
	position.x = newX;
	position.y = newY;
	movesLeft--;
}

void ship::shoot(ship* opponent, int shipCount, int boardX, int boardY)
{
	int y, x;
	cin >> y >> x;
	if (segments[1] == DESTROYED)
	{
		cout << "INVALID OPERATION \"SHOOT " << id << " " << type << " " << y <<" "<< x << "\": SHIP CANNOT SHOOT" << endl;
		exit(0);
	}
	if (shootsLeft == 0)
	{
		cout << "INVALID OPERATION \"SHOOT " << id << " " << type << " " << y << " " << x << "\": TOO MANY SHOOTS" << endl;
		exit(0);
	}
	if (y < 0 || y >= boardY || x < 0 || x >= boardX)
	{
		cout << "INVALID OPERATION \"SHOOT " << id << " " << type << " " << y << " " << x << "\": FIELD DOES NOT EXIST" << endl;
		exit(0);
	}
	int cannonX=0, cannonY=0;
	switch (direction)
	{
	case 'N':
		cannonX = position.x;
		cannonY = position.y + 1;
		break;
	case 'S':
		cannonX = position.x;
		cannonY = position.y - 1;
		break;
	case 'E':
		cannonX = position.x - 1;
		cannonY = position.y;
		break;
	case 'W':
		cannonX = position.x + 1;
		cannonY = position.y;
		break;
	}
	double distance = sqrt(pow(abs(cannonY - y), 2) + pow(abs(cannonX - x), 2));
	if (distance > size && size != 5)
	{
		cout << "INVALID OPERATION \"SHOOT " << id << " " << type << " " << y << " " << x << "\": SHOOTING TOO FAR" << endl;
		exit(0);
	}
	for (int i = 0; i < shipCount; i++)
	{
		for (int j = 0; j < opponent[i].size; j++)
		{
			switch (opponent[i].direction)
			{
			case 'N':
				if (opponent[i].position.x == x && opponent[i].position.y + j == y)
					opponent[i].segments[j] = DESTROYED;
				break;
			case 'S':
				if (opponent[i].position.x == x && opponent[i].position.y - j == y)
					opponent[i].segments[j] = DESTROYED;
				break;
			case 'E':
				if (opponent[i].position.x - j == x && opponent[i].position.y == y)
					opponent[i].segments[j] = DESTROYED;
				break;
			case 'W':
				if (opponent[i].position.x + j == x && opponent[i].position.y == y)
					opponent[i].segments[j] = DESTROYED;
				break;
			}
		}
	}
	shootsLeft--;
}