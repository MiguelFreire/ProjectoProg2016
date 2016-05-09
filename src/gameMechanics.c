#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "config.h"
#include "players.h"
#include "cards.h"
#include "gameMechanics.h"

GameTable createGameTable(){
	GameTable tmp = {0};
	return tmp;
}

void actionHit(GameTable *table, Pile *cardPile, ActionSubject subject) {
	if(subject == PLAYER) {
		Player *player = &(table->slots[table->currentPlayer]->player);
		player->state = HIT;
		player->hand = pushToHand(player->hand, dealCard(cardPile), &player->numCards);
		player->numCards++;
	} else if(subject == HOUSE) {
		House *house = table->house;
		house->hand = pushToHand(house->hand, dealCard(cardPile), &house->numCards);
		house->numCards++;
		//same for house
		//Need a function to calculate handValue and check if there is a BlackJack or a Bust
		//Update state here!!!
	}

}

void actionStand(GameTable *table) {
	table->currentPlayer++;
}

void actionNewGame(GameTable *table) {

}

void actionDouble(GameTable *table, Pile *cardPile) {
	Player *player = &(table->slots[table->currentPlayer]->player);
	if(player->state == HIT) return;

	player->money -= player->bet;
	player->betMultiplier = 2;

	actionHit(table, cardPile, PLAYER);
	actionStand(table);
}

void actionSurrender(GameTable *table) {
	Player *player = &(table->slots[table->currentPlayer]->player);
	if(player->state == HIT) return;

	player->state = SURRENDERED;

	actionStand(table);
}

void actionBet(GameTable *table) {

}

void modifyBet();
