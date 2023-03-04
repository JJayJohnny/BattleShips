#include <iostream>
#include <string.h>
#include "game.h"

const int maxTagLength = 15;
using namespace std;

void stateCommands(gameState& game);
void playerCommands(gameState& game, int player);

int main()
{
	gameState game;
	char group[maxTagLength];
	game.startGame();

	while (cin >> group)
	{
		if (strcmp(group, "[state]")==0)
		{
			stateCommands(game);
		}
		else if (strcmp(group, "[playerA]") == 0)
		{
			if (game.expectedPlayer == 1)
			{
				cout << "INVALID OPERATION \"[playerA] \": THE OTHER PLAYER EXPECTED" << endl;
				exit(0);
			}
			playerCommands(game, 0);
		}
		else if (strcmp(group, "[playerB]") == 0)
		{
			if (game.expectedPlayer == 0)
			{
				cout<< "INVALID OPERATION \"[playerB] \": THE OTHER PLAYER EXPECTED" << endl;
				exit(0);
			}
			playerCommands(game, 1);
		}
	}
	return 0;
}

void stateCommands(gameState& game)
{
	char command[maxTagLength];
	while (cin>>command)
	{
		if (strcmp(command, "[state]") == 0)
			break;
		else if (strcmp(command, "PRINT") == 0)
		{
			int x;
			cin >> x;
			game.statePrint(x);
		}
		else if (strcmp(command, "SET_FLEET") == 0)
		{
			game.setFleet();
		}
		else if (strcmp(command, "NEXT_PLAYER") == 0)
			game.nextPlayer();
		else if (strcmp(command, "BOARD_SIZE") == 0)
			game.setBoardSize();
		else if (strcmp(command, "INIT_POSITION") == 0)
			game.initPosition();
		else if (strcmp(command, "REEF") == 0)
			game.placeReef();
		else if (strcmp(command, "SHIP") == 0)
			game.ship();
		else if (strcmp(command, "EXTENDED_SHIPS") == 0)
			game.extendedShips = abs(game.extendedShips - 1);
		else if (strcmp(command, "SAVE") == 0)
			game.save();
	}
}

void playerCommands(gameState& game, int player)
{
	game.players[player].newRound();
	char command[maxTagLength];
	while (cin>>command)
	{
		if (strcmp(command, "[playerA]") == 0 || strcmp(command, "[playerB]") == 0)
		{
			game.checkVictory();
			game.expectedPlayer = abs(player - 1);
			game.players[player].shootsLeft = 1;
			break;
		}
		else if (strcmp(command, "PLACE_SHIP") == 0)
			game.players[player].placeShip(game.prepareBoard(), game.field.x, game.field.y, 0, player + 'A');
		else if (strcmp(command, "SHOOT") == 0)
		{
			if (game.extendedShips == 0)
				game.players[player].shoot(game.players[abs(player - 1)], game.field.x, game.field.y);
			else
			{
				int id;
				char tag[4];
				cin >> id >> tag;
				int opponentFleet = game.players[abs(player - 1)].fleet[0] + game.players[abs(player - 1)].fleet[1] + game.players[abs(player - 1)].fleet[2] + game.players[abs(player - 1)].fleet[3] - game.players[abs(player - 1)].shipsLeftToPlace;
				game.players[player].ships[game.players[player].findShipByIdAndSize(giveSizeFromType(tag), id)].shoot(game.players[abs(player - 1)].ships, opponentFleet, game.field.x, game.field.y);
			}
		}
		else if (strcmp(command, "MOVE") == 0)
		{
			int id;
			char tag[4];
			cin >> id >> tag;
			game.players[player].ships[game.players[player].findShipByIdAndSize(giveSizeFromType(tag), id)].move(game.prepareBoard(), game.field.x, game.field.y, game.extendedShips);
		}
		else if (strcmp(command, "SPY") == 0)
			game.players[player].spy(game.field.x, game.field.y);
		else if (strcmp(command, "PRINT") == 0)
		{
			int x;
			cin >> x;
			if (x == 0)
				game.players[player].print(game.prepareBoard(), game.field.x, game.field.y, 0);
			else if (x == 1)
				game.players[player].print(game.prepareBoardAdvanced(), game.field.x, game.field.y, 1);
		}
	}
}