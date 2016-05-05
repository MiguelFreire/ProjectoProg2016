#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gameMechanics.h"
#include "players.h"

GameTable createGameTable(){
	GameTable tmp = {0};
	return tmp;
}

void actionHit(GameTable *table) {
	Player player = table->slots[table.currentPlayer]->player;
	//deal card to player's hand
}

void actionStand(GameTable *table) {
	table->currentPlayer++;
}

void actionNewGame(GameTable table) {

}

void actionDouble(GameTable table) {

}

void actionSurrender(GameTable table) {

}

void actionBet(GameTable table) {
	
}
