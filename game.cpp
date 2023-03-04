#include "game.h"
#include "player.h"
#include "ship.h"
#include <iostream>
using namespace std;

void gameState::startGame()
{	
	players[0].yMin = 0;
	players[0].yMax = 9;
	players[1].yMin = 11;
	players[1].yMax = 20;
	players[0].xMin = 0;
	players[0].xMax = 9;
	players[1].xMin = 0;
	players[1].xMax = 9;

	players[0].initShips();
	players[1].initShips();

	field.x = defaultBoardX;
	field.y = defaultBoardY;
	possibleReefs = field.x * field.y;
	reefs = new coordinates[possibleReefs];
	reefCount = 0;
	extendedShips = 0;
	players[0].spyPlanes = new coordinates[100];
	players[1].spyPlanes = new coordinates[100];
	players[0].spyPlanesCount = 0;
	players[1].spyPlanesCount = 0;
}

void gameState::setFleet()
{
	char player;
	int a1, a2, a3, a4;
	cin >> player >> a1 >> a2 >> a3 >> a4;
	if (a1 >= 0 && a2 >= 0 && a3 >= 0 && a4 >= 0)
	{
		players[player - 'A'].fleet[0] = a1;
		players[player - 'A'].fleet[1] = a2;
		players[player - 'A'].fleet[2] = a3;
		players[player - 'A'].fleet[3] = a4;
		players[player - 'A'].shipsLeftToPlace = a1 + a2 + a3 + a4;
		players[player - 'A'].initShips();
	}
}
void gameState::statePrint(int printMode)
{
	char** board;

	if (printMode == 0)
		board = prepareBoard();
	else
		board = prepareBoardAdvanced();
	
	if (boardEmpty(board, field.y, field.x) == 0)
	{
		if (printMode == 1)
		{
			if (field.x > 10)
			{
				cout << "  ";
				for (int i = 0; i < field.x; i++)cout << (int)i / 10;
				cout << endl;
			}
			cout << "  ";
			for (int i = 0; i < field.x; i++)cout << i % 10;
			cout << endl;
		}
		for (int i = 0; i < field.y; i++)
		{
			if (printMode == 1)
			{
				cout << (int)i / 10 << i % 10;
			}
			for (int j = 0; j < field.x; j++)
			{
				cout << board[i][j];
			}
			cout << endl;
		}
		cout << "PARTS REMAINING:: A : " << players[0].countAliveParts() << " B : " << players[1].countAliveParts() << endl;
	}

	for (int i = 0; i < field.y; i++)
		delete[] board[i];
	delete[] board;
}
void gameState::nextPlayer()
{
	char player;
	cin >> player;
	expectedPlayer = player - 'A';
}

int boardEmpty(char** board, int y, int x)
{
	for (int i = 0; i < y; i++)
		for (int j = 0; j < x; j++)
			if (board[i][j] != ' ')
				return 0;
	return 1;
}

void gameState::checkVictory()
{
	if (players[0].shipsLeftToPlace == 0 && players[1].shipsLeftToPlace == 0)
	{
		int segmentsA = players[0].countAliveParts();
		int segmentsB = players[1].countAliveParts();
		if (segmentsA == 0)
		{
			cout << "B won" << endl;
			exit(0);
		}
		if (segmentsB == 0)
		{
			cout << "A won" << endl;
			exit(0);
		}
	}
}

void gameState::setBoardSize()
{
	int y, x;
	cin >> y >> x;
	field.x = x;
	field.y = y;
	possibleReefs = field.x * field.y;
	reefs = new coordinates[possibleReefs];
	reefCount = 0;
}

void gameState::initPosition()
{
	char player;
	int y1, y2, x1, x2;
	cin >> player >> y1 >> x1 >> y2 >> x2;
	players[player - 'A'].yMin = y1;
	players[player - 'A'].yMax = y2;
	players[player - 'A'].xMin = x1;
	players[player - 'A'].xMax = y2;
}

void gameState::placeReef()
{
	int y, x;
	cin >> y >> x;
	if (y<0 || y>field.y-1 || x<0 || x>field.x-1)
	{
		cout << "INVALID OPERATION \"REEF " << y << " " << x << "\": REEF IS NOT PLACED ON BOARD" << endl;
		exit(0);
	}
	reefs[reefCount].x = x;
	reefs[reefCount].y = y;
	reefCount++;
}

