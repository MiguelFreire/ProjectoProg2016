#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "config.h"
#include "players.h"
#include "cards.h"
#include "util.h"
#include "gameMechanics.h"

GameTable createGameTable(){
	GameTable tmp = {0};
	return tmp;
}

bool slotIsEmpty(PlayerNode *slot){
	return (slot == NULL);
}

void actionHit(GameTable *table, Pile *cardPile, ActionSubject subject) {
	if(subject == PLAYER) {
		Player *player = &(table->slots[table->currentPlayer]->player);
		player->state = HIT;
		player->hand = pushToHand(player->hand, dealCard(cardPile), &player->numCards);
		player->handValue = updatePlayerHandValue(player);
		printf("%d", player->state);
		if(player->state == BUSTED || player->state == BLACKJACK) actionStand(table);
	} else if(subject == HOUSE) {
		House *house = table->house;
		house->hand = pushToHand(house->hand, dealCard(cardPile), &house->numCards);
		house->handValue = updateHouseHandValue(house);
		if(house->state == HOUSE_BUSTED || house->state == HOUSE_BLACKJACK) actionStand(table);
	}

}

void actionStand(GameTable *table) {
	do {
		table->currentPlayer++;
	} while (slotIsEmpty(table->slots[table->currentPlayer]) ||
		table->slots[table->currentPlayer]->player.state != STANDARD); // next player has a BLACKJACK

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
	printf("What's the name of the player?\n");
	printf("(Write CANCEL to quit)\n");

	char playerName[MAX_BUFFER_SIZE];

	fgets(playerName, MAX_BUFFER_SIZE, stdin);
	playerName[strlen(playerName)-1] = '\0';

	Player *player = NULL;

	if(strcmp("CANCEL",playerName) == 0) return;

	for(int i = 0; i < TABLE_SLOTS; i++) {
		if(!slotIsEmpty(table->slots[i]) && strcmp(table->slots[i]->player.name, playerName) == 0) {
			player = &(table->slots[i]->player);
		}
	}

	if(player == NULL) {
		printf("Player not found\n");
		actionBet(table);
	}

	char buffer[MAX_BUFFER_SIZE];
	int newBet = 0;
	bool error = false;
	do {
		if(error) printf("Player's bet value must be between: %d and %.2f\n", 2, 0.25*player->money);
		printf("New bet?\n");
		fgets(buffer, MAX_BUFFER_SIZE, stdin);
		sscanf(buffer, "%d", &newBet);
   } while(!(isBetween(newBet, 2, 0.25*player->money)) && (error = true));

   player->bet = newBet;
   printf("Bet set\n");

   return;
}
