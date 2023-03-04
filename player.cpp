#include "player.h"
#include "ship.h"
#include "game.h"
#include <iostream>
#include <math.h>
using namespace std;


void player::initShips()
{
	
	ships = new ship[fleet[0] + fleet[1] + fleet[2] + fleet[3]];

}

int player::findShipByIdAndSize(int size, int id)
{
	for (int i = 0; i < fleet[0] + fleet[1] + fleet[2] + fleet[3] - shipsLeftToPlace; i++)
	{
		if (ships[i].id == id && ships[i].size == size)
			return i;
	}
		return -1;
}

int player::countShipsByType(char type[4])
{
	int counter = 0;
	for (int i = 0; i < fleet[0] + fleet[1] + fleet[2] + fleet[3] - shipsLeftToPlace; i++)
	{
		if (strcmp(ships[i].type, type) == 0)
			counter++;
	}
	return counter;
}

int player::countAliveParts()
{
	int counter = 0;
	for (int i = 0; i < fleet[0] + fleet[1] + fleet[2] + fleet[3] - shipsLeftToPlace; i++)
		counter += ships[i].countAliveSegments();
	return counter;
}

void player::placeShip(char** board, int boardX, int boardY, int ifFromState, char player)
{
	if(ships==nullptr)initShips();
	int y, x, i;
	char D, shipClass[4];
	cin >> y >> x >> D >> i >> shipClass;
	int size = giveSizeFromType(shipClass);
	int n = fleet[0] + fleet[1] + fleet[2] + fleet[3] - shipsLeftToPlace;

	char* segments = new char[size+1];//for execution from state
	if (ifFromState)
	{	
		cin >> segments;
	}

	if (y < yMin || y>yMax || x<xMin || x>xMax || (D == 'N' && y + size - 1 > yMax) || (D == 'E' && x - size + 1 < xMin) || (D == 'S' && y - size + 1 < yMin) || (D == 'W' && x + size - 1 > xMax))
	{
		if (ifFromState != 1)
		{
			cout << "INVALID OPERATION \"PLACE_SHIP " << y << " " << x << " " << D << " " << i << " " << shipClass << "\": NOT IN STARTING POSITION" << endl;
			exit(0);
		}
	}
	else if (findShipByIdAndSize(size, i) != -1)
	{
		if(ifFromState)
			cout << "INVALID OPERATION \"SHIP " << player << " " << y << " " << x << " " << D << " " << i << " " << shipClass << " " << segments << "\": SHIP ALREADY PRESENT" << endl;
		else
			cout << "INVALID OPERATION \"PLACE_SHIP " << y << " " << x << " " << D << " " << i << " " << shipClass << "\": SHIP ALREADY PRESENT"<<endl;
		exit(0);
	}
	else if (countShipsByType(shipClass)>=fleet[abs(5-size)])
	{
		if(ifFromState)
			cout << "INVALID OPERATION \"SHIP " << player << " " << y << " " << x << " " << D << " " << i << " " << shipClass << " " << segments << "\": ALL SHIPS OF THE CLASS ALREADY SET" << endl;
		else
			cout << "INVALID OPERATION \"PLACE_SHIP " << y << " " << x << " " << D << " " << i << " " << shipClass << "\": ALL SHIPS OF THE CLASS ALREADY SET"<<endl;
		exit(0);
	}
	for (int j = 0; j < size; j++)//sprawdzenie czy zaden segment nie jest na rafie albo obok innego statku
	{
		int checkX = 0, checkY = 0;
		switch (D)
		{
		case 'N':
			checkX = x;
			checkY = y + j;
			break;
		case 'S':
			checkX = x;
			checkY = y - j;
			break;
		case'E':
			checkX = x - j;
			checkY = y;
			break;
		case'W':
			checkX = x + j;
			checkY = y;
			break;
		}
		if (board[checkY][checkX] == REEF)//check if ship on reef
		{
			if(ifFromState)
				cout << "INVALID OPERATION \"SHIP " << player << " " << y << " " << x << " " << D << " " << i << " " << shipClass << " " << segments << "\": PLACING SHIP ON REEF" << endl;
			else
				cout << "INVALID OPERATION \"PLACE_SHIP " << y << " " << x << " " << D << " " << i << " " << shipClass << "\": PLACING SHIP ON REEF" << endl;
			exit(0);
		}
		else if ((checkX - 1 >= 0) && board[checkY][checkX - 1] == segmentStates[1])//check 4 sides for other ships nearby
		{
			if(ifFromState)
				cout << "INVALID OPERATION \"SHIP " << player << " " << y << " " << x << " " << D << " " << i << " " << shipClass << " " << segments << "\": PLACING SHIP TOO CLOSE TO OTHER SHIP" << endl;
			else
				cout << "INVALID OPERATION \"PLACE_SHIP " << y << " " << x << " " << D << " " << i << " " << shipClass << "\": PLACING SHIP TOO CLOSE TO OTHER SHIP" << endl;
			exit(0);
		}
		else if((checkX + 1 < boardX) && board[checkY][checkX +1] == segmentStates[1])
		{
			if(ifFromState)
				cout << "INVALID OPERATION \"SHIP " << player << " " << y << " " << x << " " << D << " " << i << " " << shipClass << " " << segments << "\": PLACING SHIP TOO CLOSE TO OTHER SHIP" << endl;
			else
				cout << "INVALID OPERATION \"PLACE_SHIP " << y << " " << x << " " << D << " " << i << " " << shipClass << "\": PLACING SHIP TOO CLOSE TO OTHER SHIP" << endl;
			exit(0);
		}
		else if((checkY - 1 >= 0) && board[checkY-1][checkX] == segmentStates[1])
		{
			if(ifFromState)
				cout << "INVALID OPERATION \"SHIP " << player << " " << y << " " << x << " " << D << " " << i << " " << shipClass << " " << segments << "\": PLACING SHIP TOO CLOSE TO OTHER SHIP" << endl;
			else
				cout << "INVALID OPERATION \"PLACE_SHIP " << y << " " << x << " " << D << " " << i << " " << shipClass << "\": PLACING SHIP TOO CLOSE TO OTHER SHIP" << endl;
			exit(0);
		}
		else if((checkY + 1 < boardY) && board[checkY+1][checkX] == segmentStates[1])
		{
			if(ifFromState)
				cout << "INVALID OPERATION \"SHIP " << player << " " << y << " " << x << " " << D << " " << i << " " << shipClass << " " << segments << "\": PLACING SHIP TOO CLOSE TO OTHER SHIP" << endl;
			else
				cout << "INVALID OPERATION \"PLACE_SHIP " << y << " " << x << " " << D << " " << i << " " << shipClass << "\": PLACING SHIP TOO CLOSE TO OTHER SHIP" << endl;
			exit(0);
		}
	}
	if(n>=0 && n<fleet[0]+ fleet[1] + fleet[2] + fleet[3])
	{
		ships[n].id = i;
		ships[n].position.x = x;
		ships[n].position.y = y;
		ships[n].status = 1;
		ships[n].size = size;
		ships[n].segments = new int[ships[n].size];
		for (int j = 0; j < ships[n].size; j++)
			ships[n].segments[j] = 1;	
		ships[n].direction = D;
		strcpy_s(ships[n].type, shipClass);
		shipsLeftToPlace--;
		if (ifFromState)
		{
			for (int j = 0; j < size; j++)
				ships[n].segments[j] = segments[j] - '0';
		}
	}
	delete[] segments;
}