char** gameState::prepareBoard()
{
	char** board = new char* [field.y];
	for (int i = 0; i < field.y; i++)
		board[i] = new char[field.x];

	for (int i = 0; i < field.y; i++)
	{
		for (int j = 0; j < field.x; j++)
		{
			board[i][j] = ' ';
		}
	}

	for (int p = 0; p < 2; p++)
	{
		int fleet = players[p].fleet[0] + players[p].fleet[1] + players[p].fleet[2] + players[p].fleet[3] - players[p].shipsLeftToPlace;
		for (int i = 0; i < fleet; i++)
		{
			if (players[p].ships[i].size > 1 && players[p].ships[i].size <= 5)
			{
				for (int j = 0; j < players[p].ships[i].size; j++)
				{
					switch (players[p].ships[i].direction)
					{
					case 'N':
						board[players[p].ships[i].position.y + j][players[p].ships[i].position.x] = segmentStates[players[p].ships[i].segments[j]];
						break;
					case 'E':
						board[players[p].ships[i].position.y][players[p].ships[i].position.x - j] = segmentStates[players[p].ships[i].segments[j]];
						break;
					case 'S':
						board[players[p].ships[i].position.y - j][players[p].ships[i].position.x] = segmentStates[players[p].ships[i].segments[j]];
						break;
					case 'W':
						board[players[p].ships[i].position.y][players[p].ships[i].position.x + j] = segmentStates[players[p].ships[i].segments[j]];
						break;
					default:
						break;
					}
				}
			}
		}
	}

	for (int i = 0; i < reefCount; i++)
	{
		board[reefs[i].y][reefs[i].x] = REEF;
	}
	return board;
}

void gameState::ship()
{
	char player;
	cin >> player;
	players[player - 'A'].placeShip(prepareBoard(), field.x, field.y, 1, player);
}

char** gameState::prepareBoardAdvanced()
{
	char** board = new char* [field.y];
	for (int i = 0; i < field.y; i++)
		board[i] = new char[field.x];

	for (int i = 0; i < field.y; i++)
	{
		for (int j = 0; j < field.x; j++)
		{
			board[i][j] = ' ';
		}
	}

	for (int p = 0; p < 2; p++)
	{
		int fleet = players[p].fleet[0] + players[p].fleet[1] + players[p].fleet[2] + players[p].fleet[3] - players[p].shipsLeftToPlace;
		for (int i = 0; i < fleet; i++)
		{
			if (players[p].ships[i].size > 1 && players[p].ships[i].size <= 5)
			{
				for (int j = 0; j < players[p].ships[i].size; j++)
				{
					int segmentX = 0, segmentY = 0;
					switch (players[p].ships[i].direction)
					{
					case 'N':
						segmentY = players[p].ships[i].position.y + j;
						segmentX = players[p].ships[i].position.x;
						break;
					case 'E':
						segmentY = players[p].ships[i].position.y;
						segmentX = players[p].ships[i].position.x - j;
						break;
					case 'S':
						segmentY = players[p].ships[i].position.y - j;
						segmentX = players[p].ships[i].position.x;
						break;
					case 'W':
						segmentY = players[p].ships[i].position.y;
						segmentX = players[p].ships[i].position.x + j;
						break;
					default:
						break;
					}
					if (players[p].ships[i].segments[j] == DESTROYED)
						board[segmentY][segmentX] = segmentStates[DESTROYED];
					else
					{
						if (j == 0)
							board[segmentY][segmentX] = RADAR;
						else if (j == 1 && players[p].ships[i].size > 2)
							board[segmentY][segmentX] = CANNON;
						else if (j == players[p].ships[i].size - 1)
							board[segmentY][segmentX] = ENGINE;
						else
							board[segmentY][segmentX] = segmentStates[HEALTHY];
					}
				}
			}
		}
	}

	for (int i = 0; i < reefCount; i++)
	{
		board[reefs[i].y][reefs[i].x] = REEF;
	}
	return board;
}

void gameState::save()
{
	cout << "[state]" << endl;
	cout << "BOARD_SIZE " << field.y << " " << field.x << endl;
	cout << "NEXT_PLAYER " << (char)(expectedPlayer + 'A') << endl;
	for (int p = 0; p < 2; p++)
	{
		cout << "INIT_POSITION " << (char)(p + 'A') << " " << players[p].yMin << " " << players[p].xMin << " " << players[p].yMax << " " << players[p].xMax << endl;
		cout << "SET_FLEET " << (char)(p + 'A') << " " << players[p].fleet[0] << " " << players[p].fleet[1] << " " << players[p].fleet[2] << " " << players[p].fleet[3] << endl;
		for (int type = 5; type >= 2; type--)
		{
			for (int i = 0; i < players[p].fleet[abs(type-5)]; i++)
			{
				int n = players[p].findShipByIdAndSize(type, i);
				cout << "SHIP " << (char)(p + 'A') << " " << players[p].ships[n].position.y << " " << players[p].ships[n].position.x << " " << players[p].ships[n].direction << " " << i << " " << players[p].ships[n].type << " ";
				for (int j = 0; j < players[p].ships[n].size; j++) cout << players[p].ships[n].segments[j];
				cout << endl;
			}
		}
	}
	for (int i = 0; i < reefCount; i++)
		cout << "REEF " << reefs[i].y << " " << reefs[i].x << endl;
	if (extendedShips == 1)
		cout << "EXTENDED_SHIPS" << endl;
	cout << "[state]" << endl;
}