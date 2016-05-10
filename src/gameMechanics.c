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

bool slotIsEmpty(PlayerNode *slot){
	return (slot == NULL);
}
/*
void actionHit(GameTable *table, Pile *cardPile, ActionSubject subject) {
	if(subject == PLAYER) {
		Player *player = &(table->slots[table->currentPlayer]->player);
		player->state = HIT;
		player->hand = pushToHand(player->hand, dealCard(cardPile), &player->numCards);
		player->numCards++;
		player->handValue = updatePlayerHandValue(player, player->numCards);
		if(player->state == BUSTED || player->state == BLACKJACK) actionStand(table);
	} else if(subject == HOUSE) {
		House *house = table->house;
		house->hand = pushToHand(house->hand, dealCard(cardPile), &house->numCards);
		house->numCards++;
		house->handValue = updateHouseHandValue(house, house->numCards);
		if(house->state == HOUSE_BUSTED || house->state == HOUSE_BLACKJACK) actionStand(table);
	}

}
*/
void actionStand(GameTable *table) {
	table->currentPlayer++;
}

void actionNewGame(GameTable *table, Pile *cardPile) {
	PlayerNode *curPlayer = NULL;
	// deal 2 cards to each player and house
	for (int i = 0; i < 2; i++){
		// hand a card to each player
		for (int j = 0; j < TABLE_SLOTS; j++){
			if(!slotIsEmpty(table->slots[j])){
				curPlayer = table->slots[j];
				curPlayer->player.hand = pushToHand(curPlayer->player.hand, dealCard(cardPile), &curPlayer->player.numCards);
			}
		}
	// hand a card to the house
	table->house->hand = pushToHand(table->house->hand , dealCard(cardPile), &table->house->numCards);
	}
}

/*void actionDouble(GameTable *table, Pile *cardPile) {
	Player *player = &(table->slots[table->currentPlayer]->player);
	if(player->state == HIT) return;

	player->money -= player->bet;
	player->betMultiplier = 2;

	actionHit(table, cardPile, PLAYER);
	actionStand(table);
}
*/
void actionSurrender(GameTable *table) {
	Player *player = &(table->slots[table->currentPlayer]->player);
	if(player->state == HIT) return;

	player->state = SURRENDERED;

	actionStand(table);
}

void actionBet(GameTable *table) {

}

void modifyBet();