void player::shoot(player otherPlayer, int maxX, int maxY)
{
	int y, x;
	cin >> y >> x;
	if (y<0 || y>maxY - 1 || x<0 || x>maxX - 1)
	{
		cout << "INVALID OPERATION \"SHOOT " << y << " " << x << "\": FIELD DOES NOT EXIST";
		exit(0);
	}
	else if(shipsLeftToPlace>0 || otherPlayer.shipsLeftToPlace>0)
	{
		cout << "INVALID OPERATION \"SHOOT " << y << " " << x << "\": NOT ALL SHIPS PLACED";
		exit(0);
	}
	else
	{
		int oponentFleet = otherPlayer.fleet[0] + otherPlayer.fleet[1] + otherPlayer.fleet[2] + otherPlayer.fleet[3];
		for (int i = 0; i < oponentFleet; i++)
		{
			for (int j = 0; j < otherPlayer.ships[i].size; j++)
			{
				switch (otherPlayer.ships[i].direction)
				{
				case 'N':
					if (otherPlayer.ships[i].position.x == x && otherPlayer.ships[i].position.y + j == y)
					{
						otherPlayer.ships[i].segments[j] = DESTROYED;
					}
					break;
				case 'S':
					if (otherPlayer.ships[i].position.x == x && otherPlayer.ships[i].position.y - j == y)
					{
						otherPlayer.ships[i].segments[j] = DESTROYED;
					}
					break;
				case 'E':
					if (otherPlayer.ships[i].position.x - j == x && otherPlayer.ships[i].position.y == y)
					{
						otherPlayer.ships[i].segments[j] = DESTROYED;
					}
					break;
				case 'W':
					if (otherPlayer.ships[i].position.x + j == x && otherPlayer.ships[i].position.y == y)
					{
						otherPlayer.ships[i].segments[j] = DESTROYED;
					}
					break;
				}
			}
		}
	}
}

void player::newRound()
{
	for (int i = 0; i < fleet[0] + fleet[1] + fleet[2] + fleet[3] - shipsLeftToPlace; i++)
	{
		if (ships[i].size == 5)
			ships[i].movesLeft = 2;
		else
			ships[i].movesLeft = 3;
		ships[i].shootsLeft = ships[i].size;
	}
	shootsLeft = 1;
}

void player::spy(int boardX, int boardY)
{
	int id, y, x;
	cin >> id >> y >> x;
	int carier = findShipByIdAndSize(5, id);
	if (carier == -1)
	{
		cout << "INVALID OPERATION \"SPY " << id << " " << y << " " << x << "\": CARRIER NOT FOUND";
		exit(0);
	}
	if (x < 0 || x >= boardX || y < 0 || y >= boardY)
	{
		cout << "INVALID OPERATION \"SPY " <<id<<" "<< y << " " << x << "\": FIELD DOES NOT EXIST";
		exit(0);
	}
	if (ships[carier].shootsLeft == 0)
	{
		cout << "INVALID OPERATION \"SPY " << id << " " << y << " " << x << "\": ALL PLANES SENT";
		exit(0);
	}
	if (ships[carier].segments[1] == DESTROYED)
	{
		cout << "INVALID OPERATION \"SPY " << id << " " << y << " " << x << "\": CANNOT SEND PLANE";
		exit(0);
	}
	
	spyPlanes[spyPlanesCount].x = x;
	spyPlanes[spyPlanesCount].y = y;
	spyPlanesCount++;
	ships[carier].shootsLeft--;
}

void player::print(char** board, int boardX, int boardY, int printMode)
{
	char** filter = new char*[boardY];
	for (int i = 0; i < boardY; i++)
		filter[i] = new char[boardX];

	for (int i = 0; i < boardY; i++)
		for (int j = 0; j < boardX; j++)
			filter[i][j] = FOG;

	for (int i = 0; i < fleet[0] + fleet[1] + fleet[2] + fleet[3] - shipsLeftToPlace; i++)
	{
		if (ships[i].segments[0] == HEALTHY)
		{
			for (int y = 0; y < boardY; y++)
			{
				for (int x = 0; x < boardX; x++)
				{
					double distance = sqrt(pow(abs(ships[i].position.y - y), 2) + pow(abs(ships[i].position.x - x), 2));
					if (distance <= ships[i].size)
						filter[y][x] = ' ';
				}
			}
		}
		else
		{
			for (int j = 0; j < ships[i].size; j++)//wyswietlamy tylko statek
			{
				switch (ships[i].direction)
				{
				case 'N':
					filter[ships[i].position.y + j][ships[i].position.x] = ' ';
					break;
				case 'S':
					filter[ships[i].position.y - j][ships[i].position.x] = ' ';
					break;
				case 'E':
					filter[ships[i].position.y][ships[i].position.x - j] = ' ';
					break;
				case 'W':
					filter[ships[i].position.y][ships[i].position.x + j] = ' ';
					break;
				}
			}
			//oraz pola dookola radaru
			if (ships[i].position.x - 1 >= 0)
				filter[ships[i].position.y][ships[i].position.x - 1] = ' ';
			if (ships[i].position.x + 1 < boardX)
				filter[ships[i].position.y][ships[i].position.x + 1] = ' ';
			if (ships[i].position.y - 1 >= 0)
				filter[ships[i].position.y - 1][ships[i].position.x] = ' ';
			if (ships[i].position.y + 1 < boardY)
				filter[ships[i].position.y + 1][ships[i].position.x] = ' ';
		}
	}

	for (int i = 0; i < spyPlanesCount; i++)
	{
		filter[spyPlanes[i].y][spyPlanes[i].x] = ' ';
		if (spyPlanes[i].x - 1 >= 0 && spyPlanes[i].y - 1 >= 0) filter[spyPlanes[i].y - 1][spyPlanes[i].x - 1] = ' ';
		if (spyPlanes[i].x - 1 >= 0) filter[spyPlanes[i].y][spyPlanes[i].x - 1] = ' ';
		if (spyPlanes[i].x - 1 >= 0 && spyPlanes[i].y + 1 < boardY) filter[spyPlanes[i].y + 1][spyPlanes[i].x - 1] = ' ';
		if (spyPlanes[i].y - 1 >= 0) filter[spyPlanes[i].y - 1][spyPlanes[i].x] = ' ';
		if (spyPlanes[i].y + 1 >= 0) filter[spyPlanes[i].y + 1][spyPlanes[i].x] = ' ';
		if (spyPlanes[i].x + 1 >= 0 && spyPlanes[i].y - 1 >= 0) filter[spyPlanes[i].y - 1][spyPlanes[i].x + 1] = ' ';
		if (spyPlanes[i].x + 1 >= 0) filter[spyPlanes[i].y][spyPlanes[i].x + 1] = ' ';
		if (spyPlanes[i].x + 1 >= 0 && spyPlanes[i].y + 1 >= 0) filter[spyPlanes[i].y + 1][spyPlanes[i].x + 1] = ' ';
	}

	if (printMode == 1)
	{
		if (boardX > 10)
		{
			cout << "  ";
			for (int i = 0; i < boardX; i++)cout << (int)i / 10;
			cout << endl;
		}
		cout << "  ";
		for (int i = 0; i < boardX; i++)cout << i % 10;
		cout << endl;
	}
	for (int i = 0; i < boardY; i++)
	{
		if (printMode == 1)
		{
			cout << (int)i / 10 << i % 10;
		}
		for (int j = 0; j < boardX; j++)
		{
			if (filter[i][j] != FOG)
				cout << board[i][j];
			else
				cout << FOG;
		}
		cout << endl;
	}
	
	for (int i = 0; i < boardY; i++)
		delete[] filter[i];
	delete[] filter;
}

